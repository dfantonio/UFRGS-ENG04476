section .text
    global sqrt_asm2
    global _sqrt_asm2@4    ; Para convenção __stdcall no Windows

sqrt_asm2:
_sqrt_asm2@4:
    push ebp
    mov ebp, esp
    
    ; Pega o número de entrada
    mov eax, [ebp+8]   ; Número para calcular a raiz
    
    ; Verifica se é zero
    test eax, eax
    jz done_zero
    
    ; Verifica se é negativo
    test eax, eax
    js done_zero
    
    ; Inicializa a aproximação
    mov ebx, 1         ; Começa com 1 como aproximação inicial
    
    ; Guarda N em ecx para uso no loop
    mov ecx, eax
    
    ; Loop principal do método de Newton
loop_start:
    ; Calcula N/Xn
    mov eax, ecx       ; Move N para eax
    xor edx, edx       ; Limpa edx para divisão
    div ebx            ; Divide ax por bx (N/Xn)
    
    add eax, ebx       ; Soma a raiz aproximada (Xn)
    
    ; Faz a divisão por 2
    shr eax, 1         ; Divide por 2 usando shift right
    
    ; Verifica se convergiu
    cmp eax, ebx       ; Compara o novo valor (Xn+1) com o anterior (Xn)
    mov ebx, eax       ; Atualiza bx com o novo valor
    jne loop_start     ; Se Xn+1 != Xn, continua o loop
    
done_zero:
    xor eax, eax       ; Retorna 0 para números negativos ou zero
    
done:
    mov eax, ebx       ; Move o resultado final para eax
    pop ebp
    ret 