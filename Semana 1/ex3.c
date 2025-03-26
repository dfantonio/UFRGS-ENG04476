#include <stdio.h>
#include <stdlib.h>

// Fun��o para calcular a raiz quadrada usando o m�todo de Newton-Raphson
int calcular_raiz(int n)
{
  // Inicializa��o
  int xn = 1; // Aproxima��o inicial
  int xn_anterior;

  do
  {
    xn_anterior = xn;
    // F�rmula: Xn+1 = (N/Xn + Xn) / 2
    xn = (n / xn + xn) / 2;
  } while (xn != xn_anterior); // Continua at� convergir

  return xn;
}

int main()
{
  int numero;

  printf("Digite um numero: ");
  scanf("%d", &numero);

  // Calcula a raiz quadrada
  int resultado = calcular_raiz(numero);

  // Exibe o resultado
  printf("A raiz eh: %d\n", resultado);

  return 0;
}
