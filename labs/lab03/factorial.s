.data
n: .word 8

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial
    
    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result
    
    addi a0, x0, 10
    ecall # Exit

factorial:
    addi sp, sp, -8
    sw ra, 4(sp)
    sw a0, 0(sp)
    
    beq a0, x0, end
    addi a0, a0, -1
    jal ra factorial
    lw t0, 0(sp)
    mul a0, a0, t0
    
    lw ra, 4(sp)
    addi sp, sp, 8
    jr ra
    
end:
    addi a0, x0, 1
    lw ra, 4(sp)
    addi sp, sp, 8
    jr ra
