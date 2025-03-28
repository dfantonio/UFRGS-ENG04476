section .text
    global sqrt_fpu
    global _sqrt_fpu@4    ; Para convenção __stdcall no Windows

sqrt_fpu:
_sqrt_fpu@4:
    push ebp
    mov ebp, esp
    
    ; Pega o número de entrada
    fild dword [ebp+8]    ; Carrega o número inteiro na FPU
    fsqrt                 ; Calcula a raiz quadrada
    fistp dword [ebp+8]   ; Converte o resultado de volta para inteiro e salva
    
    mov eax, [ebp+8]      ; Move o resultado para eax (retorno)
    
    pop ebp
    ret