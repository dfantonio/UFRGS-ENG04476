#include <stdio.h>
#include <math.h>
#include <time.h>

// Declara��o das fun��es Assembly
#ifdef _WIN32
extern int __stdcall sqrt_asm(int n);
extern int __stdcall sqrt_asm2(int n);
#else
extern int sqrt_asm(int n);
extern int sqrt_asm2(int n);
#endif

// Implementa��o em C do m�todo de Newton
double sqrt_newton(double n)
{
  double x = n;
  double y = 1;
  double e = 0.000001; // Precis�o desejada

  while (x - y > e)
  {
    x = (x + y) / 2;
    y = n / x;
  }
  return x;
}

int main()
{
  int numeros[] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 144, 169, 196, 225};
  int num_testes = sizeof(numeros) / sizeof(numeros[0]);
  int iterations = 100000; // N�mero de itera��es para teste de tempo

  printf("Compara��o de Implementa��es de Raiz Quadrada\n");
  printf("============================================\n\n");

  // Tabela de resultados
  printf("| Implementa��o           | Tamanho (bytes) | Tempo de execu��o (1000.000 execu��es) |\n");
  printf("| ----------------------- | --------------- | -------------------------------------- |\n");

  // Teste da implementa��o Assembly 1
  clock_t start = clock();
  for (int i = 0; i < iterations; i++)
  {
    for (int j = 0; j < num_testes; j++)
    {
      sqrt_asm(numeros[j]);
    }
  }
  clock_t end = clock();
  double time_asm1 = ((double)(end - start)) / CLOCKS_PER_SEC;

  // Teste da implementa��o Assembly 2
  start = clock();
  for (int i = 0; i < iterations; i++)
  {
    for (int j = 0; j < num_testes; j++)
    {
      sqrt_asm2(numeros[j]);
    }
  }
  end = clock();
  double time_asm2 = ((double)(end - start)) / CLOCKS_PER_SEC;

  // Teste da implementa��o C (Newton)
  // start = clock();
  // for (int i = 0; i < iterations; i++)
  // {
  //   for (int j = 0; j < num_testes; j++)
  //   {
  //     sqrt_newton(numeros[j]);
  //   }
  // }
  // end = clock();
  // double time_c = ((double)(end - start)) / CLOCKS_PER_SEC;

  // // Teste da fun��o sqrt() padr�o
  // start = clock();
  // for (int i = 0; i < iterations; i++)
  // {
  //   for (int j = 0; j < num_testes; j++)
  //   {
  //     sqrt(numeros[j]);
  //   }
  // }
  // end = clock();
  // double time_sqrt = ((double)(end - start)) / CLOCKS_PER_SEC;

  // Imprime resultados na tabela
  printf("| Raiz quadrada ASM 1     | %14d | %41.6f |\n", 0, time_asm1);
  printf("| Raiz quadrada ASM 2     | %14d | %41.6f |\n", 0, time_asm2);
  // printf("| Raiz quadrada C         | %14d | %41.6f |\n", 0, time_c);
  // printf("| Fun��o sqrt()           | %14d | %41.6f |\n", 0, time_sqrt);

  // Imprime resultados detalhados
  printf("\nResultados Detalhados:\n");
  printf("N�mero\tASM1\tASM2\tC\tsqrt()\n");
  printf("----------------------------------------\n");

  // for (int i = 0; i < num_testes; i++)
  // {
  //   int n = numeros[i];
  //   printf("%d\t%d\t%d\t%.1f\t%.1f\n",
  //          n, sqrt_asm(n), sqrt_asm2(n), sqrt_newton(n), sqrt(n));
  // }

  return 0;
}