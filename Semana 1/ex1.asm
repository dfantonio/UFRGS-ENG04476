section .data
    msg db "A raiz eh: "
    msg_len equ $ - msg
    buffer db 0, 0      ; Buffer para dois dígitos
    newline db 0ah

section .text
    global _start

print_number:
    ; Código necessário para a função
    push rbp
    mov rbp, rsp
    
    ; Converte o número para ASCII (dois dígitos)
    mov rax, r8         ; Move o número para rax
    mov rdx, 0          ; Limpa rdx para divisão
    mov rbx, 10         ; Divisor para separar dígitos
    div rbx             ; Divide por 10 (quociente em rax, resto em rdx)
    
    ; Converte primeiro dígito (dezena)
    add rax, '0'        ; Converte para ASCII
    mov [buffer], al    ; Salva primeiro dígito
    
    ; Converte segundo dígito (unidade)
    mov rax, rdx        ; Move o resto para rax
    add rax, '0'        ; Converte para ASCII
    mov [buffer+1], al  ; Salva segundo dígito

    ; Exibe os dois dígitos
    mov rax, 1          ; syscall write
    mov rdi, 1          ; stdout
    mov rsi, buffer     ; endereço do buffer
    mov rdx, 2          ; tamanho (2 bytes)
    syscall

    ; Exibe nova linha
    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall
    
    ; Código necessário para a função
    mov rsp, rbp
    pop rbp
    ret

_start:
    ; Algoritmo de raiz quadrada por Newton-Raphson
    ; Equação: (N + Xn^2) / (2*Xn)

    ; 1. Inicialização
    mov bx, 1           ; raiz aproximada (Xn)
    mov cx, 144         ; Valor que queremos calcular (N)

    ; 2. Loop principal
    loop_start:
    ; Calcula o quadrado de raiz aproximada (Xn) e armazena em ax
    mov ax, bx          ; Move bx para ax
    mul bx              ; Multiplica ax por bx

    add ax, cx          ; Soma o valor original (N)

    ; Prepara o divisor (2*Xn)
    mov si, bx          ; Move o valor de raiz aproximada (Xn) para si
    add si, bx          ; Soma o valor de raiz aproximada (Xn) novamente
    
    div si              ; Calcula Xn+1 (ax = ax/si)

    cmp ax, bx          ; Compara o novo valor (Xn+1) com o anterior (Xn)
    mov bx, ax          ; Atualiza bx com o novo valor
    jne loop_start      ; Se Xn+1 != Xn, continua o loop

    ; Coloca o resultado final no registrador
    mov r8, rax        ; Número que queremos exibir

    ; Exibe a mensagem
    mov rax, 1          ; syscall write
    mov rdi, 1          ; stdout
    mov rsi, msg        ; mensagem
    mov rdx, msg_len    ; tamanho
    syscall

    ; Chama a função para imprimir o número
    call print_number

    ; Encerra o programa com sucesso
    mov rax, 60         ; syscall exit
    mov rdi, 0          ; código de retorno 0
    syscall
