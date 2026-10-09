; IEEE-754 binary32 helpers. facc and farg are little-endian.
; fadd:    facc = farg + facc
; fsub:    facc = farg - facc
; fneg:    facc = -facc
; fload:   facc = (HL)
; fstore:  (HL) = facc
; fpush:   push facc
; fpop:    farg = pop
; fcmp:    HL = -1/0/1 for farg ? facc (lhs ? rhs)
; print_f: print facc with printf "% .7g " via print_ch

fload:
    ld    de,facc
    ld    bc,4
    ldir
    ret

fstore:
    push  hl
    pop   de
    ld    hl,facc
    ld    bc,4
    ldir
    ret

fpush:
    ld    hl,(sf_sp)
    ex    de,hl
    ld    hl,facc
    ld    bc,4
    ldir
    ld    (sf_sp),de
    ret

fpop:
    ld    hl,(sf_sp)
    dec   hl
    dec   hl
    dec   hl
    dec   hl
    ld    (sf_sp),hl
    ld    de,farg
    ld    bc,4
    ldir
    ret

fneg:
    ld    a,(facc+3)
    xor   0x80
    ld    (facc+3),a
    ret

fsub:
    call  fneg
    jp    fadd

; CF set when |facc| < |farg|
sf_magless:
    ld    a,(facc+3)
    and   0x7f
    ld    c,a
    ld    a,(farg+3)
    and   0x7f
    cp    c
    jr    nz,sf_magcc
    ld    a,(farg+2)
    ld    c,a
    ld    a,(facc+2)
    cp    c
    jr    nz,sf_magcc2
    ld    a,(farg+1)
    ld    c,a
    ld    a,(facc+1)
    cp    c
    jr    nz,sf_magcc2
    ld    a,(farg+0)
    ld    c,a
    ld    a,(facc+0)
    cp    c
sf_magcc2:
    ; CF if |facc| < |farg| already (A is facc, C is farg, CP sets CF when A<C)
    ret
sf_magcc:
    ; A is |farg| msb, C is |facc| msb. CF if |farg| < |facc|. Invert.
    ccf
    ret

sf_swap:
    ld    hl,facc
    ld    de,farg
    ld    b,4
sf_swap1:
    ld    c,(hl)
    ld    a,(de)
    ld    (hl),a
    ld    a,c
    ld    (de),a
    inc   hl
    inc   de
    djnz  sf_swap1
    ret

sf_zero4:
    ; HL -> 4 zero bytes
    xor   a
    ld    (hl),a
    inc   hl
    ld    (hl),a
    inc   hl
    ld    (hl),a
    inc   hl
    ld    (hl),a
    ret

; HL = low byte. Shift left one, carry in starts clear.
sf_shl1:
    or    a
    ld    b,4
sf_shl1l:
    rl    (hl)
    inc   hl
    djnz  sf_shl1l
    ret

; HL = high byte. Shift right one.
sf_shr1:
    or    a
    ld    b,4
sf_shr1l:
    rr    (hl)
    dec   hl
    djnz  sf_shr1l
    ret

; (HL) += (DE), 4 bytes. Returns with CF = carry out.
sf_add4:
    ld    b,4
    or    a
sf_add4l:
    ld    a,(de)
    adc   a,(hl)
    ld    (hl),a
    inc   hl
    inc   de
    djnz  sf_add4l
    ret

; (HL) -= (DE), 4 bytes.
sf_sub4:
    ld    b,4
    or    a
sf_sub4l:
    ld    a,(de)
    ld    c,a
    ld    a,(hl)
    sbc   a,c
    ld    (hl),a
    inc   hl
    inc   de
    djnz  sf_sub4l
    ret

sf_clr_facc:
    ld    hl,0
    ld    (facc),hl
    ld    (facc+2),hl
    ret

fadd:
    call  sf_magless
    jr    nc,fadd_noswap
    call  sf_swap
fadd_noswap:
    ld    a,(farg+3)
    and   0x7f
    ld    c,a
    ld    a,(farg+2)
    or    c
    ld    c,a
    ld    a,(farg+1)
    or    c
    ld    c,a
    ld    a,(farg+0)
    or    c
    ret   z
    ld    a,(facc+3)
    and   0x7f
    ld    b,a
    ld    a,(facc+3)
    rlca
    and   1
    ld    (sf_sa),a
    ld    a,(farg+3)
    rlca
    and   1
    ld    (sf_sb),a
    ld    a,b
    add   a,a
    ld    c,a
    ld    a,(facc+2)
    rlca
    and   1
    or    c
    ld    (sf_ea),a
    ld    a,(farg+3)
    and   0x7f
    add   a,a
    ld    c,a
    ld    a,(farg+2)
    rlca
    and   1
    or    c
    ld    (sf_eb),a
    ld    a,(sf_ea)
    or    a
    ret   z
    cp    255
    ret   z
    ld    a,(sf_eb)
    or    a
    jp    z,sf_clr_facc
    cp    255
    ret   z
    ; ma = (frac | hidden) << 3
    ld    hl,sf_ma
    call  sf_zero4
    ld    a,(facc+0)
    ld    (sf_ma+0),a
    ld    a,(facc+1)
    ld    (sf_ma+1),a
    ld    a,(facc+2)
    and   0x7f
    ld    (sf_ma+2),a
    ld    a,(sf_ea)
    or    a
    jr    z,fadd_noha
    ld    a,(sf_ma+2)
    or    0x80
    ld    (sf_ma+2),a
fadd_noha:
    ld    hl,sf_ma
    call  sf_shl1
    ld    hl,sf_ma
    call  sf_shl1
    ld    hl,sf_ma
    call  sf_shl1
    ld    hl,sf_mb
    call  sf_zero4
    ld    a,(farg+0)
    ld    (sf_mb+0),a
    ld    a,(farg+1)
    ld    (sf_mb+1),a
    ld    a,(farg+2)
    and   0x7f
    ld    (sf_mb+2),a
    ld    a,(sf_mb+2)
    or    0x80
    ld    (sf_mb+2),a
    ld    hl,sf_mb
    call  sf_shl1
    ld    hl,sf_mb
    call  sf_shl1
    ld    hl,sf_mb
    call  sf_shl1
    ld    a,(sf_ea)
    ld    c,a
    ld    a,(sf_eb)
    ld    b,a
    ld    a,c
    sub   b
    ld    b,a
    cp    32
    jr    c,fadd_dosh
    ld    hl,sf_mb
    call  sf_zero4
    jr    fadd_shdone
fadd_dosh:
    ld    a,b
    or    a
    jr    z,fadd_shdone
fadd_shl:
    push  bc
    ld    hl,sf_mb+3
    call  sf_shr1
    pop   bc
    djnz  fadd_shl
fadd_shdone:
    ld    a,(sf_sa)
    ld    c,a
    ld    a,(sf_sb)
    cp    c
    jr    nz,fadd_diff
    ld    hl,sf_ma
    ld    de,sf_mb
    call  sf_add4
    ld    a,(sf_ma+3)
    bit   3,a
    jr    z,fadd_pack
    ld    hl,sf_ma+3
    call  sf_shr1
    ld    hl,sf_ea
    inc   (hl)
    jr    fadd_pack
fadd_diff:
    ld    hl,sf_ma
    ld    de,sf_mb
    call  sf_sub4
    ld    a,(sf_ma+0)
    or    a
    ld    a,(sf_ma+1)
    or    a
    ; fall through check all bytes
    ld    hl,sf_ma
    ld    a,(hl)
    inc   hl
    or    (hl)
    inc   hl
    or    (hl)
    inc   hl
    or    (hl)
    jp    z,sf_clr_facc
fadd_norm:
    ld    a,(sf_ma+3)
    bit   2,a
    jr    nz,fadd_pack
    ld    hl,sf_ma
    call  sf_shl1
    ld    hl,sf_ea
    dec   (hl)
    jp    z,sf_clr_facc
    jr    fadd_norm
fadd_pack:
    ld    a,(sf_ma+0)
    and   7
    ld    (sf_rnd),a
    ld    hl,sf_ma+3
    call  sf_shr1
    ld    hl,sf_ma+3
    call  sf_shr1
    ld    hl,sf_ma+3
    call  sf_shr1
    ld    a,(sf_rnd)
    cp    4
    jr    c,fadd_nornd
    jr    nz,fadd_dornd
    ld    a,(sf_ma+0)
    and   1
    jr    z,fadd_nornd
fadd_dornd:
    ld    hl,sf_ma
    ld    a,(hl)
    add   a,1
    ld    (hl),a
    ld    b,3
fadd_rinc:
    jr    nc,fadd_nornd
    inc   hl
    ld    a,(hl)
    adc   a,0
    ld    (hl),a
    djnz  fadd_rinc
fadd_nornd:
    ld    a,(sf_ma+3)
    and   1
    jr    z,fadd_noov
    ld    hl,sf_ma+3
    call  sf_shr1
    ld    hl,sf_ea
    inc   (hl)
fadd_noov:
    ld    a,(sf_ea)
    cp    255
    jr    c,fadd_finite
    ld    a,(sf_sa)
    rrca
    or    0x7f
    ld    (facc+3),a
    ld    a,0x80
    ld    (facc+2),a
    xor   a
    ld    (facc+1),a
    ld    (facc+0),a
    ret
fadd_finite:
    ld    a,(sf_ea)
    or    a
    jp    z,sf_clr_facc
    ; pack
    ld    a,(sf_ma+0)
    ld    (facc+0),a
    ld    a,(sf_ma+1)
    ld    (facc+1),a
    ld    a,(sf_ea)
    rrca
    and   0x80
    ld    c,a
    ld    a,(sf_ma+2)
    and   0x7f
    or    c
    ld    (facc+2),a
    ld    a,(sf_ea)
    srl   a
    ld    c,a
    ld    a,(sf_sa)
    rrca
    or    c
    ld    (facc+3),a
    ret

; A = 0 equal, 1 if |facc| > |farg|, 2 if |facc| < |farg|
sf_magcmp:
    ld    hl,facc+3
    ld    de,farg+3
    ld    a,(hl)
    and   0x7f
    ld    c,a
    ld    a,(de)
    and   0x7f
    cp    c
    jr    nz,sf_mset
    ld    b,3
sf_mnext:
    dec   hl
    dec   de
    ld    a,(de)
    cp    (hl)
    jr    nz,sf_mset
    djnz  sf_mnext
    xor   a
    ret
sf_mset:
    jr    c,sf_fbig
    ld    a,2
    ret
sf_fbig:
    ld    a,1
    ret

; HL = -1 if farg < facc, 0 if equal, 1 if farg > facc
fcmp:
    call  sf_magcmp
    or    a
    jr    z,fcmp_eq
    ld    c,a
    ld    a,(facc+3)
    ld    b,a
    ld    a,(farg+3)
    xor   b
    bit   7,a
    jr    nz,fcmp_ds
    ld    a,b
    rlca
    and   1
    ld    b,a
    ld    a,c
    cp    1
    jr    z,fcmp_fb
    ld    a,b
    or    a
    jr    nz,fcmp_m1
    ld    hl,1
    ret
fcmp_fb:
    ld    a,b
    or    a
    jr    z,fcmp_m1
    ld    hl,1
    ret
fcmp_ds:
    ld    a,(farg+3)
    rlca
    jr    c,fcmp_m1
    ld    hl,1
    ret
fcmp_m1:
    ld    hl,-1
    ret
fcmp_eq:
    ld    hl,0
    ret

; ---- 8-byte helpers. HL and DE point at the low byte. ----

sf_clr8:
    xor   a
    ld    b,8
sf_clr8l:
    ld    (hl),a
    inc   hl
    djnz  sf_clr8l
    ret

sf_cpy8:
    ; (DE) = (HL)
    ld    b,8
sf_cpy8l:
    ld    a,(hl)
    ld    (de),a
    inc   hl
    inc   de
    djnz  sf_cpy8l
    ret

sf_shl8:
    or    a
    ld    hl,sf_n
    ld    b,8
sf_shl8l:
    rl    (hl)
    inc   hl
    djnz  sf_shl8l
    ret

sf_cmp8:
    ; CF if (HL) < (DE), Z if equal.
    push  bc
    push  hl
    push  de
    ld    bc,7
    add   hl,bc
    ex    de,hl
    add   hl,bc
    ex    de,hl
    ld    b,8
sf_cmp8l:
    ld    a,(de)
    ld    c,a
    ld    a,(hl)
    cp    c
    jr    nz,sf_cmp8d
    dec   hl
    dec   de
    djnz  sf_cmp8l
sf_cmp8d:
    pop   de
    pop   hl
    pop   bc
    ret

sf_mul10:
    ; sf_n *= 10
    ld    hl,sf_n
    ld    de,sf_t
    call  sf_cpy8
    call  sf_shl8
    ld    hl,sf_n
    ld    de,sf_u
    call  sf_cpy8
    call  sf_shl8
    call  sf_shl8
    ld    hl,sf_n
    ld    de,sf_u
    call  sf_add8
    ret

sf_add8:
    ; (HL) += (DE)
    ld    b,8
    or    a
sf_add8l:
    ld    a,(de)
    adc   a,(hl)
    ld    (hl),a
    inc   hl
    inc   de
    djnz  sf_add8l
    ret

sf_sub8:
    push  bc
    ld    b,8
    or    a
sf_sub8l:
    ld    a,(de)
    ld    c,a
    ld    a,(hl)
    sbc   a,c
    ld    (hl),a
    inc   hl
    inc   de
    djnz  sf_sub8l
    pop   bc
    ret

; print facc like printf("% .7g ")
print_f:
    ld    a,(facc+3)
    and   0x7f
    add   a,a
    ld    c,a
    ld    a,(facc+2)
    rlca
    and   1
    or    c
    ld    (sf_ea),a
    or    a
    jp    z,print_f0
    cp    255
    jp    z,print_finf
    ld    a,(facc+3)
    rlca
    and   1
    ld    (sf_sign),a
    ld    hl,sf_n
    call  sf_clr8
    ld    a,(facc+0)
    ld    (sf_n+0),a
    ld    a,(facc+1)
    ld    (sf_n+1),a
    ld    a,(facc+2)
    and   0x7f
    or    0x80
    ld    (sf_n+2),a
    ld    hl,sf_den
    call  sf_clr8
    ld    a,1
    ld    (sf_den),a
    ld    a,(sf_ea)
    sub   150
    jr    c,print_fright
    ld    b,a
    or    a
    jr    z,print_fscaled
print_fl:
    push  bc
    call  sf_shl8
    pop   bc
    djnz  print_fl
    jr    print_fscaled
print_fright:
    neg
    ld    b,a
print_frl:
    push  bc
    call  sf_den_shl
    pop   bc
    djnz  print_frl
print_fscaled:
    xor   a
    ld    (sf_e),a
print_fgrow:
    ld    hl,sf_den
    ld    de,sf_t
    call  sf_cpy8
    ld    hl,sf_n
    ld    de,sf_u
    call  sf_cpy8
    ld    hl,sf_t
    push  hl
    pop   de
    ; den*10 into sf_t via the mul routine's pointer. Use sf_n as scratch carefully.
    ld    hl,sf_n
    ld    de,sf_v
    call  sf_cpy8
    ld    hl,sf_t
    ld    de,sf_n
    call  sf_cpy8
    call  sf_mul10
    ld    hl,sf_n
    ld    de,sf_v
    call  sf_cmp8
    jr    c,print_fgrow_ok
    jr    z,print_fgrow_ok
    jr    print_fgrow_stop
print_fgrow_ok:
    ; num >= den*10, keep the *10 denominator
    ld    hl,sf_n
    ld    de,sf_den
    call  sf_cpy8
    ld    hl,sf_v
    ld    de,sf_n
    call  sf_cpy8
    ld    a,(sf_e)
    inc   a
    ld    (sf_e),a
    cp    20
    jr    nz,print_fgrow
print_fgrow_stop:
    ld    hl,sf_v
    ld    de,sf_n
    call  sf_cpy8
print_fshrink:
    ld    hl,sf_n
    ld    de,sf_den
    call  sf_cmp8
    jr    nc,print_fdigits
    ld    a,(sf_e)
    cp    0xec
    jr    z,print_fdigits
    call  sf_mul10
    ld    a,(sf_e)
    dec   a
    ld    (sf_e),a
    jr    print_fshrink
print_fdigits:
    ld    b,6
print_fm6:
    push  bc
    call  sf_mul10
    pop   bc
    djnz  print_fm6
    ; add den/2
    ld    hl,sf_den
    ld    de,sf_t
    call  sf_cpy8
    call  sf_t_shr
    ld    hl,sf_n
    ld    de,sf_t
    call  sf_add8
    call  sf_div
    call  sf_emit
    ret
print_f0:
    ld    a,' '
    call  print_ch
    ld    a,'0'
    call  print_ch
    ld    a,' '
    call  print_ch
    ret
print_finf:
    ld    a,'i'
    call  print_ch
    ld    a,'n'
    call  print_ch
    ld    a,'f'
    call  print_ch
    ret

sf_den_shl:
    or    a
    ld    hl,sf_den
    ld    b,8
sf_den_shll:
    rl    (hl)
    inc   hl
    djnz  sf_den_shll
    ret

sf_t_shr:
    or    a
    ld    hl,sf_t+7
    ld    b,8
sf_t_shrl:
    rr    (hl)
    dec   hl
    djnz  sf_t_shrl
    ret

; sf_n / sf_den -> sf_q
sf_div:
    ld    hl,sf_q
    call  sf_clr8
    ld    hl,sf_r
    call  sf_clr8
    ld    c,64
sf_divl:
    call  sf_r_shl_nbit
    call  sf_q_shl
    ld    hl,sf_r
    ld    de,sf_den
    call  sf_cmp8
    jr    c,sf_div_no
    ld    hl,sf_r
    ld    de,sf_den
    call  sf_sub8
    ld    a,(sf_q)
    or    1
    ld    (sf_q),a
sf_div_no:
    dec   c
    jr    nz,sf_divl
    ret

sf_r_shl_nbit:
    or    a
    ld    hl,sf_n
    ld    b,8
sf_nshl:
    rl    (hl)
    inc   hl
    djnz  sf_nshl
    ld    hl,sf_r
    ld    b,8
sf_rshl:
    rl    (hl)
    inc   hl
    djnz  sf_rshl
    ret

sf_q_shl:
    or    a
    ld    hl,sf_q
    ld    b,8
sf_qshll:
    rl    (hl)
    inc   hl
    djnz  sf_qshll
    ret

sf_q_div10:
    ld    hl,sf_q
    ld    de,sf_n
    call  sf_cpy8
    ld    hl,sf_den
    call  sf_clr8
    ld    a,10
    ld    (sf_den),a
    call  sf_div
    ret

sf_q_digits:
    ld    de,sf_powers
    ld    hl,sf_asc
    ld    b,7
sf_dg:
    push  bc
    push  hl
    ld    c,0
sf_dg2:
    push  de
    ld    hl,sf_q
    call  sf_cmp8
    pop   de
    jr    c,sf_dg3
    push  de
    ld    hl,sf_q
    call  sf_sub8
    pop   de
    inc   c
    jr    sf_dg2
sf_dg3:
    ld    a,c
    add   a,'0'
    pop   hl
    ld    (hl),a
    inc   hl
    push  hl
    ld    hl,8
    add   hl,de
    ex    de,hl
    pop   hl
    pop   bc
    djnz  sf_dg
    ret

sf_emit:
    ld    hl,sf_q
    ld    de,sf_tenm
    call  sf_cmp8
    jr    c,sf_emit_ok
    call  sf_q_div10
    ld    a,(sf_e)
    inc   a
    ld    (sf_e),a
sf_emit_ok:
    call  sf_q_digits
    ld    b,7
    ld    hl,sf_asc+6
sf_strip:
    ld    a,(hl)
    cp    '0'
    jr    nz,sf_stripped
    dec   hl
    djnz  sf_strip
    ld    b,1
sf_stripped:
    ld    a,b
    ld    (sf_len),a
    ld    a,(sf_sign)
    or    a
    jr    z,sf_emit_sp
    ld    a,'-'
    call  print_ch
    jr    sf_emit_body
sf_emit_sp:
    ld    a,' '
    call  print_ch
sf_emit_body:
    ld    a,(sf_e)
    bit   7,a
    jr    nz,sf_frac
    inc   a
    ld    c,a
    ld    a,(sf_len)
    cp    c
    jr    c,sf_intpad
    jr    z,sf_intpad
    ; fraction: print C integer digits, dot, then the rest
    ld    hl,sf_asc
    ld    b,c
sf_il:
    ld    a,(hl)
    call  print_ch
    inc   hl
    djnz  sf_il
    ld    a,'.'
    call  print_ch
    ld    a,(sf_len)
    sub   c
    ld    b,a
sf_fl:
    ld    a,(hl)
    call  print_ch
    inc   hl
    djnz  sf_fl
    jr    sf_emit_tail
sf_intpad:
    ld    hl,sf_asc
    ld    a,(sf_len)
    ld    b,a
sf_ipl:
    ld    a,(hl)
    call  print_ch
    inc   hl
    djnz  sf_ipl
    ld    a,c
    ld    b,a
    ld    a,(sf_len)
    ld    c,a
    ld    a,b
    sub   c
    jr    z,sf_emit_tail
    ld    b,a
sf_pzl:
    ld    a,'0'
    call  print_ch
    djnz  sf_pzl
    jr    sf_emit_tail
sf_frac:
    ld    a,'0'
    call  print_ch
    ld    a,'.'
    call  print_ch
    ld    a,(sf_e)
    neg
    dec   a
    jr    z,sf_fracd
    ld    b,a
sf_fz:
    ld    a,'0'
    call  print_ch
    djnz  sf_fz
sf_fracd:
    ld    hl,sf_asc
    ld    a,(sf_len)
    ld    b,a
sf_fp:
    ld    a,(hl)
    call  print_ch
    inc   hl
    djnz  sf_fp
sf_emit_tail:
    ld    a,' '
    call  print_ch
    ret

; facc = farg * facc
fmul:
    ld    a,(facc+3)
    rlca
    and   1
    ld    c,a
    ld    a,(farg+3)
    rlca
    and   1
    xor   c
    ld    (sf_sign),a
    call  sf_exp_facc
    ld    (sf_ea),a
    or    a
    jp    z,fmul_zero
    cp    255
    ret   z
    call  sf_exp_farg
    ld    (sf_eb),a
    or    a
    jp    z,fmul_zero
    cp    255
    ret   z
    ld    a,(sf_ea)
    ld    c,a
    ld    a,(sf_eb)
    add   a,c
    jr    c,fmul_ebig
    cp    127
    jp    c,fmul_zero
    sub   127
    jp    z,fmul_zero
    ld    (sf_e),a
    jr    fmul_mant
fmul_ebig:
    add   a,129
    jp    c,fmul_inf
    cp    255
    jp    nc,fmul_inf
    ld    (sf_e),a
fmul_mant:
    ld    a,(facc)
    ld    (sf_ma),a
    ld    a,(facc+1)
    ld    (sf_ma+1),a
    ld    a,(facc+2)
    and   0x7f
    or    0x80
    ld    (sf_ma+2),a
    ld    a,(farg)
    ld    (sf_mb),a
    ld    a,(farg+1)
    ld    (sf_mb+1),a
    ld    a,(farg+2)
    and   0x7f
    or    0x80
    ld    (sf_mb+2),a
    ld    hl,sf_prod
    ld    b,8
    xor   a
fmul_clr:
    ld    (hl),a
    inc   hl
    djnz  fmul_clr
    ld    c,0
fmul_i:
    ld    b,0
fmul_j:
    push  bc
    ld    hl,sf_ma
    ld    a,l
    add   a,c
    ld    l,a
    ld    a,h
    adc   a,0
    ld    h,a
    ld    e,(hl)
    ld    hl,sf_mb
    ld    a,l
    add   a,b
    ld    l,a
    ld    a,h
    adc   a,0
    ld    h,a
    ld    a,(hl)
    call  sf_mul8
    pop   bc
    push  bc
    ld    a,b
    add   a,c
    ld    c,a
    call  sf_prodadd
    pop   bc
    inc   b
    ld    a,b
    cp    3
    jr    c,fmul_j
    inc   c
    ld    a,c
    cp    3
    jr    c,fmul_i
    xor   a
    ld    (sf_rnd),a
    ld    (sf_sticky),a
    ld    a,(sf_prod+5)
    bit   7,a
    jr    z,fmul_s23
    ld    a,(sf_e)
    inc   a
    cp    255
    jp    nc,fmul_inf
    ld    (sf_e),a
    ld    b,24
    jr    fmul_sh
fmul_s23:
    ld    b,23
fmul_sh:
    dec   b
fmul_shl:
    push  bc
    call  sf_shr6
    jr    nc,fmul_ns
    ld    a,1
    ld    (sf_sticky),a
fmul_ns:
    pop   bc
    djnz  fmul_shl
    call  sf_shr6
    jr    nc,fmul_nr
    ld    a,1
    ld    (sf_rnd),a
fmul_nr:
    ld    a,(sf_rnd)
    or    a
    jr    z,fmul_pack
    ld    a,(sf_sticky)
    or    a
    jr    nz,fmul_rup
    ld    a,(sf_prod)
    and   1
    jr    z,fmul_pack
fmul_rup:
    ld    hl,sf_prod
    inc   (hl)
    jr    nz,fmul_pack
    inc   hl
    inc   (hl)
    jr    nz,fmul_pack
    inc   hl
    inc   (hl)
    jr    nz,fmul_pack
    xor   a
    ld    (sf_prod),a
    ld    (sf_prod+1),a
    ld    (sf_prod+2),a
    ld    a,(sf_e)
    inc   a
    ld    (sf_e),a
fmul_pack:
    ld    a,(sf_e)
    cp    255
    jp    nc,fmul_inf
    ld    a,(sf_prod)
    ld    (facc),a
    ld    a,(sf_prod+1)
    ld    (facc+1),a
    ld    a,(sf_prod+2)
    and   0x7f
    ld    (facc+2),a
    ld    a,(sf_e)
    and   1
    jr    z,fmul_el
    ld    a,(facc+2)
    or    0x80
    ld    (facc+2),a
fmul_el:
    ld    a,(sf_e)
    srl   a
    ld    c,a
    ld    a,(sf_sign)
    or    a
    jr    z,fmul_es
    ld    a,c
    or    0x80
    ld    c,a
fmul_es:
    ld    a,c
    ld    (facc+3),a
    ret
fmul_zero:
    xor   a
    ld    (facc),a
    ld    (facc+1),a
    ld    (facc+2),a
    ld    a,(sf_sign)
    rrca
    ld    (facc+3),a
    ret
fmul_inf:
    xor   a
    ld    (facc),a
    ld    (facc+1),a
    ld    (facc+2),a
    ld    a,(sf_sign)
    rrca
    or    0x7f
    ld    (facc+3),a
    ret

; A * E -> HL
sf_mul8:
    ld    hl,0
    ld    d,0
    ld    b,8
sf_m8:
    add   hl,hl
    add   a,a
    jr    nc,sf_m8n
    add   hl,de
sf_m8n:
    djnz  sf_m8
    ret

; add HL into sf_prod at offset C
sf_prodadd:
    push  de
    ld    a,c
    ld    de,sf_prod
    add   a,e
    ld    e,a
    ld    a,d
    adc   a,0
    ld    d,a
    ld    a,(de)
    add   a,l
    ld    (de),a
    inc   de
    ld    a,(de)
    adc   a,h
    ld    (de),a
    inc   de
    ld    a,(de)
    adc   a,0
    ld    (de),a
    inc   de
    ld    a,(de)
    adc   a,0
    ld    (de),a
    pop   de
    ret

; shift sf_prod right one. CF = bit shifted out.
sf_shr6:
    ld    hl,sf_prod+5
    or    a
    ld    c,6
sf_shr6l:
    rr    (hl)
    dec   hl
    dec   c
    jr    nz,sf_shr6l
    ret

; A = biased exponent of facc / farg. 0 if the value is zero.
sf_exp_facc:
    ld    a,(facc)
    ld    c,a
    ld    a,(facc+1)
    or    c
    ld    c,a
    ld    a,(facc+2)
    or    c
    ld    c,a
    ld    a,(facc+3)
    and   0x7f
    or    c
    ret   z
    ld    a,(facc+3)
    and   0x7f
    add   a,a
    ld    c,a
    ld    a,(facc+2)
    rlca
    and   1
    or    c
    ret

sf_exp_farg:
    ld    a,(farg)
    ld    c,a
    ld    a,(farg+1)
    or    c
    ld    c,a
    ld    a,(farg+2)
    or    c
    ld    c,a
    ld    a,(farg+3)
    and   0x7f
    or    c
    ret   z
    ld    a,(farg+3)
    and   0x7f
    add   a,a
    ld    c,a
    ld    a,(farg+2)
    rlca
    and   1
    or    c
    ret

; facc = farg / facc
fdiv:
    ld    a,(facc+3)
    rlca
    and   1
    ld    c,a
    ld    a,(farg+3)
    rlca
    and   1
    xor   c
    ld    (sf_sign),a
    call  sf_exp_facc
    ld    (sf_ea),a
    or    a
    jp    z,fmul_inf
    cp    255
    ret   z
    call  sf_exp_farg
    ld    (sf_eb),a
    or    a
    jp    z,fmul_zero
    cp    255
    jp    z,fmul_inf
    ; eb - ea + 127
    ld    a,(sf_eb)
    ld    c,a
    ld    a,(sf_ea)
    ld    b,a
    ld    a,c
    sub   b
    jr    c,fdiv_eborrow
    add   a,127
    jp    c,fmul_inf
    cp    255
    jp    nc,fmul_inf
    ld    (sf_e),a
    jr    fdiv_mant
fdiv_eborrow:
    ; A = eb-ea (negative, two's in A as unsigned wrap). Real delta = A-256.
    ; exp = delta + 127 = A - 129
    sub   129
    jp    c,fmul_zero
    or    a
    jp    z,fmul_zero
    ld    (sf_e),a
fdiv_mant:
    ld    a,(farg)
    ld    (sf_ma),a
    ld    a,(farg+1)
    ld    (sf_ma+1),a
    ld    a,(farg+2)
    and   0x7f
    or    0x80
    ld    (sf_ma+2),a
    xor   a
    ld    (sf_ma+3),a
    ld    a,(facc)
    ld    (sf_mb),a
    ld    a,(facc+1)
    ld    (sf_mb+1),a
    ld    a,(facc+2)
    and   0x7f
    or    0x80
    ld    (sf_mb+2),a
    xor   a
    ld    (sf_mb+3),a
    ld    hl,sf_hold
    call  sf_zero4
    ld    hl,sf_prod
    ld    b,4
    xor   a
fdiv_cl:
    ld    (hl),a
    inc   hl
    djnz  fdiv_cl
    ld    b,47
fdiv_bit:
    push  bc
    ld    hl,sf_hold
    call  sf_shl1
    or    a
    ld    hl,sf_ma
    rl    (hl)
    inc   hl
    rl    (hl)
    inc   hl
    rl    (hl)
    jr    nc,fdiv_nobit
    ld    a,(sf_hold)
    or    1
    ld    (sf_hold),a
fdiv_nobit:
    ld    hl,sf_prod
    call  sf_shl1
    ld    hl,sf_hold
    ld    de,sf_hold2
    ld    bc,4
    ldir
    ld    hl,sf_hold
    ld    de,sf_mb
    call  sf_sub4
    jr    nc,fdiv_took
    ld    hl,sf_hold2
    ld    de,sf_hold
    ld    bc,4
    ldir
    jr    fdiv_nosub
fdiv_took:
    ld    a,(sf_prod)
    or    1
    ld    (sf_prod),a
fdiv_nosub:
    pop   bc
    djnz  fdiv_bit
    ld    a,(sf_prod+2)
    bit   7,a
    jr    nz,fdiv_aligned
    ld    hl,sf_prod
    call  sf_shl1
    ld    a,(sf_e)
    dec   a
    jp    z,fmul_zero
    ld    (sf_e),a
    ld    hl,sf_hold
    call  sf_shl1
    ld    hl,sf_hold
    ld    de,sf_mb
    call  sf_ucmp4
    jr    c,fdiv_aligned
    ld    hl,sf_hold
    ld    de,sf_mb
    call  sf_sub4
    ld    a,(sf_prod)
    or    1
    ld    (sf_prod),a
fdiv_aligned:
    ld    hl,sf_hold
    ld    de,sf_hold2
    ld    bc,4
    ldir
    ld    hl,sf_hold2
    call  sf_shl1
    ld    hl,sf_hold2
    ld    de,sf_mb
    call  sf_ucmp4
    jr    c,fdiv_norm
    jr    nz,fdiv_rup
    ld    a,(sf_prod)
    and   1
    jr    z,fdiv_norm
fdiv_rup:
    ld    hl,sf_prod
    inc   (hl)
    jr    nz,fdiv_norm
    inc   hl
    inc   (hl)
    jr    nz,fdiv_norm
    inc   hl
    inc   (hl)
    jr    nz,fdiv_norm
    xor   a
    ld    (sf_prod),a
    ld    (sf_prod+1),a
    ld    (sf_prod+2),a
    ld    a,(sf_e)
    inc   a
    ld    (sf_e),a
fdiv_norm:
    ld    a,(sf_prod)
    ld    (facc),a
    ld    a,(sf_prod+1)
    ld    (facc+1),a
    ld    a,(sf_prod+2)
    and   0x7f
    ld    (facc+2),a
    ld    a,(sf_e)
    and   1
    jr    z,fdiv_el
    ld    a,(facc+2)
    or    0x80
    ld    (facc+2),a
fdiv_el:
    ld    a,(sf_e)
    srl   a
    ld    c,a
    ld    a,(sf_sign)
    or    a
    jr    z,fdiv_es
    ld    a,c
    or    0x80
    ld    c,a
fdiv_es:
    ld    a,c
    ld    (facc+3),a
    ret

; CF if (HL) > (DE) for 3 bytes? We want CF when ma < mb, i.e. (DE) < (HL) if DE=ma HL=mb.
; Compare 3 bytes high to low. CF set if (DE) < (HL).
sf_cmp3:
    push  bc
    ld    b,3
    ld    a,l
    add   a,2
    ld    l,a
    ld    a,h
    adc   a,0
    ld    h,a
    ld    a,e
    add   a,2
    ld    e,a
    ld    a,d
    adc   a,0
    ld    d,a
sf_cmp3l:
    ld    a,(de)
    cp    (hl)
    jr    nz,sf_cmp3d
    dec   hl
    dec   de
    djnz  sf_cmp3l
    or    a
    pop   bc
    ret
sf_cmp3d:
    pop   bc
    ret

; (HL) ? (DE), 4 bytes from the high byte.
; CF if (HL) < (DE). Z if equal.
sf_ucmp4:
    push  bc
    push  hl
    push  de
    ld    b,4
    ld    a,l
    add   a,3
    ld    l,a
    ld    a,h
    adc   a,0
    ld    h,a
    ld    a,e
    add   a,3
    ld    e,a
    ld    a,d
    adc   a,0
    ld    d,a
sf_ucmp4l:
    ld    a,(de)
    ld    c,a
    ld    a,(hl)
    cp    c
    jr    nz,sf_ucmp4d
    dec   hl
    dec   de
    djnz  sf_ucmp4l
    pop   de
    pop   hl
    pop   bc
    xor   a
    ret
sf_ucmp4d:
    pop   de
    pop   hl
    pop   bc
    ret

; (HL) -= (DE), 3 bytes
sf_sub3:
    push  bc
    ld    b,3
    or    a
sf_sub3l:
    ld    a,(de)
    ld    c,a
    ld    a,(hl)
    sbc   a,c
    ld    (hl),a
    inc   hl
    inc   de
    djnz  sf_sub3l
    pop   bc
    ret

; facc -> HL, trunc toward zero. Saturates at 16-bit limits.
f2i:
    call  sf_exp_facc
    cp    127
    jr    c,f2i_zero
    sub   127
    cp    15
    jr    nc,f2i_sat
    ld    c,a
    ld    a,23
    sub   c
    ld    b,a
    ld    a,(facc)
    ld    (sf_ma),a
    ld    a,(facc+1)
    ld    (sf_ma+1),a
    ld    a,(facc+2)
    and   0x7f
    or    0x80
    ld    (sf_ma+2),a
    xor   a
    ld    (sf_ma+3),a
    ld    a,b
    or    a
    jr    z,f2i_nosh
f2i_sh:
    ld    hl,sf_ma+3
    or    a
    rr    (hl)
    dec   hl
    rr    (hl)
    dec   hl
    rr    (hl)
    dec   hl
    rr    (hl)
    djnz  f2i_sh
f2i_nosh:
    ld    a,(sf_ma)
    ld    l,a
    ld    a,(sf_ma+1)
    ld    h,a
    ld    a,(facc+3)
    bit   7,a
    ret   z
    xor   a
    sub   l
    ld    l,a
    ld    a,0
    sbc   a,h
    ld    h,a
    ret
f2i_zero:
    ld    hl,0
    ret
f2i_sat:
    ld    hl,32767
    ld    a,(facc+3)
    bit   7,a
    ret   z
    ld    hl,0x8000
    ret

; HL (0..32767) -> facc. If C flag set on entry via sf_i2f_div, exponent is reduced by 15.
i2f:
    ld    a,h
    or    l
    jp    z,fmul_zero_pos
    xor   a
    ld    (sf_sign),a
    bit   7,h
    jr    z,i2f_mag
    xor   a
    sub   l
    ld    l,a
    ld    a,0
    sbc   a,h
    ld    h,a
    ld    a,1
    ld    (sf_sign),a
i2f_mag:
    ld    b,0
i2f_n:
    bit   7,h
    jr    nz,i2f_p
    add   hl,hl
    inc   b
    jr    i2f_n
i2f_p:
    ld    a,142
    sub   b
    ld    c,a
    ld    a,(sf_i2f_mode)
    or    a
    jr    z,i2f_pack
    ld    a,c
    sub   15
    ld    c,a
i2f_pack:
    xor   a
    ld    (facc),a
    ld    a,l
    ld    (facc+1),a
    ld    a,h
    and   0x7f
    ld    (facc+2),a
    ld    a,c
    and   1
    jr    z,i2f_e0
    ld    a,(facc+2)
    or    0x80
    ld    (facc+2),a
i2f_e0:
    ld    a,c
    srl   a
    ld    c,a
    ld    a,(sf_sign)
    or    a
    jr    z,i2f_es
    ld    a,c
    or    0x80
    ld    c,a
i2f_es:
    ld    a,c
    ld    (facc+3),a
    ret
fmul_zero_pos:
    xor   a
    ld    (sf_sign),a
    jp    fmul_zero

; facc = rnd(facc). Seed matches the C runtime LCG.
rnd:
    call  sf_exp_facc
    or    a
    jp    z,rnd_make
    ld    a,(facc+3)
    bit   7,a
    jr    z,rnd_step
    call  fneg
    ld    hl,rnd_1000
    ld    de,farg
    ld    bc,4
    ldir
    call  fmul
    call  f2i
    ld    a,l
    or    1
    ld    (rnd_seed),a
    ld    a,h
    ld    (rnd_seed+1),a
    xor   a
    ld    (rnd_seed+2),a
    ld    (rnd_seed+3),a
    jr    rnd_make
rnd_step:
    ld    hl,rnd_acc
    call  sf_zero4
    ld    hl,rnd_seed
    ld    de,rnd_tmp
    ld    bc,4
    ldir
    ld    hl,rnd_k
    ld    (hl),0xfd
    inc   hl
    ld    (hl),0x43
    inc   hl
    ld    (hl),0x03
    inc   hl
    ld    (hl),0x00
    ld    b,32
rnd_lp:
    push  bc
    ld    hl,rnd_k+3
    call  sf_shr1
    jr    nc,rnd_no
    ld    hl,rnd_acc
    ld    de,rnd_tmp
    call  sf_add4
rnd_no:
    ld    hl,rnd_tmp
    call  sf_shl1
    pop   bc
    djnz  rnd_lp
    ld    hl,rnd_acc
    ld    de,rnd_inc
    call  sf_add4
    ld    hl,rnd_acc
    ld    de,rnd_seed
    ld    bc,4
    ldir
rnd_make:
    ld    a,(rnd_seed+2)
    ld    l,a
    ld    a,(rnd_seed+3)
    and   0x7f
    ld    h,a
    ld    a,1
    ld    (sf_i2f_mode),a
    call  i2f
    xor   a
    ld    (sf_i2f_mode),a
    ret

sf_powers:
    db    0x40,0x42,0x0f,0x00,0x00,0x00,0x00,0x00
    db    0xa0,0x86,0x01,0x00,0x00,0x00,0x00,0x00
    db    0x10,0x27,0x00,0x00,0x00,0x00,0x00,0x00
    db    0xe8,0x03,0x00,0x00,0x00,0x00,0x00,0x00
    db    0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    db    0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    db    0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00

sf_tenm:
    db    0x80,0x96,0x98,0x00,0x00,0x00,0x00,0x00

facc:       db 0,0,0,0
farg:       db 0,0,0,0
sf_ma:      db 0,0,0,0
sf_mb:      db 0,0,0,0
sf_prod:    db 0,0,0,0,0,0,0,0
sf_sticky:  db 0
sf_i2f_mode: db 0
sf_hold:    db 0,0,0,0
sf_hold2:   db 0,0,0,0
rnd_seed:   db 0,0,5,0
rnd_acc:    db 0,0,0,0
rnd_tmp:    db 0,0,0,0
rnd_k:      db 0,0,0,0
rnd_inc:    db 0xc3,0x9e,0x26,0x00
rnd_1000:   db 0,0,122,68
sf_ea:      db 0
sf_eb:      db 0
sf_sa:      db 0
sf_sb:      db 0
sf_rnd:     db 0
sf_e:       db 0
sf_sign:    db 0
sf_len:     db 0
sf_asc:     db 0,0,0,0,0,0,0
sf_n:       db 0,0,0,0,0,0,0,0
sf_den:     db 0,0,0,0,0,0,0,0
sf_t:       db 0,0,0,0,0,0,0,0
sf_u:       db 0,0,0,0,0,0,0,0
sf_v:       db 0,0,0,0,0,0,0,0
sf_q:       db 0,0,0,0,0,0,0,0
sf_r:       db 0,0,0,0,0,0,0,0
sf_sp:      dw sf_stk
sf_stk:     db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
