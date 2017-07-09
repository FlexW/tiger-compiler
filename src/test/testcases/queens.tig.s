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
L35:
    movl $8, -16(%ebp)
    movl %ebp, %edi
    addl $-20, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl $0, %ecx
    pushl %ecx
    movl -16(%ebp), %esi
    pushl %esi
    call initArray
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, (%edi)
    movl %ebp, %edi
    addl $-24, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl $0, %ecx
    pushl %ecx
    movl -16(%ebp), %esi
    pushl %esi
    call initArray
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, (%edi)
    movl %ebp, %ebx
    addl $-28, %ebx
    # movl %ebx, %ebx
    pushl %edx
    pushl %ecx
    movl $0, %edx
    pushl %edx
    movl -16(%ebp), %edi
    movl -16(%ebp), %esi
    # movl %edi, %edi
    addl %esi, %edi
    # movl %edi, %edi
    subl $1, %edi
    pushl %edi
    call initArray
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
    movl %edi, (%ebx)
    movl %ebp, %esi
    addl $-32, %esi
    # movl %esi, %esi
    pushl %edx
    pushl %ecx
    movl $0, %ebx
    pushl %ebx
    movl -16(%ebp), %ecx
    movl -16(%ebp), %edi
    # movl %ecx, %ecx
    addl %edi, %ecx
    # movl %ecx, %ecx
    subl $1, %ecx
    pushl %ecx
    call initArray
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
    movl %edi, (%esi)
    pushl %edx
    pushl %ecx
    movl $0, %ecx
    pushl %ecx
    movl %ebp, %edi
    addl $8, %edi
    pushl %edi
    call L1
    addl $8, %esp
    popl %ecx
    popl %edx
    # movl %eax, %eax
    jmp L34
L34:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

# PROCEDURE L1

L1:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    subl $100, %esp
L37:
    movl 12(%ebp), %ebx
    movl 8(%ebp), %esi
    movl -24(%esi), %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    je L31
    jmp L32
L32:
    movl $0, -16(%ebp)
L29:
    movl 8(%ebp), %edi
    movl -24(%edi), %edi
    # movl %edi, %edi
    subl $1, %edi
    # movl %edi, %edi
    movl -16(%ebp), %ebx
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    jle L30
    jmp L15
L15:
    nop
L33:
    movl $0, %edi
    movl %edi, %eax
    jmp L36
L31:
    pushl %edx
    pushl %ecx
    movl 8(%ebp), %edi
    pushl %edi
    call L0
    addl $4, %esp
    popl %ecx
    popl %edx
    jmp L33
L30:
    movl 8(%ebp), %edi
    movl -28(%edi), %esi
    movl -16(%ebp), %edi
    movl $4, %ebx
    # movl %edi, %edi
    imul %ebx, %edi
    # movl %esi, %esi
    addl %edi, %esi
    movl (%esi), %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    je L16
    jmp L17
L17:
    movl $0, %esi
    # movl %esi, %esi
L18:
    movl $0, %edi
    # movl %esi, %esi
    # movl %edi, %edi
    cmp %edi, %esi
    jne L21
    jmp L22
L22:
    movl $0, %esi
    # movl %esi, %esi
L23:
    movl $0, %edi
    # movl %esi, %esi
    # movl %edi, %edi
    cmp %edi, %esi
    jne L26
    jmp L27
L27:
    nop
L28:
    movl -16(%ebp), %edi
    # movl %edi, %edi
    addl $1, %edi
    movl %edi, -16(%ebp)
    jmp L29
L16:
    movl $1, %esi
    # movl %esi, %esi
    movl 8(%ebp), %edi
    movl -36(%edi), %edi
    movl %edi, -104(%ebp)  # spilled
    movl -16(%ebp), %ebx
    movl %ebx, -60(%ebp)  # spilled
    movl 12(%ebp), %edi
    movl -60(%ebp), %ebx  # spilled
    # movl %ebx, %ebx
    addl %edi, %ebx
    movl $4, %edi
    # movl %ebx, %ebx
    imul %edi, %ebx
    movl -104(%ebp), %edi  # spilled
    # movl %edi, %edi
    addl %ebx, %edi
    movl (%edi), %edi
    movl $0, %ebx
    # movl %edi, %edi
    # movl %ebx, %ebx
    cmp %ebx, %edi
    je L19
    jmp L20
L20:
    movl $0, %esi
    # movl %esi, %esi
L19:
    # movl %esi, %esi
    jmp L18
L21:
    movl $1, %esi
    # movl %esi, %esi
    movl 8(%ebp), %edi
    movl -40(%edi), %ebx
    movl %ebx, -64(%ebp)  # spilled
    movl -16(%ebp), %edi
    # movl %edi, %edi
    addl $7, %edi
    movl 12(%ebp), %ebx
    # movl %edi, %edi
    subl %ebx, %edi
    movl $4, %ebx
    # movl %edi, %edi
    imul %ebx, %edi
    movl -64(%ebp), %ebx  # spilled
    # movl %ebx, %ebx
    addl %edi, %ebx
    movl (%ebx), %edi
    movl $0, %ebx
    # movl %edi, %edi
    # movl %ebx, %ebx
    cmp %ebx, %edi
    je L24
    jmp L25
L25:
    movl $0, %esi
    # movl %esi, %esi
L24:
    # movl %esi, %esi
    jmp L23
L26:
    movl $1, %edi
    movl 8(%ebp), %ebx
    movl -28(%ebx), %ebx
    movl %ebx, -68(%ebp)  # spilled
    movl -16(%ebp), %esi
    movl $4, %ebx
    # movl %esi, %esi
    imul %ebx, %esi
    movl -68(%ebp), %ebx  # spilled
    # movl %ebx, %ebx
    addl %esi, %ebx
    movl %edi, (%ebx)
    movl $1, %ebx
    movl 8(%ebp), %esi
    movl -36(%esi), %esi
    movl %esi, -100(%ebp)  # spilled
    movl -16(%ebp), %edi
    movl %edi, -52(%ebp)  # spilled
    movl 12(%ebp), %esi
    movl -52(%ebp), %edi  # spilled
    # movl %edi, %edi
    movl %edi, -48(%ebp)  # spilled
    movl -48(%ebp), %edi  # spilled
    addl %esi, %edi
    movl %edi, -48(%ebp)  # spilled
    movl $4, %esi
    movl -48(%ebp), %edi  # spilled
    # movl %edi, %edi
    movl %edi, -56(%ebp)  # spilled
    movl -56(%ebp), %edi  # spilled
    imul %esi, %edi
    movl %edi, -56(%ebp)  # spilled
    movl -100(%ebp), %esi  # spilled
    # movl %esi, %esi
    movl -56(%ebp), %edi  # spilled
    addl %edi, %esi
    movl %ebx, (%esi)
    movl $1, %edi
    movl %edi, -96(%ebp)  # spilled
    movl %edi, -96(%ebp)  # spilled
    movl 8(%ebp), %edi
    movl -40(%edi), %ebx
    movl -16(%ebp), %edi
    # movl %edi, %edi
    addl $7, %edi
    movl 12(%ebp), %esi
    movl %esi, -44(%ebp)  # spilled
    # movl %edi, %edi
    movl -44(%ebp), %esi  # spilled
    subl %esi, %edi
    movl $4, %esi
    movl %esi, -40(%ebp)  # spilled
    # movl %edi, %edi
    movl -40(%ebp), %esi  # spilled
    imul %esi, %edi
    # movl %ebx, %ebx
    addl %edi, %ebx
    movl -96(%ebp), %edi  # spilled
    movl -96(%ebp), %edi  # spilled
    movl %edi, (%ebx)
    movl %ebp, %edi
    addl $-16, %edi
    movl (%edi), %edi
    movl 8(%ebp), %ebx
    movl -32(%ebx), %ebx
    movl %ebx, -72(%ebp)  # spilled
    movl 12(%ebp), %esi
    movl $4, %ebx
    # movl %esi, %esi
    imul %ebx, %esi
    movl -72(%ebp), %ebx  # spilled
    # movl %ebx, %ebx
    addl %esi, %ebx
    movl %edi, (%ebx)
    pushl %edx
    pushl %ecx
    movl 12(%ebp), %edi
    # movl %edi, %edi
    addl $1, %edi
    pushl %edi
    movl 8(%ebp), %esi
    pushl %esi
    call L1
    addl $8, %esp
    popl %ecx
    popl %edx
    movl $0, %edi
    movl 8(%ebp), %ebx
    movl -28(%ebx), %ebx
    movl %ebx, -76(%ebp)  # spilled
    movl -16(%ebp), %esi
    movl $4, %ebx
    # movl %esi, %esi
    imul %ebx, %esi
    movl -76(%ebp), %ebx  # spilled
    # movl %ebx, %ebx
    addl %esi, %ebx
    movl %edi, (%ebx)
    movl $0, %ebx
    movl 8(%ebp), %esi
    movl -36(%esi), %esi
    movl %esi, -88(%ebp)  # spilled
    movl -16(%ebp), %edi
    movl %edi, -32(%ebp)  # spilled
    movl 12(%ebp), %esi
    movl -32(%ebp), %edi  # spilled
    # movl %edi, %edi
    movl %edi, -28(%ebp)  # spilled
    movl -28(%ebp), %edi  # spilled
    addl %esi, %edi
    movl %edi, -28(%ebp)  # spilled
    movl $4, %esi
    movl -28(%ebp), %edi  # spilled
    # movl %edi, %edi
    movl %edi, -36(%ebp)  # spilled
    movl -36(%ebp), %edi  # spilled
    imul %esi, %edi
    movl %edi, -36(%ebp)  # spilled
    movl -88(%ebp), %esi  # spilled
    # movl %esi, %esi
    movl -36(%ebp), %edi  # spilled
    addl %edi, %esi
    movl %ebx, (%esi)
    movl $0, %edi
    movl %edi, -84(%ebp)  # spilled
    movl %edi, -84(%ebp)  # spilled
    movl 8(%ebp), %edi
    movl -40(%edi), %ebx
    movl -16(%ebp), %edi
    # movl %edi, %edi
    addl $7, %edi
    movl 12(%ebp), %esi
    movl %esi, -24(%ebp)  # spilled
    # movl %edi, %edi
    movl -24(%ebp), %esi  # spilled
    subl %esi, %edi
    movl $4, %esi
    movl %esi, -20(%ebp)  # spilled
    # movl %edi, %edi
    movl -20(%ebp), %esi  # spilled
    imul %esi, %edi
    # movl %ebx, %ebx
    addl %edi, %ebx
    movl -84(%ebp), %edi  # spilled
    movl -84(%ebp), %edi  # spilled
    movl %edi, (%ebx)
    jmp L28
L36:
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
L39:
    movl $0, -16(%ebp)
L12:
    movl 8(%ebp), %edi
    movl -24(%edi), %edi
    # movl %edi, %edi
    subl $1, %edi
    # movl %edi, %edi
    movl -16(%ebp), %ebx
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    jle L13
    jmp L2
L2:
    pushl %edx
    pushl %ecx
    movl $L14, %edi
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    # movl %eax, %eax
    jmp L38
L13:
    movl $0, -20(%ebp)
L9:
    movl 8(%ebp), %edi
    movl -24(%edi), %edi
    # movl %edi, %edi
    subl $1, %edi
    # movl %edi, %edi
    movl -20(%ebp), %ebx
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    jle L10
    jmp L3
L3:
    pushl %edx
    pushl %ecx
    movl $L11, %edi
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    movl -16(%ebp), %edi
    # movl %edi, %edi
    addl $1, %edi
    movl %edi, -16(%ebp)
    jmp L12
L10:
    movl 8(%ebp), %edi
    movl -32(%edi), %esi
    movl -16(%ebp), %edi
    movl $4, %ebx
    # movl %edi, %edi
    imul %ebx, %edi
    # movl %esi, %esi
    addl %edi, %esi
    movl (%esi), %ebx
    movl -20(%ebp), %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    je L6
    jmp L7
L7:
    movl $L5, %edi
    # movl %edi, %edi
L8:
    pushl %edx
    pushl %ecx
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    movl -20(%ebp), %edi
    # movl %edi, %edi
    addl $1, %edi
    movl %edi, -20(%ebp)
    jmp L9
L6:
    movl $L4, %edi
    # movl %edi, %edi
    jmp L8
L38:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

.data

L14:
    .long 0x1
    .ascii "\n\0\0\0"

L11:
    .long 0x1
    .ascii "\n\0\0\0"

L5:
    .long 0x2
    .ascii " .\0\0\0"

L4:
    .long 0x2
    .ascii " O\0\0\0"

