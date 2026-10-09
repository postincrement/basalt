#!/usr/bin/env python3
"""Run a CP/M .COM produced by basalt's Z80 backend.

Usage: cpmrun.py program.com
Prints console output. Exits 0 when the program warm-boots.
"""

import sys


class Z80:
    def __init__(self, image):
        self.mem = bytearray(0x10000)
        self.mem[0x100:0x100 + len(image)] = image
        # JP wboot, JP bdos, BDOS address word at 0006.
        self.mem[0] = 0xC3
        self.mem[1] = 0x03
        self.mem[2] = 0xFE
        self.mem[5] = 0xC3
        self.mem[6] = 0x00
        self.mem[7] = 0xFD
        # BIOS jump table. WBOOT is at FE03, CONOUT JP is WBOOT+9 = FE0C.
        for off, dest in ((0, 0xFE20), (3, 0xFE20), (6, 0xFE30), (9, 0xFE30), (12, 0xFE40)):
            self.mem[0xFE00 + off] = 0xC3
            self.mem[0xFE01 + off] = dest & 0xFF
            self.mem[0xFE02 + off] = dest >> 8
        self.pc = 0x100
        self.sp = 0xFD00
        self.a = self.b = self.c = self.d = self.e = self.h = self.l = 0
        self.ap = self.bp = self.cp = self.dp = self.ep = self.hp = self.lp = self.fp = 0
        self.ix = self.iy = 0
        self.i = self.r = 0
        self.iff = False
        self.f = 0
        self.out = []
        self.halted = False
        self.stdin = None
        self.stdin_i = 0

    def rb(self, addr):
        return self.mem[addr & 0xFFFF]

    def rw(self, addr):
        return self.rb(addr) | (self.rb(addr + 1) << 8)

    def wb(self, addr, val):
        self.mem[addr & 0xFFFF] = val & 0xFF

    def ww(self, addr, val):
        self.wb(addr, val)
        self.wb(addr + 1, val >> 8)

    def fetch(self):
        val = self.rb(self.pc)
        self.pc = (self.pc + 1) & 0xFFFF
        return val

    def fetchw(self):
        lo = self.fetch()
        hi = self.fetch()
        return lo | (hi << 8)

    def push(self, val):
        self.sp = (self.sp - 2) & 0xFFFF
        self.ww(self.sp, val)

    def pop(self):
        val = self.rw(self.sp)
        self.sp = (self.sp + 2) & 0xFFFF
        return val

    @property
    def hl(self):
        return (self.h << 8) | self.l

    @hl.setter
    def hl(self, val):
        self.h = (val >> 8) & 0xFF
        self.l = val & 0xFF

    @property
    def bc(self):
        return (self.b << 8) | self.c

    @bc.setter
    def bc(self, val):
        self.b = (val >> 8) & 0xFF
        self.c = val & 0xFF

    @property
    def de(self):
        return (self.d << 8) | self.e

    @de.setter
    def de(self, val):
        self.d = (val >> 8) & 0xFF
        self.e = val & 0xFF

    @property
    def af(self):
        return (self.a << 8) | self.f

    @af.setter
    def af(self, val):
        self.a = (val >> 8) & 0xFF
        self.f = val & 0xFF

    def set_flags(self, result, carry=0, subtract=0, half=0, overflow=None):
        result &= 0xFFFF
        low = result & 0xFF
        sign = low & 0x80
        zero = 0x40 if (low == 0) else 0
        pv = 0
        if overflow is None:
            bits = bin(low).count("1")
            pv = 0x04 if (bits % 2 == 0) else 0
        else:
            pv = 0x04 if overflow else 0
        self.f = sign | zero | (0x10 if half else 0) | pv | (0x02 if subtract else 0) | (1 if carry else 0)

    def reg8(self, code, ix=None):
        if code == 6:
            if ix is None:
                return self.rb(self.hl)
            return self.rb(ix)
        return [self.b, self.c, self.d, self.e, self.h, self.l, 0, self.a][code]

    def set_reg8(self, code, val, ix=None):
        val &= 0xFF
        if code == 6:
            if ix is None:
                self.wb(self.hl, val)
            else:
                self.wb(ix, val)
            return
        if code == 0: self.b = val
        elif code == 1: self.c = val
        elif code == 2: self.d = val
        elif code == 3: self.e = val
        elif code == 4: self.h = val
        elif code == 5: self.l = val
        elif code == 7: self.a = val

    def pair(self, code):
        return [self.bc, self.de, self.hl, self.sp][code]

    def set_pair(self, code, val):
        val &= 0xFFFF
        if code == 0: self.bc = val
        elif code == 1: self.de = val
        elif code == 2: self.hl = val
        else: self.sp = val

    def alu(self, op, value):
        a = self.a
        value &= 0xFF
        if op == 0:  # ADD
            result = a + value
            self.set_flags(result, result > 0xFF, 0, ((a & 0xF) + (value & 0xF)) > 0xF,
                           ((a ^ result) & (value ^ result) & 0x80) != 0)
            self.a = result & 0xFF
        elif op == 1:  # ADC
            carry = self.f & 1
            result = a + value + carry
            self.set_flags(result, result > 0xFF, 0, ((a & 0xF) + (value & 0xF) + carry) > 0xF)
            self.a = result & 0xFF
        elif op == 2:  # SUB
            result = a - value
            self.set_flags(result, result < 0, 1, (a & 0xF) < (value & 0xF),
                           ((a ^ value) & (a ^ (result & 0xFF)) & 0x80) != 0)
            self.a = result & 0xFF
        elif op == 3:  # SBC
            carry = self.f & 1
            result = a - value - carry
            self.set_flags(result, result < 0, 1)
            self.a = result & 0xFF
        elif op == 4:  # AND
            self.a = a & value
            self.set_flags(self.a, 0, 0, 1)
        elif op == 5:  # XOR
            self.a = a ^ value
            self.set_flags(self.a, 0, 0, 0)
        elif op == 6:  # OR
            self.a = a | value
            self.set_flags(self.a, 0, 0, 0)
        elif op == 7:  # CP
            result = a - value
            self.set_flags(result, result < 0, 1, (a & 0xF) < (value & 0xF))

    def step(self):
        if self.pc == 0xFE20:
            self.halted = True
            return
        if self.pc == 0xFE40:
            self.out.append(self.c & 0xFF)
            self.pc = self.pop()
            return
        if self.pc == 0xFD00:
            # BDOS. C=2 console out, C=9 print string, C=1 console in.
            if self.c == 2:
                self.out.append(self.e & 0xFF)
            elif self.c == 9:
                addr = self.de
                while self.rb(addr) != ord('$'):
                    self.out.append(self.rb(addr))
                    addr += 1
            elif self.c == 1:
                if self.stdin is None:
                    if sys.stdin.isatty():
                        self.stdin = b""
                    else:
                        self.stdin = sys.stdin.buffer.read()
                    self.stdin_i = 0
                if self.stdin_i >= len(self.stdin):
                    self.a = 0x1A
                else:
                    self.a = self.stdin[self.stdin_i]
                    self.stdin_i += 1
            self.pc = self.pop()
            return

        op = self.fetch()
        if op == 0x00:
            return
        if op == 0x76:
            self.halted = True
            return
        if op in (0xDD, 0xFD):
            self.prefix(self.ix if op == 0xDD else self.iy, op == 0xDD)
            return
        if op == 0xCB:
            self.cb(None)
            return
        if op == 0xED:
            self.ed()
            return

        x = op >> 6
        y = (op >> 3) & 7
        z = op & 7
        if x == 0:
            if z == 0 and y == 1:  # LD (nn),HL? no, that's 22. y==1 z==0 is EX AF,AF' if y==1? 
                pass
            if op == 0x08:
                self.a, self.ap = self.ap, self.a
                self.f, self.fp = getattr(self, "fp", 0), self.f
                return
            if op == 0x10:  # DJNZ
                disp = self.fetch()
                if disp & 0x80:
                    disp -= 256
                self.b = (self.b - 1) & 0xFF
                if self.b != 0:
                    self.pc = (self.pc + disp) & 0xFFFF
                return
            if op in (0x18, 0x20, 0x28, 0x30, 0x38):
                disp = self.fetch()
                if disp & 0x80:
                    disp -= 256
                take = {0x18: True, 0x20: (self.f & 0x40) == 0, 0x28: (self.f & 0x40) != 0,
                        0x30: (self.f & 1) == 0, 0x38: (self.f & 1) != 0}[op]
                if take:
                    self.pc = (self.pc + disp) & 0xFFFF
                return
            if op == 0x02:  # LD (BC),A
                self.wb(self.bc, self.a); return
            if op == 0x0A:  # LD A,(BC)
                self.a = self.rb(self.bc); return
            if op == 0x12:
                self.wb(self.de, self.a); return
            if op == 0x1A:
                self.a = self.rb(self.de); return
            if op == 0x22:
                self.ww(self.fetchw(), self.hl); return
            if op == 0x2A:
                self.hl = self.rw(self.fetchw()); return
            if op == 0x32:
                self.wb(self.fetchw(), self.a); return
            if op == 0x3A:
                self.a = self.rb(self.fetchw()); return
            if z == 1 and (y % 2) == 0:  # LD dd,nn
                self.set_pair(y // 2, self.fetchw()); return
            if z == 1 and (y % 2) == 1:  # ADD HL,ss
                ss = self.pair(y // 2)
                result = self.hl + ss
                carry = 1 if result > 0xFFFF else 0
                self.hl = result
                self.f = (self.f & 0xC4) | carry
                return
            if z == 3 and (y % 2) == 0:  # INC ss
                code = y // 2
                self.set_pair(code, self.pair(code) + 1); return
            if z == 3 and (y % 2) == 1:  # DEC ss
                code = y // 2
                self.set_pair(code, self.pair(code) - 1); return
            if z == 4:  # INC r
                val = (self.reg8(y) + 1) & 0xFF
                self.set_flags(val, self.f & 1, 0, (self.reg8(y) & 0xF) == 0xF, self.reg8(y) == 0x7F)
                self.set_reg8(y, val); return
            if z == 5:  # DEC r
                old = self.reg8(y)
                val = (old - 1) & 0xFF
                self.set_flags(val, self.f & 1, 1, (old & 0xF) == 0, old == 0x80)
                self.set_reg8(y, val); return
            if z == 6:  # LD r,n
                self.set_reg8(y, self.fetch()); return
            if z == 2 and y >= 4:  # JP cc is x==3. RLCA etc
                pass
            if op == 0x07:  # RLCA
                carry = (self.a >> 7) & 1
                self.a = ((self.a << 1) | carry) & 0xFF
                self.f = (self.f & 0xC4) | carry
                return
            if op == 0x0F:  # RRCA
                carry = self.a & 1
                self.a = ((self.a >> 1) | (carry << 7)) & 0xFF
                self.f = (self.f & 0xC4) | carry
                return
            if op == 0x17:  # RLA
                carry = (self.a >> 7) & 1
                self.a = ((self.a << 1) | (self.f & 1)) & 0xFF
                self.f = (self.f & 0xC4) | carry
                return
            if op == 0x1F:  # RRA
                carry = self.a & 1
                self.a = ((self.a >> 1) | ((self.f & 1) << 7)) & 0xFF
                self.f = (self.f & 0xC4) | carry
                return
            if op == 0x27:
                return  # DAA ignored
            if op == 0x2F:  # CPL
                self.a ^= 0xFF
                self.f |= 0x12
                return
            if op == 0x37:  # SCF
                self.f = (self.f & 0xC4) | 1
                return
            if op == 0x3F:  # CCF
                self.f = (self.f & 0xC4) | ((0 if self.f & 1 else 1))
                return
        elif x == 1:
            if op == 0x76:
                self.halted = True
                return
            self.set_reg8(y, self.reg8(z))
            return
        elif x == 2:
            self.alu(y, self.reg8(z))
            return
        elif x == 3:
            if z == 0:  # RET cc
                take = self.cond(y)
                if take:
                    self.pc = self.pop()
                return
            if z == 1 and (y % 2) == 0:  # POP
                val = self.pop()
                code = y // 2
                if code == 3: self.af = val
                else: self.set_pair(code, val)
                return
            if z == 1 and y == 1:  # RET
                self.pc = self.pop(); return
            if z == 1 and y == 3:  # EXX
                self.b, self.bp = self.bp, self.b
                self.c, self.cp = self.cp, self.c
                self.d, self.dp = self.dp, self.d
                self.e, self.ep = self.ep, self.e
                self.h, self.hp = self.hp, self.h
                self.l, self.lp = self.lp, self.l
                return
            if z == 1 and y == 5:  # JP HL
                self.pc = self.hl; return
            if z == 1 and y == 7:  # LD SP,HL
                self.sp = self.hl; return
            if z == 2:  # JP cc
                addr = self.fetchw()
                if self.cond(y):
                    self.pc = addr
                return
            if op == 0xC3:
                self.pc = self.fetchw(); return
            if z == 4:  # CALL cc
                addr = self.fetchw()
                if self.cond(y):
                    self.push(self.pc)
                    self.pc = addr
                return
            if z == 5 and (y % 2) == 0:  # PUSH
                code = y // 2
                self.push(self.af if code == 3 else self.pair(code))
                return
            if op == 0xCD:
                addr = self.fetchw()
                self.push(self.pc)
                self.pc = addr
                return
            if z == 6:  # ALU n
                self.alu(y, self.fetch()); return
            if z == 7:  # RST
                self.push(self.pc)
                self.pc = y * 8
                return
            if op == 0xE3:  # EX (SP),HL
                val = self.rw(self.sp)
                self.ww(self.sp, self.hl)
                self.hl = val
                return
            if op == 0xE9:
                self.pc = self.hl; return
            if op == 0xEB:  # EX DE,HL
                self.de, self.hl = self.hl, self.de
                return
            if op == 0xF3:
                self.iff = False; return
            if op == 0xFB:
                self.iff = True; return
            if op == 0xF9:
                self.sp = self.hl; return
        raise RuntimeError("unimplemented opcode %02X at %04X" % (op, (self.pc - 1) & 0xFFFF))

    def cond(self, y):
        z = self.f & 0x40
        c = self.f & 1
        return [z == 0, z != 0, c == 0, c != 0, True, True, True, True][y]

    def cb(self, addr):
        op = self.fetch()
        x = op >> 6
        y = (op >> 3) & 7
        z = op & 7
        if addr is None and z == 6:
            val = self.rb(self.hl)
        elif addr is not None:
            val = self.rb(addr)
        else:
            val = self.reg8(z)
        if x == 0:
            if y == 0:  # RLC
                carry = (val >> 7) & 1
                val = ((val << 1) | carry) & 0xFF
            elif y == 1:  # RRC
                carry = val & 1
                val = ((val >> 1) | (carry << 7)) & 0xFF
            elif y == 2:  # RL
                carry = (val >> 7) & 1
                val = ((val << 1) | (self.f & 1)) & 0xFF
            elif y == 3:  # RR
                carry = val & 1
                val = ((val >> 1) | ((self.f & 1) << 7)) & 0xFF
            elif y == 4:  # SLA
                carry = (val >> 7) & 1
                val = (val << 1) & 0xFF
            elif y == 5:  # SRA
                carry = val & 1
                val = (val >> 1) | (val & 0x80)
            elif y == 6:  # SLL
                carry = (val >> 7) & 1
                val = ((val << 1) | 1) & 0xFF
            else:  # SRL
                carry = val & 1
                val = val >> 1
            self.set_flags(val, carry, 0)
        elif x == 1:  # BIT
            bit = (val >> y) & 1
            self.f = (self.f & 1) | (0x40 if bit == 0 else 0) | ((val & 0x80) if y == 7 else 0) | 0x10
            return
        elif x == 2:  # RES
            val &= ~(1 << y)
        else:  # SET
            val |= 1 << y
        if addr is None:
            self.set_reg8(z, val)
        else:
            self.wb(addr, val)

    def ed(self):
        op = self.fetch()
        if op == 0x42 or op == 0x52 or op == 0x62 or op == 0x72:  # SBC HL,ss
            code = (op >> 4) & 3
            ss = self.pair(code)
            carry = self.f & 1
            result = self.hl - ss - carry
            self.set_flags(result, result < 0, 1)
            # set_flags only looks at low byte zero. Fix zero for 16-bit.
            if (result & 0xFFFF) == 0:
                self.f |= 0x40
            else:
                self.f &= ~0x40
            if result & 0x8000:
                self.f |= 0x80
            else:
                self.f &= ~0x80
            self.hl = result
            return
        if op == 0x43 or op == 0x53 or op == 0x63 or op == 0x73:
            self.ww(self.fetchw(), self.pair((op >> 4) & 3)); return
        if op == 0x4B or op == 0x5B or op == 0x6B or op == 0x7B:
            self.set_pair((op >> 4) & 3, self.rw(self.fetchw())); return
        if op == 0x44 or op == 0x4C or op == 0x54 or op == 0x5C or op == 0x64 or op == 0x6C or op == 0x74 or op == 0x7C:
            result = -self.a
            self.set_flags(result, result < 0, 1, (self.a & 0xF) != 0)
            self.a = result & 0xFF
            return
        if op == 0xB0:  # LDIR
            while True:
                self.wb(self.de, self.rb(self.hl))
                self.hl = (self.hl + 1) & 0xFFFF
                self.de = (self.de + 1) & 0xFFFF
                self.bc = (self.bc - 1) & 0xFFFF
                if self.bc == 0:
                    break
            return
        if op == 0xB1:  # CPIR
            while True:
                val = self.rb(self.hl)
                self.alu(7, val)
                self.hl = (self.hl + 1) & 0xFFFF
                self.bc = (self.bc - 1) & 0xFFFF
                if self.bc == 0 or (self.f & 0x40):
                    break
            return
        if op == 0x57:  # LD A,I
            self.a = self.i
            return
        raise RuntimeError("unimplemented ED %02X at %04X" % (op, (self.pc - 2) & 0xFFFF))

    def prefix(self, index, is_ix):
        op = self.fetch()
        disp = 0
        addr = None
        if op in (0x34, 0x35, 0x36, 0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E,
                  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77, 0x86, 0x8E, 0x96, 0x9E,
                  0xA6, 0xAE, 0xB6, 0xBE) or op == 0xCB:
            disp = self.fetch()
            if disp & 0x80:
                disp -= 256
            addr = (index + disp) & 0xFFFF
        if op == 0xCB:
            self.cb(addr)
            return
        if op == 0x21:
            index = self.fetchw()
        elif op == 0x22:
            self.ww(self.fetchw(), index)
        elif op == 0x2A:
            index = self.rw(self.fetchw())
        elif op == 0x23:
            index = (index + 1) & 0xFFFF
        elif op == 0x2B:
            index = (index - 1) & 0xFFFF
        elif op == 0x36:
            self.wb(addr, self.fetch())
        elif op == 0x34:
            val = (self.rb(addr) + 1) & 0xFF
            self.wb(addr, val)
        elif op == 0x35:
            val = (self.rb(addr) - 1) & 0xFF
            self.wb(addr, val)
        elif op == 0xE5:
            self.push(index)
        elif op == 0xE1:
            index = self.pop()
        elif op == 0xE9:
            self.pc = index
        elif op == 0xF9:
            self.sp = index
        elif (op & 0xC7) == 0x46:  # LD r,(ix+d)
            self.set_reg8((op >> 3) & 7, self.rb(addr))
        elif (op & 0xF8) == 0x70 and op != 0x76:  # LD (ix+d),r
            self.wb(addr, self.reg8(op & 7))
        else:
            # Fall back: treat like a normal opcode that uses HL by substituting.
            raise RuntimeError("unimplemented prefix %02X at %04X" % (op, (self.pc - 2) & 0xFFFF))
        if is_ix:
            self.ix = index & 0xFFFF
        else:
            self.iy = index & 0xFFFF

    def run(self, steps=5000000):
        for _ in range(steps):
            if self.halted:
                break
            self.step()
        else:
            sys.stderr.write("step limit pc=%04X\n" % self.pc)
        return bytes(self.out)


def main():
    image = open(sys.argv[1], "rb").read()
    cpu = Z80(image)
    sys.stdout.buffer.write(cpu.run())


if __name__ == "__main__":
    main()
