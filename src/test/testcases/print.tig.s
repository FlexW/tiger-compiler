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
L2:
    pushl %edx
    pushl %ecx
    movl $L0, %edi
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    # movl %eax, %eax
    jmp L1
L1:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

.data

L0:
    .long 0xc
    .ascii "Hello World\n\0\0\0"

