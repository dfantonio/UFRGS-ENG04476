#include <stdio.h>
#include <math.h>

// Declaração da função Assembly com convenção __stdcall
#ifdef _WIN32
extern int __stdcall sqrt_asm(int n);
#else
extern int sqrt_asm(int n);
#endif

int main()
{
  int numeros[] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 144, 169, 196, 225};
  int num_testes = sizeof(numeros) / sizeof(numeros[0]);

  printf("Testando diferentes valores:\n");
  printf("Número\tAssembly\tC sqrt()\tDiferença\n");
  printf("----------------------------------------\n");

  for (int i = 0; i < num_testes; i++)
  {
    int n = numeros[i];
    int resultado_asm = sqrt_asm(n);
    double resultado_c = sqrt(n);
    double diferenca = fabs(resultado_asm - resultado_c);

    printf("%d\t%d\t\t%.1f\t\t%.1f\n",
           n, resultado_asm, resultado_c, diferenca);
  }

  return 0;
}