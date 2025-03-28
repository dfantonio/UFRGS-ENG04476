@echo off

echo "Compilando..."

:: Compila o arquivo Assembly
nasm -f win32 sqrt_asm.asm -o sqrt_asm.obj

:: Compila o arquivo C e linka com o objeto Assembly
gcc -m32 main.c sqrt_asm.obj -o programa.exe

echo "Executando..."

:: Executa o programa
programa.exe 