sys_read:               equ                     0
sys_write:              equ                     1
sys_exit:               equ                     60

stdin:                  equ                     0
stdout:                 equ                     1
stderr:                 equ                     2

                        section                 .text

                        global                  _start
_start:
                        xor                     ebx, ebx
                        mov			r8, 1
.read_next_buff:
                        mov                     eax, sys_read
                        mov                     edi, stdin
                        mov                     rsi, buffer
                        mov                     rdx, buffer_len
                        syscall

                        test                    rax, rax
                        js                      .read_error
                        jz                      .eof

                        xor                     ecx, ecx

.next_byte:
			mov			r10, r8
			mov			r8, 1
                        mov			r9b, [rsi + rcx]
                        cmp                     r9b, 32
                        je                      .whiteSpace
                        sub			r9b, 9
                        cmp                     r9b, 14 - 9
                        jl                      .whiteSpace
                        add                     rbx, r10
                        xor			r8, r8
.whiteSpace:
                        inc                     rcx

			cmp                     rcx, rax
                        je                      .read_next_buff
                        jmp                     .next_byte

.eof:
                        call                    print_int

                        mov                     eax, sys_exit
                        xor                     edi, edi
                        syscall

.read_error:
                        mov                     eax, sys_write
                        mov                     edi, stderr
                        mov                     rsi, error_message
                        mov                     rdx, error_message_len
                        syscall

                        mov                     eax, sys_exit
                        mov                     edi, -1
                        syscall

print_int:
                        mov                     rax, rbx
                        mov                     ecx, 10

                        mov                     rsi, rsp

                        dec                     rsi
                        mov                     BYTE [rsi], `\n`

                        

.next_digit:
                        xor                     edx, edx

                        div                     rcx
                        dec                     rsi
                        add                     dl, '0'
                        mov                     [rsi], dl

                        test                    rax, rax
                        jnz                     .next_digit

                        mov                     eax, sys_write
                        mov                     edi, stdout
                        ; mov                     rsi, rsi
                        mov                     rdx, rsp
                        sub                     rdx, rsi
                        syscall

                        ret

                        section                 .rodata
error_message:          db                      `Read error!\n`
error_message_len:      equ                     $ - error_message

                        section                 .bss
buffer_len:             equ                     4096
buffer:                 resb                    buffer_len
