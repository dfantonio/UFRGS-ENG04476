#include <stdio.h>
#include <math.h>

// Declaração das funções Assembly
#ifdef _WIN32
extern int __stdcall sqrt_asm(int n);
extern int __stdcall sqrt_asm2(int n);
#else
extern int sqrt_asm(int n);
extern int sqrt_asm2(int n);
#endif

int main()
{
  int numeros[] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 144, 169, 196, 225};
  int num_testes = sizeof(numeros) / sizeof(numeros[0]);

  printf("Testando diferentes valores:\n");
  printf("Número\tAssembly1\tAssembly2\tC sqrt()\tDif1\tDif2\n");
  printf("--------------------------------------------------------\n");

  for (int i = 0; i < num_testes; i++)
  {
    int n = numeros[i];
    int resultado_asm1 = sqrt_asm(n);
    int resultado_asm2 = sqrt_asm2(n);
    double resultado_c = sqrt(n);
    double diferenca1 = fabs(resultado_asm1 - resultado_c);
    double diferenca2 = fabs(resultado_asm2 - resultado_c);

    printf("%d\t%d\t\t%d\t\t%.1f\t\t%.1f\t%.1f\n",
           n, resultado_asm1, resultado_asm2, resultado_c, diferenca1, diferenca2);
  }

  return 0;
}