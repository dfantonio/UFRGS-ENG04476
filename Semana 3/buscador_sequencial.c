#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 1024
#define ARQUIVO "sherlock.txt"
#define BUSCA "pa"

int main(int argc, char *argv[])
{
  FILE *file = fopen(ARQUIVO, "r");
  if (!file)
  {
    printf("Erro ao abrir arquivo %s\n", ARQUIVO);
    return 1;
  }

  char line[MAX_LINE];
  int count = 0;
  const char *search_str = BUSCA;
  clock_t start, end;

  start = clock();
  for (int i = 0; i < 1000; i++)
  {
    // Volta para o início do arquivo a cada iteração
    rewind(file);

    while (fgets(line, MAX_LINE, file))
    {
      char *pos = line;
      while ((pos = strstr(pos, search_str)) != NULL)
      {
        count++;
        pos++;
      }
    }
  }

  end = clock();
  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

  fclose(file);
  printf("Total de ocorrencias: %d\n", count);
  printf("Tempo de execucao: %f segundos\n", cpu_time_used);
  return 0;
}