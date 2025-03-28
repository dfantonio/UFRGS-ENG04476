
section .text
    global sqrt_asm
    global _sqrt_asm@4    ; Para convenção __stdcall no Windows

sqrt_asm:
_sqrt_asm@4:
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
    mov ebx, eax       ; Começa com o próprio número
    shr ebx, 1         ; Divide por 2 para melhor aproximação inicial
    
    ; Verifica se ebx é zero
    test ebx, ebx
    jz done_zero
    
    ; Guarda N em ecx para uso no loop
    mov ecx, eax
    
    ; Loop principal do método de Newton
loop_start:
    ; Calcula Xn+1 = (N/Xn + Xn)/2
    mov eax, ecx       ; Move N para eax
    xor edx, edx       ; Limpa edx para divisão
    div ebx            ; N/Xn
    
    add eax, ebx       ; + Xn
    shr eax, 1         ; /2
    
    ; Verifica se convergiu
    cmp eax, ebx
    je done            ; Se Xn+1 = Xn, terminamos
    
    ; Atualiza para próxima iteração
    mov ebx, eax
    jmp loop_start
    
done_zero:
    xor eax, eax       ; Retorna 0 para números negativos ou zero
    
done:
    mov eax, ebx       ; Move o resultado final para eax
    pop ebp
    ret
