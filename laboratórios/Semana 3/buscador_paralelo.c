#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define NUM_THREADS 4
#define ARQUIVO "sherlock.txt"
#define BUSCA "pa"

// Estrutura para passar argumentos para as threads
typedef struct
{
  char **lines;
  int start_line;
  int end_line;
  const char *search_string;
  int local_count;
  int thread_id;
} ThreadArgs;

// Função que cada thread executará
void *search_in_chunk(void *arg)
{
  ThreadArgs *args = (ThreadArgs *)arg;
  args->local_count = 0;

  // Processa as linhas designadas para esta thread
  for (int i = args->start_line; i < args->end_line; i++)
  {
    if (args->lines[i] == NULL)
    {
      continue;
    }

    char *pos = args->lines[i];
    while ((pos = strstr(pos, args->search_string)) != NULL)
    {
      args->local_count++;
      pos += strlen(args->search_string);
    }
  }

  return NULL;
}

int main()
{
  clock_t start, end;
  double cpu_time_used;

  // Primeiro, lê todo o arquivo na memória
  FILE *file = fopen(ARQUIVO, "r");
  if (!file)
  {
    printf("Erro ao abrir arquivo\n");
    return 1;
  }

  // Conta o número total de linhas
  int total_lines = 0;
  char line[MAX_LINE_LENGTH];
  while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
  {
    total_lines++;
  }

  printf("Total de linhas no arquivo: %d\n", total_lines);

  // Aloca memória para armazenar todas as linhas
  char **lines = (char **)malloc(total_lines * sizeof(char *));
  if (lines == NULL)
  {
    printf("Erro ao alocar memória\n");
    fclose(file);
    return 1;
  }

  // Volta ao início do arquivo e lê todas as linhas
  rewind(file);
  for (int i = 0; i < total_lines; i++)
  {
    lines[i] = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    if (lines[i] == NULL)
    {
      printf("Erro ao alocar memória para linha %d\n", i);
      for (int j = 0; j < i; j++)
      {
        free(lines[j]);
      }
      free(lines);
      fclose(file);
      return 1;
    }

    if (fgets(lines[i], MAX_LINE_LENGTH, file) == NULL)
    {
      printf("Erro ao ler linha %d\n", i);
      free(lines[i]);
      for (int j = 0; j < i; j++)
      {
        free(lines[j]);
      }
      free(lines);
      fclose(file);
      return 1;
    }
  }

  fclose(file);

  // Calcula quantas linhas cada thread deve processar
  int lines_per_thread = total_lines / NUM_THREADS;
  int remaining_lines = total_lines % NUM_THREADS;

  // Cria as threads e seus argumentos
  pthread_t threads[NUM_THREADS];
  ThreadArgs thread_args[NUM_THREADS];
  int total_count = 0;

  start = clock();

  for (int loop = 0; loop < 1000; loop++)
  {
    int current_line = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
      thread_args[i].lines = lines;
      thread_args[i].search_string = BUSCA;
      thread_args[i].start_line = current_line;

      // Distribui as linhas restantes entre as threads
      int extra_lines = (i < remaining_lines) ? 1 : 0;
      thread_args[i].end_line = current_line + lines_per_thread + extra_lines;

      current_line = thread_args[i].end_line;

      thread_args[i].local_count = 0;
      thread_args[i].thread_id = i;

      if (pthread_create(&threads[i], NULL, search_in_chunk, &thread_args[i]) != 0)
      {
        printf("Erro ao criar thread %d\n", i);
        return 1;
      }
    }

    // Aguarda todas as threads terminarem e soma os resultados
    for (int i = 0; i < NUM_THREADS; i++)
    {
      pthread_join(threads[i], NULL);
      total_count += thread_args[i].local_count;
    }
  }

  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

  printf("Total de ocorrencias encontradas: %d\n", total_count);
  printf("Tempo de execucao: %f segundos\n", cpu_time_used);

  // Libera a memória alocada
  for (int i = 0; i < total_lines; i++)
  {
    free(lines[i]);
  }
  free(lines);

  return 0;
}