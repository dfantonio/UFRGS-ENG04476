#include <stdio.h>
#include <math.h>
#include <time.h>

// Declara��o das fun��es Assembly
#ifdef _WIN32
extern int __stdcall sqrt_asm(int n);
extern int __stdcall sqrt_asm2(int n);
extern int __stdcall sqrt_fpu(int n);
#else
extern int sqrt_asm(int n);
extern int sqrt_asm2(int n);
extern int sqrt_fpu(int n);
#endif

// Implementa��o em C do m�todo de Newton
double sqrt_newton(double n)
{
  int x = n;
  int y = 1;

  while (x - y > 0)
  {
    x = (x + y) / 2;
    y = n / x;
  }
  return x;
}

int main()
{
  int raiz = 40000;
  int iterations = 100000; // N�mero de itera��es para teste de tempo

  printf("Compara��o de Implementa��es de Raiz Quadrada\n");
  printf("============================================\n\n");

  // Tabela de resultados
  printf("| Implementa��o           | Tamanho (bytes) | N�mero de Clocks (100.000 execu��es) |\n");
  printf("| ----------------------- | --------------- | ------------------------------------ |\n");

  // Teste da implementa��o Assembly 1
  clock_t start = clock();
  for (int i = 0; i < iterations; i++)
  {
    sqrt_asm(raiz);
  }
  clock_t end = clock();
  clock_t time_asm1 = end - start;

  // Teste da implementa��o Assembly 2
  start = clock();
  for (int i = 0; i < iterations; i++)
  {
    sqrt_asm2(raiz);
  }
  end = clock();
  clock_t time_asm2 = end - start;

  // Teste da implementa��o FPU
  start = clock();
  for (int i = 0; i < iterations; i++)
  {
    sqrt_fpu(raiz);
  }
  end = clock();
  clock_t time_fpu = end - start;

  // Teste da implementa��o C (Newton)
  start = clock();
  for (int i = 0; i < iterations; i++)
  {
    sqrt_newton(raiz);
  }
  end = clock();
  clock_t time_c = end - start;

  // Teste da fun��o sqrt() padr�o
  start = clock();
  for (int i = 0; i < iterations; i++)
  {
    sqrt(raiz);
  }
  end = clock();
  clock_t time_sqrt = end - start;

  // Imprime resultados na tabela
  printf("| Raiz quadrada ASM 1     | %14d | %41ld |\n", 0, time_asm1);
  printf("| Raiz quadrada ASM 2     | %14d | %41ld |\n", 0, time_asm2);
  printf("| Raiz quadrada C         | %14d | %41ld |\n", 0, time_c);
  printf("| Fun��o sqrt()           | %14d | %41ld |\n", 0, time_sqrt);
  printf("| Raiz quadrada FPU       | %14d | %41ld |\n", 0, time_fpu);

  return 0;
}