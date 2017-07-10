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
L59:
    movl %ebp, %edi
    addl $-16, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    call getchar
    addl $0, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, (%edi)
    movl %ebp, %edi
    addl $-20, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl %ebp, %esi
    addl $8, %esi
    pushl %esi
    call L24
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, (%edi)
    movl %ebp, %edi
    addl $-24, %edi
    # movl %edi, %edi
    movl %ebp, %ebx
    addl $-16, %ebx
    # movl %ebx, %ebx
    pushl %edx
    pushl %ecx
    call getchar
    addl $0, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, (%ebx)
    pushl %edx
    pushl %ecx
    movl %ebp, %esi
    addl $8, %esi
    pushl %esi
    call L24
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, (%edi)
    movl %ebp, %esi
    addl $8, %esi
    # movl %esi, %esi
    pushl %edx
    pushl %ecx
    movl -24(%ebp), %edi
    pushl %edi
    movl -20(%ebp), %ecx
    pushl %ecx
    movl %ebp, %ebx
    addl $8, %ebx
    pushl %ebx
    call L25
    addl $12, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
    pushl %edx
    pushl %ecx
    pushl %edi
    pushl %esi
    call L27
    addl $8, %esp
    popl %ecx
    popl %edx
    # movl %eax, %eax
    jmp L58
L58:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

# PROCEDURE L27

L27:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    subl $100, %esp
L61:
    movl 12(%ebp), %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    je L56
    jmp L57
L57:
    pushl %edx
    pushl %ecx
    movl 12(%ebp), %esi
    movl $0, %edi
    movl $4, %ecx
    # movl %edi, %edi
    imul %ecx, %edi
    # movl %esi, %esi
    addl %edi, %esi
    movl (%esi), %edi
    pushl %edi
    movl 8(%ebp), %ecx
    pushl %ecx
    call L26
    addl $8, %esp
    popl %ecx
    popl %edx
    pushl %edx
    pushl %ecx
    movl $L54, %edi
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    pushl %edx
    pushl %ecx
    movl 12(%ebp), %esi
    movl $1, %edi
    movl $4, %ecx
    # movl %edi, %edi
    imul %ecx, %edi
    # movl %esi, %esi
    addl %edi, %esi
    movl (%esi), %edi
    pushl %edi
    movl 8(%ebp), %ecx
    pushl %ecx
    call L27
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
L55:
    movl %edi, %eax
    jmp L60
L56:
    pushl %edx
    pushl %ecx
    movl $L53, %edi
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
    jmp L55
L60:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

# PROCEDURE L26

L26:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    subl $100, %esp
L63:
    movl 12(%ebp), %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    jl L51
    jmp L52
L52:
    movl 12(%ebp), %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    jg L48
    jmp L49
L49:
    pushl %edx
    pushl %ecx
    movl $L46, %edi
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
L47:
    # movl %edi, %edi
L50:
    movl %edi, %eax
    jmp L62
L51:
    pushl %edx
    pushl %ecx
    movl $L45, %edi
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    pushl %edx
    pushl %ecx
    movl $0, %edi
    movl 12(%ebp), %esi
    # movl %edi, %edi
    subl %esi, %edi
    pushl %edi
    movl %ebp, %esi
    addl $8, %esi
    pushl %esi
    call L40
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
    jmp L50
L48:
    pushl %edx
    pushl %ecx
    movl 12(%ebp), %ecx
    pushl %ecx
    movl %ebp, %edi
    addl $8, %edi
    pushl %edi
    call L40
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
    jmp L47
L62:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

# PROCEDURE L40

L40:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    subl $100, %esp
L65:
    movl 12(%ebp), %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    jg L43
    jmp L44
L44:
    movl $0, %edi
    movl %edi, %eax
    jmp L64
L43:
    pushl %edx
    pushl %ecx
    movl 12(%ebp), %edi
    movl $10, %esi
    movl %edi, %eax
    movl $0, %edx
    divl %esi
    movl %eax, %edi
    pushl %edi
    movl 8(%ebp), %ecx
    pushl %ecx
    call L40
    addl $8, %esp
    popl %ecx
    popl %edx
    movl 12(%ebp), %edi
    movl 12(%ebp), %edx
    movl $10, %ebx
    movl %edx, %eax
    movl $0, %edx
    divl %ebx
    movl %eax, %ebx
    movl $10, %esi
    # movl %ebx, %ebx
    imul %esi, %ebx
    # movl %edi, %edi
    subl %ebx, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl $L41, %ecx
    pushl %ecx
    call ord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    pushl %edx
    pushl %ecx
    # movl %edi, %edi
    addl %esi, %edi
    pushl %edi
    call chr
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
    pushl %edx
    pushl %ecx
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    jmp L44
L64:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

# PROCEDURE L25

L25:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    subl $100, %esp
L67:
    movl 16(%ebp), %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    je L38
    jmp L39
L39:
    movl 12(%ebp), %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    je L35
    jmp L36
L36:
    movl 16(%ebp), %esi
    movl $0, %edi
    movl $4, %ebx
    # movl %edi, %edi
    imul %ebx, %edi
    # movl %esi, %esi
    addl %edi, %esi
    movl (%esi), %ebx
    movl 12(%ebp), %edi
    movl %edi, -20(%ebp)  # spilled
    movl $0, %esi
    movl $4, %edi
    # movl %esi, %esi
    imul %edi, %esi
    movl -20(%ebp), %edi  # spilled
    # movl %edi, %edi
    addl %esi, %edi
    movl (%edi), %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    jl L32
    jmp L33
L33:
    pushl %edx
    pushl %ecx
    movl $8, %edi
    pushl %edi
    call allocRecord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, -16(%ebp)  # spilled
    movl -16(%ebp), %esi  # spilled
    # movl %esi, %esi
    addl $4, %esi
    # movl %esi, %esi
    pushl %edx
    pushl %ecx
    movl 12(%ebp), %edi
    movl $1, %edx
    movl $4, %ecx
    # movl %edx, %edx
    imul %ecx, %edx
    # movl %edi, %edi
    addl %edx, %edi
    movl (%edi), %edx
    pushl %edx
    movl 16(%ebp), %ecx
    pushl %ecx
    movl 8(%ebp), %edi
    pushl %edi
    call L25
    addl $12, %esp
    popl %ecx
    popl %edx
    movl %eax, %ebx
    movl %ebx, (%esi)
    movl 12(%ebp), %esi
    movl $0, %edi
    movl $4, %ebx
    # movl %edi, %edi
    imul %ebx, %edi
    # movl %esi, %esi
    addl %edi, %esi
    movl (%esi), %ebx
    movl -16(%ebp), %esi  # spilled
    movl %ebx, 0(%esi)
    movl -16(%ebp), %esi  # spilled
    # movl %esi, %esi
L31:
    # movl %esi, %esi
L34:
    # movl %esi, %esi
L37:
    movl %esi, %eax
    jmp L66
L38:
    movl 12(%ebp), %esi
    # movl %esi, %esi
    jmp L37
L35:
    movl 16(%ebp), %esi
    # movl %esi, %esi
    jmp L34
L32:
    pushl %edx
    pushl %ecx
    movl $8, %edi
    pushl %edi
    call allocRecord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, %edi
    addl $4, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl 12(%ebp), %edx
    pushl %edx
    movl 16(%ebp), %ecx
    movl %ecx, -24(%ebp)  # spilled
    movl $1, %ebx
    movl $4, %ecx
    # movl %ebx, %ebx
    imul %ecx, %ebx
    movl -24(%ebp), %ecx  # spilled
    # movl %ecx, %ecx
    addl %ebx, %ecx
    movl (%ecx), %ecx
    pushl %ecx
    movl 8(%ebp), %ebx
    pushl %ebx
    call L25
    addl $12, %esp
    popl %ecx
    popl %edx
    movl %eax, %ebx
    movl %ebx, (%edi)
    movl 16(%ebp), %edi
    movl %edi, -28(%ebp)  # spilled
    movl $0, %ebx
    movl $4, %edi
    # movl %ebx, %ebx
    imul %edi, %ebx
    movl -28(%ebp), %edi  # spilled
    # movl %edi, %edi
    addl %ebx, %edi
    movl (%edi), %edi
    movl %edi, 0(%esi)
    # movl %esi, %esi
    jmp L31
L66:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

# PROCEDURE L24

L24:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    subl $100, %esp
L69:
    movl %ebp, %edi
    addl $-16, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl $4, %esi
    pushl %esi
    call allocRecord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl $0, 0(%esi)
    movl %esi, (%edi)
    movl %ebp, %edi
    addl $-20, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl -16(%ebp), %esi
    pushl %esi
    movl 8(%ebp), %ecx
    pushl %ecx
    call L0
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    movl %esi, (%edi)
    movl -16(%ebp), %esi
    movl $0, %edi
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
    jne L29
    jmp L30
L30:
    movl $0, %edi
    # movl %edi, %edi
L28:
    movl %edi, %eax
    jmp L68
L29:
    pushl %edx
    pushl %ecx
    movl $8, %edi
    pushl %edi
    call allocRecord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
    movl %edi, %esi
    addl $4, %esi
    # movl %esi, %esi
    pushl %edx
    pushl %ecx
    movl 8(%ebp), %ebx
    pushl %ebx
    call L24
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %ebx
    movl %ebx, (%esi)
    movl -20(%ebp), %esi
    movl %esi, 0(%edi)
    # movl %edi, %edi
    jmp L28
L68:
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
L71:
    movl $0, -16(%ebp)
    pushl %edx
    pushl %ecx
    movl %ebp, %edi
    addl $8, %edi
    pushl %edi
    call L2
    addl $4, %esp
    popl %ecx
    popl %edx
    movl 12(%ebp), %esi
    movl $0, %edi
    movl $4, %ebx
    # movl %edi, %edi
    imul %ebx, %edi
    # movl %esi, %esi
    addl %edi, %esi
    # movl %esi, %esi
    pushl %edx
    pushl %ecx
    movl 8(%ebp), %ecx
    movl -24(%ecx), %edi
    pushl %edi
    movl %ebp, %ecx
    addl $8, %ecx
    pushl %ecx
    call L1
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %ebx
    movl %ebx, (%esi)
L22:
    pushl %edx
    pushl %ecx
    movl 8(%ebp), %edi
    movl -24(%edi), %ecx
    pushl %ecx
    movl %ebp, %edi
    addl $8, %edi
    pushl %edi
    call L1
    addl $8, %esp
    popl %ecx
    popl %edx
    movl %eax, %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    jne L23
    jmp L20
L20:
    movl -16(%ebp), %edi
    movl %edi, %eax
    jmp L70
L23:
    movl %ebp, %edi
    addl $-16, %edi
    # movl %edi, %edi
    movl -16(%ebp), %ebx
    movl $10, %esi
    # movl %ebx, %ebx
    imul %esi, %ebx
    # movl %ebx, %ebx
    pushl %edx
    pushl %ecx
    movl 8(%ebp), %ecx
    movl -24(%ecx), %ecx
    pushl %ecx
    call ord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    # movl %ebx, %ebx
    addl %esi, %ebx
    # movl %ebx, %ebx
    pushl %edx
    pushl %ecx
    movl $L21, %ecx
    pushl %ecx
    call ord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    # movl %ebx, %ebx
    subl %esi, %ebx
    movl %ebx, (%edi)
    movl 8(%ebp), %edi
    # movl %edi, %edi
    addl $-24, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    call getchar
    addl $0, %esp
    popl %ecx
    popl %edx
    movl %eax, %ebx
    movl %ebx, (%edi)
    jmp L22
L70:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

# PROCEDURE L2

L2:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    subl $100, %esp
L18:
    movl 8(%ebp), %edi
    movl (%edi), %edi
    movl -24(%edi), %ebx
    movl $L11, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    je L14
    jmp L15
L15:
    movl $1, %esi
    # movl %esi, %esi
    movl 8(%ebp), %ebx
    movl (%ebx), %ebx
    movl -24(%ebx), %edi
    movl $L12, %ebx
    # movl %edi, %edi
    # movl %ebx, %ebx
    cmp %ebx, %edi
    je L16
    jmp L17
L17:
    movl $0, %esi
    # movl %esi, %esi
L16:
    # movl %esi, %esi
L13:
    movl $0, %edi
    # movl %esi, %esi
    # movl %edi, %edi
    cmp %edi, %esi
    jne L19
    jmp L10
L10:
    movl $0, %edi
    movl %edi, %eax
    jmp L72
L14:
    movl $1, %esi
    # movl %esi, %esi
    jmp L13
L19:
    movl 8(%ebp), %edi
    movl (%edi), %edi
    # movl %edi, %edi
    addl $-24, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    call getchar
    addl $0, %esp
    popl %ecx
    popl %edx
    movl %eax, %ebx
    movl %ebx, (%edi)
    jmp L18
L72:
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
L74:
    pushl %edx
    pushl %ecx
    movl 8(%ebp), %edi
    movl (%edi), %edi
    movl -24(%edi), %edi
    pushl %edi
    call ord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %ebx
    # movl %ebx, %ebx
    pushl %edx
    pushl %ecx
    movl $L3, %edi
    pushl %edi
    call ord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    # movl %ebx, %ebx
    # movl %esi, %esi
    cmp %esi, %ebx
    jge L6
    jmp L7
L7:
    movl $0, %ebx
    # movl %ebx, %ebx
L5:
    movl %ebx, %eax
    jmp L73
L6:
    movl $1, %ebx
    # movl %ebx, %ebx
    pushl %edx
    pushl %ecx
    movl 8(%ebp), %edi
    movl (%edi), %edi
    movl -24(%edi), %edi
    pushl %edi
    call ord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %edi
    # movl %edi, %edi
    pushl %edx
    pushl %ecx
    movl $L4, %esi
    pushl %esi
    call ord
    addl $4, %esp
    popl %ecx
    popl %edx
    movl %eax, %esi
    # movl %edi, %edi
    # movl %esi, %esi
    cmp %esi, %edi
    jle L8
    jmp L9
L9:
    movl $0, %ebx
    # movl %ebx, %ebx
L8:
    # movl %ebx, %ebx
    jmp L5
L73:
    nop
    addl $100, %esp
    popl %edi
    popl %esi
    popl %ebx
    leave
    ret

# END

.data

L54:
    .long 0x1
    .ascii " \0\0\0"

L53:
    .long 0x1
    .ascii "\n\0\0\0"

L46:
    .long 0x1
    .ascii "0\0\0\0"

L45:
    .long 0x1
    .ascii "-\0\0\0"

L41:
    .long 0x1
    .ascii "0\0\0\0"

L21:
    .long 0x1
    .ascii "0\0\0\0"

L12:
    .long 0x1
    .ascii "\n\0\0\0"

L11:
    .long 0x1
    .ascii " \0\0\0"

L4:
    .long 0x1
    .ascii "9\0\0\0"

L3:
    .long 0x1
    .ascii "0\0\0\0"

