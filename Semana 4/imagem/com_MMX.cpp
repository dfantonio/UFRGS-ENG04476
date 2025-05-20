#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mmintrin.h> // Para instruções MMX

#define CABECALHO_SIZE 1078
#define PIXELS_POR_VEZ 8 // Processamos 8 bytes por vez com MMX

int main()
{
  FILE *entrada, *saida;
  unsigned char cabecalho[CABECALHO_SIZE];
  unsigned char *buffer_entrada, *buffer_saida;
  int valor_brilho;
  int bytes_lidos;
  clock_t inicio, fim;
  double tempo_total;
  long tamanho_arquivo;

  // Abrir arquivo de entrada
  entrada = fopen("camera200.bmp", "rb");
  if (entrada == NULL)
  {
    printf("Erro ao abrir o arquivo de entrada.\n");
    return 1;
  }

  // Obter tamanho do arquivo
  fseek(entrada, 0, SEEK_END);
  tamanho_arquivo = ftell(entrada);
  fseek(entrada, 0, SEEK_SET);

  // Alocar buffers
  long tamanho_dados = tamanho_arquivo - CABECALHO_SIZE;
  buffer_entrada = (unsigned char *)_aligned_malloc(tamanho_dados, 16); // Alinhamento para MMX
  buffer_saida = (unsigned char *)_aligned_malloc(tamanho_dados, 16);

  if (!buffer_entrada || !buffer_saida)
  {
    printf("Erro na alocação de memória.\n");
    if (buffer_entrada)
      _aligned_free(buffer_entrada);
    if (buffer_saida)
      _aligned_free(buffer_saida);
    fclose(entrada);
    return 1;
  }

  // Abrir arquivo de saída
  saida = fopen("camera200_brilho_mmx.bmp", "wb");
  if (saida == NULL)
  {
    printf("Erro ao abrir o arquivo de saída.\n");
    _aligned_free(buffer_entrada);
    _aligned_free(buffer_saida);
    fclose(entrada);
    return 1;
  }

  // Ler e copiar cabeçalho
  fread(cabecalho, 1, CABECALHO_SIZE, entrada);
  fwrite(cabecalho, 1, CABECALHO_SIZE, saida);

  // Ler todos os pixels para o buffer
  fread(buffer_entrada, 1, tamanho_dados, entrada);

  // Solicitar valor de brilho
  do
  {
    printf("Digite o valor de brilho (-100 a 100): ");
    scanf("%d", &valor_brilho);
  } while (valor_brilho < -100 || valor_brilho > 100);

  // Preparar valor de brilho para MMX (replicar para cada byte)
  __m64 valor_brilho_mmx = _mm_set1_pi8(valor_brilho);

  // Iniciar contagem do tempo
  inicio = clock();

  // Processar pixels usando MMX
  for (long i = 0; i < tamanho_dados - 7; i += 8)
  {
    // Carregar 8 bytes (pixels) de uma vez
    __m64 pixels = *(__m64 *)(&buffer_entrada[i]);

    // Adicionar brilho
    __m64 resultado = _mm_adds_pu8(pixels, valor_brilho_mmx);

    // Armazenar resultado
    *(__m64 *)(&buffer_saida[i]) = resultado;
  }

  // Processar bytes restantes (se houver) de forma convencional
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
  fim = clock();
  tempo_total = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

  // Escrever pixels processados no arquivo de saída
  fwrite(buffer_saida, 1, tamanho_dados, saida);

  // Liberar recursos
  _aligned_free(buffer_entrada);
  _aligned_free(buffer_saida);
  fclose(entrada);
  fclose(saida);

  printf("Imagem processada com sucesso! Arquivo de saída: camera200_brilho_mmx.bmp\n");
  printf("Tempo de processamento MMX: %.4f segundos\n", tempo_total);
  return 0;
}
