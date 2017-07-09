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
L1:
    movl %ebp, %edi
    addl $-16, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl $0, %ecx
    pushl %ecx
    movl $10, %esi
    pushl %esi
    call initArray
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, (%edi)
    movl -16(%ebp), %edi
    movl %edi, %eax
    jmp L0
L0:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

.data

