#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARQUIVO "sherlock.txt"
#define BUSCA "pa"
#define NUM_EXECUCOES 1000

int main()
{
  char comando[256];
  clock_t inicio, fim;
  double tempo_total = 0.0;
  double tempo_medio;
  int i;

  // Verifica se o arquivo existe
  FILE *arquivo = fopen(ARQUIVO, "r");
  if (arquivo == NULL)
  {
    printf("Erro ao abrir o arquivo %s\n", ARQUIVO);
    return 1;
  }
  fclose(arquivo);

  // Prepara o comando find
  sprintf(comando, "find /c \"%s\" %s", BUSCA, ARQUIVO);

  printf("Iniciando %d execucoes do comando find...\n", NUM_EXECUCOES);

  inicio = clock();
  for (i = 0; i < NUM_EXECUCOES; i++)
  {
    // Executa o comando find
    system(comando);
  }
  fim = clock();

  // Remove o arquivo temporário
  remove("resultado.txt");

  // Calcula e mostra o tempo médio
  tempo_total = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  tempo_medio = tempo_total / NUM_EXECUCOES;

  printf("\nResultados:\n");
  printf("Tempo total de execucao: %.4f segundos\n", tempo_total);
  printf("Tempo medio por execucao: %.4f segundos\n", tempo_medio);

  return 0;
}