@echo off

:: Compila os arquivos Assembly
nasm -f win32 sqrt_asm.asm -o sqrt_asm.obj
nasm -f win32 sqrt_asm2.asm -o sqrt_asm2.obj
nasm -f win32 sqrt_fpu.asm -o sqrt_fpu.obj

:: Compila o arquivo C e linka com os objetos Assembly
gcc -m32 -funroll-loops comparacao.c sqrt_asm.obj sqrt_asm2.obj sqrt_fpu.obj -o comparacao.exe

:: Executa o programa
comparacao.exe 