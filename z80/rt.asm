; Shared Z80 runtime. IX points at temp. print_ch is the console primitive.

print_ch:
    inc   (ix+column-temp)
print_chn:
    push  bc
    push  hl
    ld    c,a
    call  conout
    pop   hl
    pop   bc
    ret

print_newline:
    ld    a,13
    call  print_ch
    ld    a,10
    call  print_ch
    xor   a
    ld    (column),a
    ret

print_zstr:
    ld    a,(hl)
    or    a
    ret   z
    push  hl
    call  print_ch
    pop   hl
    inc   hl
    jr    print_zstr

print_tab:
    ld    a,(column)
    ld    b,(tabwid)
print_tab1:
    cp    b
    jr    c,print_tab2
    sub   b
    jr    print_tab1
print_tab2:
    ld    b,a
    ld    a,(tabwid)
    sub   b
    ld    b,a
    ld    a,' '
print_tab3:
    call  print_ch
    djnz  print_tab3
    ret

; print int16 in HL with a leading sign column and a trailing space
print_i16:
    ld    a,' '
    bit   7,h
    jr    z,print_i16p
    ld    de,0
    ex    de,hl
    xor   a
    sbc   hl,de
    ld    a,'-'
print_i16p:
    call  print_ch
print_i16n:
    ld    a,h
    or    a
    jr    nz,print_i16q
    ld    a,l
    cp    10
    jr    c,print_i16r
print_i16q:
    call  div10
    push  af
    call  print_i16n
    pop   af
print_i16r:
    add   a,'0'
    jp    print_ch

print_i16s:
    call  print_i16
    ld    a,' '
    jp    print_ch

; HL / 10. HL = quotient, A = remainder
div10:
    ld    bc,0x0d0a
    xor   a
    add   hl,hl
    rla
    add   hl,hl
    rla
    add   hl,hl
    rla
div10_1:
    add   hl,hl
    rla
    cp    c
    jr    c,div10_2
    sub   c
    inc   l
div10_2:
    djnz  div10_1
    ret

; Signed compare HL ? DE. Returns HL = -1, 0 or 1.
icmp:
    ld    a,h
    xor   d
    jp    m,icmp_diff
    or    a
    sbc   hl,de
    jp    z,icmp_eq
    jp    c,icmp_lt
    ld    hl,1
    ret
icmp_eq:
    ld    hl,0
    ret
icmp_lt:
    ld    hl,-1
    ret
icmp_diff:
    bit   7,h
    jr    nz,icmp_lt
    ld    hl,1
    ret

; Z set when facc is +0 or -0.
f_nz:
    ld    a,(facc)
    or    a
    ret   nz
    ld    a,(facc+1)
    or    a
    ret   nz
    ld    a,(facc+2)
    or    a
    ret   nz
    ld    a,(facc+3)
    and   0x7f
    ret

; HL and DE are strings. Null is empty. HL = -1 if equal, else 0.
streq:
    ld    a,h
    or    l
    jr    nz,streq_l
    ld    hl,empty
streq_l:
    ld    a,d
    or    e
    jr    nz,streq_r
    ld    de,empty
streq_r:
    ld    a,(de)
    cp    (hl)
    jr    nz,streq_no
    or    a
    jr    z,streq_yes
    inc   hl
    inc   de
    jr    streq_r
streq_yes:
    ld    hl,-1
    ret
streq_no:
    ld    hl,0
    ret

; Copy HL to DE. Null source stores an empty string.
scopy:
    ld    a,h
    or    l
    jr    nz,scopy1
    xor   a
    ld    (de),a
    ret
scopy1:
    ld    a,(hl)
    ld    (de),a
    inc   hl
    inc   de
    or    a
    jr    nz,scopy1
    ret

empty:
    db    0

in_pull:
    ld    hl,(in_ptr)
    ld    a,h
    or    l
    jr    z,in_read
    ld    a,(hl)
    or    a
    ret   nz
in_read:
    ld    hl,in_buf
    ld    b,78
in_rc:
    push  bc
    push  hl
    call  in_get
    pop   hl
    pop   bc
    cp    13
    jr    z,in_cr
    cp    10
    jr    z,in_rend
    cp    26
    jr    z,in_rend
    ld    (hl),a
    inc   hl
    djnz  in_rc
    jr    in_rend
in_cr:
    push  hl
    call  in_get
    pop   hl
    cp    10
    jr    z,in_rend
    ld    (in_unget),a
in_rend:
    ld    (hl),0
    ld    hl,in_buf
    ld    (in_ptr),hl
    ret
in_sp:
    ld    hl,(in_ptr)
in_sp2:
    ld    a,(hl)
    cp    32
    jr    nz,in_sp3
    inc   hl
    jr    in_sp2
in_sp3:
    ld    (in_ptr),hl
    ret
in_parse:
    ld    hl,0
in_pl:
    push  hl
    ld    hl,(in_ptr)
    ld    a,(hl)
    pop   hl
    cp    '0'
    ret   c
    cp    '9'+1
    ret   nc
    sub   '0'
    push  af
    add   hl,hl
    ld    d,h
    ld    e,l
    add   hl,hl
    add   hl,hl
    add   hl,de
    pop   af
    ld    e,a
    ld    d,0
    add   hl,de
    push  hl
    ld    hl,(in_ptr)
    inc   hl
    ld    (in_ptr),hl
    pop   hl
    jr    in_pl
in_skip:
    ld    hl,(in_ptr)
in_sk2:
    ld    a,(hl)
    or    a
    jr    z,in_sk3
    cp    ','
    jr    z,in_skc
    inc   hl
    jr    in_sk2
in_skc:
    inc   hl
in_sk3:
    ld    (in_ptr),hl
    ret
input_num:
    call  in_pull
    call  in_sp
    ld    hl,(in_ptr)
    ld    a,(hl)
    cp    '-'
    jr    nz,in_npos
    inc   hl
    ld    (in_ptr),hl
    call  in_parse
    xor   a
    sub   l
    ld    l,a
    ld    a,0
    sbc   a,h
    ld    h,a
    jr    in_nd
in_npos:
    call  in_parse
in_nd:
    push  hl
    call  in_skip
    pop   hl
    xor   a
    ld    (sf_i2f_mode),a
    call  i2f
    ret
input_str:
    call  in_pull
    ld    hl,(in_ptr)
in_sc:
    ld    a,(hl)
    or    a
    jr    z,in_se
    cp    ','
    jr    z,in_se
    ld    (de),a
    inc   hl
    inc   de
    jr    in_sc
in_se:
    xor   a
    ld    (de),a
    ld    (in_ptr),hl
    ld    a,(hl)
    cp    ','
    ret   nz
    inc   hl
    ld    (in_ptr),hl
    ret
line_in:
    call  in_pull
    ld    hl,(in_ptr)
lin_c:
    ld    a,(hl)
    ld    (de),a
    or    a
    jr    z,lin_e
    inc   hl
    inc   de
    jr    lin_c
lin_e:
    ld    (in_ptr),hl
    ret
in_get:
    ld    a,(in_unget)
    or    a
    jr    z,in_getb
    push  af
    xor   a
    ld    (in_unget),a
    pop   af
    ret
in_getb:
    ld    c,1
    call  5
    ret
in_ptr:
    dw    0
in_unget:
    db    0
in_buf:
    ds    80
