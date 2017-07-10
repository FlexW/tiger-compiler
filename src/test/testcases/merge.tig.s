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
    movl -20(%ebp), %edi
    pushl %edi
    movl -24(%ebp), %ecx
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
    je L55
    jmp L56
L56:
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
L57:
    movl $0, %edi
    movl %edi, %eax
    jmp L60
L55:
    pushl %edx
    pushl %ecx
    movl $L53, %edi
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
    jmp L57
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
    jl L50
    jmp L51
L51:
    movl 12(%ebp), %ebx
    movl $0, %edi
    # movl %ebx, %ebx
    # movl %edi, %edi
    cmp %edi, %ebx
    jg L47
    jmp L48
L48:
    pushl %edx
    pushl %ecx
    movl $L46, %edi
    pushl %edi
    call print
    addl $4, %esp
    popl %ecx
    popl %edx
L49:
    nop
L52:
    movl $0, %edi
    movl %edi, %eax
    jmp L62
L50:
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
    jmp L52
L47:
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
    jmp L49
L62:
    nop
    ad