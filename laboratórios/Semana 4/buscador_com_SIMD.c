#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <immintrin.h>

#define MAX_LINE 1024
#define ARQUIVO "arquivo.txt"
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
  size_t search_len = strlen(search_str);
  clock_t start, end;

  start = clock();
  for (int i = 0; i < 1000; i++)
  {
    rewind(file);

    while (fgets(line, MAX_LINE, file))
    {
      size_t line_len = strlen(line);
      if (line_len < search_len)
        continue;

      // Alinhar a linha para 16 bytes (requisito SSE)
      char *aligned_line = (char *)_mm_malloc(line_len + 16, 16);
      if (!aligned_line)
      {
        printf("Erro ao alocar memória alinhada\n");
        fclose(file);
        return 1;
      }
      memcpy(aligned_line, line, line_len);

      // Preparar vetor SIMD com o primeiro caractere da string de busca
      __m128i search_vec = _mm_set1_epi8(search_str[0]);
      size_t pos = 0;

      // Processar a linha em blocos de 16 bytes
      while (pos <= line_len - search_len)
      {
        // Carregar 16 bytes da linha
        __m128i line_vec = _mm_load_si128((__m128i *)(aligned_line + pos));
        // Comparar com o primeiro caractere da busca
        __m128i cmp = _mm_cmpeq_epi8(line_vec, search_vec);
        // Criar máscara de bits para posições correspondentes
        int mask = _mm_movemask_epi8(cmp);

        // Verificar cada posição onde o primeiro caractere coincide
        while (mask != 0)
        {
          int bit_pos = __builtin_ctz(mask); // Posição do primeiro bit 1
          if (pos + bit_pos + search_len <= line_len &&
              strncmp(aligned_line + pos + bit_pos, search_str, search_len) == 0)
          {
            count++;
          }
          mask &= ~(1 << bit_pos); // Limpar o bit processado
        }

        pos += 16; // Avançar para o próximo bloco de 16 bytes
      }

      _mm_free(aligned_line);
    }
  }

  end = clock();
  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

  fclose(file);
  printf("Total de ocorrencias: %d\n", count / 1000);
  printf("Tempo de execucao: %f segundos\n", cpu_time_used);
  return 0;
}