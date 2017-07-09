.globl tigermain

.text

# PROCEDURE tigermain

tigermain:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    subl $100, %esp
L3:
    movl %ebp, %edi
    addl $-16, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl $8, %esi
    pushl %esi
    call allocRecord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl $1000, 4(%esi)
    movl $L0, %ecx
    movl %ecx, 0(%esi)
    movl %esi, (%edi)
    movl $L1, %edx
    movl -16(%ebp), %edi
    movl $0, %esi
    movl $4, %ecx
    # movl %esi, %esi
    imul %ecx, %esi
    # movl %edi, %edi
    addl %esi, %edi
    movl %edx, (%edi)
    movl -16(%ebp), %edi
    movl %edi, %eax
    jmp L2
L2:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

.data

L1:
    .long 0x8
    .ascii "Somebody\0\0\0"

L0:
    .long 0x6
    .ascii "Nobody\0\0\0"

