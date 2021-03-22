# Q1a
str_pr:
    addi sp, sp, -8
    sw ra, 0(sp)
    sw s0, 4(sp)
    
    add s0, a0, x0
    
loop:
    lw a0, 0(s0)
    beq a0, x0, done
    jal print
    addi s0, s0, 1
    j loop
    
done:
    lw ra, 0(sp)
    lw s0, 4(sp)
    addi sp, sp, 8
    jr ra

print:
    mv a1, a0
    li a0, 4
    ecall
    jr ra

# Q1b
str_pr:
	addi sp, sp, -16
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    
    add s0, a0, x0
    add s1, a1, x0
    add s2, x0, x0
    
loop:
	beq s2, s1, done
    add a0, s2, s0
	lw a0, 0(a0)
    jal print
    addi s2, s2, 1
    j loop
    
done:
	lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    addi sp, sp, 16
    jr ra
	
print:
	mv a1, a0
	li	a0, 4
	ecall
	jr	ra
    
# Q1c
str_pr:
    addi sp, sp, -8
    sw ra, 0(sp)
    sw s0, 4(sp)
    
    add s0, a0, x0
    lw a0, 0(s0)
    beq a0, x0, done
    
loop:
    jal print
    addi s0, s0, 1
    lw a0, 0(s0)
    bne a0, x0, loop
    
done:
    lw ra, 0(sp)
    lw s0, 4(sp)
    addi sp, sp, 8
    jr ra

print:
    mv a1, a0
    li a0, 4
    ecall
    jr ra

# Q2
fib:
    addi sp, sp, -12
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    
    slti t0, a0, 2
    bne t0, x0, done
    
    add s0, a0, x0
    addi a0, a0, -1
    jal fib
    add s1, a0, x0
    addi a0, s0, -2
    jal fib
    add a0, a0, s1
    
done:
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    addi sp, sp, 12
    jr ra

# Q3
fib:
    addi sp, sp, -16
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    
    slti t0, a0, 2
    bne t0, x0, done
    
    slli t0, a0, 2
    add t0, t0, a1
    add s0, a0, x0
    lw a0, 0(t0)
    bne a0, x0, done
    
    add s2, a1, x0
    addi s0, s0, -1
    jal fib
    add s1, a0, x0
    addi a0, s0, -2
    jal fib
    add a0, a0, s1
    sw a0, 0(s2)
    
done:
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    addi sp, sp, 12
    jr ra
