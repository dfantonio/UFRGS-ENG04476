#include <stdio.h>
#include <stdlib.h>

// Função para calcular a raiz quadrada usando o método de Newton-Raphson
int calcular_raiz(int n)
{
  // Inicialização
  int xn = 1; // Aproximação inicial
  int xn_anterior;

  do
  {
    xn_anterior = xn;
    // Fórmula: Xn+1 = (N/Xn + Xn) / 2
    xn = (n / xn + xn) / 2;
  } while (xn != xn_anterior); // Continua até convergir

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
