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
L5:
    pushl %edx
    pushl %ecx
    movl $10, %ecx
    pushl %ecx
    movl %ebp, %edi
    addl $8, %edi
    pushl %edi
    call L0
    addl $8, %esp
    popl %ecx
    popl %edx
    # movl %eax, %eax
    jmp L4
L4:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

# PROCEDURE L0

L0:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    subl $100, %esp
L7:
    movl 12(%ebp), %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    je L1
    jmp L2
L2:
    movl 12(%ebp), %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl 12(%ebp), %esi
    # movl %esi, %esi
    subl $1, %esi
    pushl %esi
    movl 8(%ebp), %ebx
    pushl %ebx
    call L0
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %ecx
    # movl %edi, %edi
    imul %ecx, %edi
    # movl %edi, %edi
L3:
    movl %edi, %eax
    jmp L6
L1:
    movl $1, %edi
    # movl %edi, %edi
    jmp L3
L6:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

.data

