#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <emmintrin.h>
#include <mmintrin.h>
#include <windows.h>

#define CABECALHO_SIZE 1078
#define NUM_TESTES 100 // Número de vezes que cada versão será executada

// Função que implementa a versão sem MMX
double processar_sem_mmx(const char *arquivo_entrada, const char *arquivo_saida, int valor_brilho)
{
  FILE *entrada, *saida;
  unsigned char cabecalho[CABECALHO_SIZE];
  unsigned char pixel[3];
  int bytes_lidos;
  LARGE_INTEGER inicio, fim, freq;
  double tempo_total;

  // Abrir arquivo de entrada
  entrada = fopen(arquivo_entrada, "rb");
  if (entrada == NULL)
  {
    printf("Erro ao abrir o arquivo de entrada.\n");
    return -1;
  }

  // Abrir arquivo de saída
  saida = fopen(arquivo_saida, "wb");
  if (saida == NULL)
  {
    printf("Erro ao abrir o arquivo de saída.\n");
    fclose(entrada);
    return -1;
  }

  // Ler o cabeçalho
  bytes_lidos = fread(cabecalho, 1, CABECALHO_SIZE, entrada);
  if (bytes_lidos != CABECALHO_SIZE)
  {
    printf("Erro ao ler o cabeçalho da imagem.\n");
    fclose(entrada);
    fclose(saida);
    return -1;
  }

  // Copiar o cabeçalho para o arquivo de saída
  fwrite(cabecalho, 1, CABECALHO_SIZE, saida);

  // Obter frequência do contador
  QueryPerformanceFrequency(&freq);

  // Iniciar contagem do tempo
  QueryPerformanceCounter(&inicio);

  // Processar cada pixel
  while (fread(pixel, 1, 3, entrada) == 3)
  {
    // Ajustar o brilho de cada componente RGB
    for (int i = 0; i < 3; i++)
    {
      int novo_valor = pixel[i] + valor_brilho;
      if (novo_valor < 0)
        novo_valor = 0;
      if (novo_valor > 255)
        novo_valor = 255;
      pixel[i] = (unsigned char)novo_valor;
    }
    fwrite(pixel, 1, 3, saida);
  }

  // Finalizar contagem do tempo
  QueryPerformanceCounter(&fim);
  tempo_total = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart;

  // Fechar os arquivos
  fclose(entrada);
  fclose(saida);

  return tempo_total;
}

// Função que implementa a versão com MMX
double processar_com_mmx(const char *arquivo_entrada, const char *arquivo_saida, int valor_brilho)
{
  FILE *entrada, *saida;
  unsigned char cabecalho[CABECALHO_SIZE];
  unsigned char *buffer_entrada, *buffer_saida;
  int bytes_lidos;
  LARGE_INTEGER inicio, fim, freq;
  double tempo_total;
  long tamanho_arquivo;

  // Abrir arquivo de entrada
  entrada = fopen(arquivo_entrada, "rb");
  if (entrada == NULL)
  {
    printf("Erro ao abrir o arquivo de entrada.\n");
    return -1;
  }

  // Obter tamanho do arquivo
  fseek(entrada, 0, SEEK_END);
  tamanho_arquivo = ftell(entrada);
  fseek(entrada, 0, SEEK_SET);

  // Alocar buffers
  long tamanho_dados = tamanho_arquivo - CABECALHO_SIZE;
  buffer_entrada = (unsigned char *)_aligned_malloc(tamanho_dados, 16);
  buffer_saida = (unsigned char *)_aligned_malloc(tamanho_dados, 16);

  if (!buffer_entrada || !buffer_saida)
  {
    printf("Erro na alocação de memória.\n");
    if (buffer_entrada)
      _aligned_free(buffer_entrada);
    if (buffer_saida)
      _aligned_free(buffer_saida);
    fclose(entrada);
    return -1;
  }

  // Abrir arquivo de saída
  saida = fopen(arquivo_saida, "wb");
  if (saida == NULL)
  {
    printf("Erro ao abrir o arquivo de saída.\n");
    _aligned_free(buffer_entrada);
    _aligned_free(buffer_saida);
    fclose(entrada);
    return -1;
  }

  // Ler e copiar cabeçalho
  fread(cabecalho, 1, CABECALHO_SIZE, entrada);
  fwrite(cabecalho, 1, CABECALHO_SIZE, saida);

  // Ler todos os pixels para o buffer
  fread(buffer_entrada, 1, tamanho_dados, entrada);

  // Preparar valor de brilho para MMX
  __m64 valor_brilho_mmx = _mm_set1_pi8(valor_brilho);

  // Obter frequência do contador
  QueryPerformanceFrequency(&freq);

  // Iniciar contagem do tempo
  QueryPerformanceCounter(&inicio);

  // Processar pixels usando MMX
  for (long i = 0; i < tamanho_dados - 7; i += 8)
  {
    __m64 pixels = *(__m64 *)(&buffer_entrada[i]);
    __m64 resultado = _mm_adds_pu8(pixels, valor_brilho_mmx);
    *(__m64 *)(&buffer_saida[i]) = resultado;
  }

  // Processar bytes restantes
  for (long i = (tamanho_dados / 8) * 8; i < tamanho_dados; i++)
  {
    int novo_valor = buffer_entrada[i] + valor_brilho;
    if (novo_valor > 255)
      novo_valor = 255;
    if (novo_valor < 0)
      novo_valor = 0;
    buffer_saida[i] = (unsigned char)novo_valor;
  }

  // Limpar estado MMX
  _mm_empty();

  // Finalizar contagem do tempo
  QueryPerformanceCounter(&fim);
  tempo_total = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart;

  // Escrever pixels processados
  fwrite(buffer_saida, 1, tamanho_dados, saida);

  // Liberar recursos
  _aligned_free(buffer_entrada);
  _aligned_free(buffer_saida);
  fclose(entrada);
  fclose(saida);

  return tempo_total;
}

int main()
{
  int valor_brilho;
  double tempo_sem_mmx = 0, tempo_com_mmx = 0;
  double tempo_medio_sem_mmx = 0, tempo_medio_com_mmx = 0;
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  printf("Frequência do contador de performance: %lld Hz\n", freq.QuadPart);

  // Solicitar valor de brilho
  do
  {
    printf("Digite o valor de brilho (-100 a 100): ");
    scanf("%d", &valor_brilho);
  } while (valor_brilho < -100 || valor_brilho > 100);

  printf("\nExecutando %d testes para cada versão...\n", NUM_TESTES);

  // Executar testes
  for (int i = 0; i < NUM_TESTES; i++)
  {
    tempo_sem_mmx = processar_sem_mmx("camera200.bmp", "temp_sem_mmx.bmp", valor_brilho);
    tempo_com_mmx = processar_com_mmx("camera200.bmp", "temp_com_mmx.bmp", valor_brilho);

    tempo_medio_sem_mmx += tempo_sem_mmx;
    tempo_medio_com_mmx += tempo_com_mmx;

    if ((i + 1) % 10 == 0)
    {
      printf("Teste %d/%d concluído\n", i + 1, NUM_TESTES);
    }
  }

  // Calcular médias
  tempo_medio_sem_mmx /= NUM_TESTES;
  tempo_medio_com_mmx /= NUM_TESTES;

  // Exibir resultados
  printf("\nResultados após %d testes:\n", NUM_TESTES);
  printf("Versão sem MMX: %.6f segundos (média)\n", tempo_medio_sem_mmx);
  printf("Versão com MMX: %.6f segundos (média)\n", tempo_medio_com_mmx);
  printf("Diferença: %.6f segundos\n", tempo_medio_sem_mmx - tempo_medio_com_mmx);
  printf("Melhoria de desempenho: %.2f%%\n",
         ((tempo_medio_sem_mmx - tempo_medio_com_mmx) / tempo_medio_sem_mmx) * 100);

  // Remover arquivos temporários
  remove("temp_sem_mmx.bmp");
  remove("temp_com_mmx.bmp");

  return 0;
}
