@echo off

:: Compila os arquivos Assembly
nasm -f win32 sqrt_asm.asm -o sqrt_asm.obj
nasm -f win32 sqrt_asm2.asm -o sqrt_asm2.obj

:: Compila o arquivo C e linka com os objetos Assembly
gcc -m32 main2.c sqrt_asm.obj sqrt_asm2.obj -o programa2.exe

:: Executa o programa
programa2.exe 