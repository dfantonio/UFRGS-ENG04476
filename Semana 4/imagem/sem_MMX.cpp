#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CABECALHO_SIZE 1078

int main()
{
  FILE *entrada, *saida;
  unsigned char cabecalho[CABECALHO_SIZE];
  unsigned char pixel[3];
  int valor_brilho;
  int bytes_lidos;
  clock_t inicio, fim;
  double tempo_total;

  // Abrir arquivo de entrada
  entrada = fopen("camera200.bmp", "rb");
  if (entrada == NULL)
  {
    printf("Erro ao abrir o arquivo de entrada.\n");
    return 1;
  }

  // Abrir arquivo de sa�da
  saida = fopen("camera200_brilho.bmp", "wb");
  if (saida == NULL)
  {
    printf("Erro ao abrir o arquivo de sa�da.\n");
    fclose(entrada);
    return 1;
  }

  // Ler o cabe�alho
  bytes_lidos = fread(cabecalho, 1, CABECALHO_SIZE, entrada);
  if (bytes_lidos != CABECALHO_SIZE)
  {
    printf("Erro ao ler o cabe�alho da imagem.\n");
    fclose(entrada);
    fclose(saida);
    return 1;
  }

  // Copiar o cabe�alho para o arquivo de sa�da
  fwrite(cabecalho, 1, CABECALHO_SIZE, saida);

  // Solicitar o valor de brilho ao usu�rio
  do
  {
    printf("Digite o valor de brilho (-100 a 100): ");
    scanf("%d", &valor_brilho);
  } while (valor_brilho < -100 || valor_brilho > 100);

  // Iniciar contagem do tempo
  inicio = clock();

  // Processar cada pixel
  while (fread(pixel, 1, 3, entrada) == 3)
  {
    // Ajustar o brilho de cada componente RGB
    for (int i = 0; i < 3; i++)
    {
      int novo_valor = pixel[i] + valor_brilho;

      // Garantir que o valor fique entre 0 e 255
      if (novo_valor < 0)
        novo_valor = 0;
      if (novo_valor > 255)
        novo_valor = 255;

      pixel[i] = (unsigned char)novo_valor;
    }

    // Escrever o pixel ajustado no arquivo de sa�da
    fwrite(pixel, 1, 3, saida);
  }

  // Finalizar contagem do tempo
  fim = clock();
  tempo_total = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

  // Fechar os arquivos
  fclose(entrada);
  fclose(saida);

  printf("Imagem processada com sucesso! Arquivo de sa�da: camera200_brilho.bmp\n");
  printf("Tempo de processamento: %.4f segundos\n", tempo_total);
  return 0;
}
