Implemente uma função que busque um determinado caracter dentro de um arquivo de teste e informe quantas ocorrências desta palavra ele encontrou.

A seguir gere uma versão multi-thread desta solução onde diferentes tarefas de procura são executadas em paralelo, cada tarefa procurando em uma porção distinta do arquivo pelo caacter buscada.

Para tanto usar a biblioteca pthreads (posix threads). Observar que deve ser configurado no codeblocks pela inclusão da biblioteca pthreds conforme ilustrado em:

https://askubuntu.com/questions/568068/multithreading-in-codeblocks

Depois compare suas soluções com o comando de sistema find:

Exemplo:

find /c "pa" arquivo.txt

------ ARQUIVO.TXT: 130

Usar comando system() do C para executar o find dentro do codeblocks.

Gerar relatório em PDF com resultados e comentários. Informar qual sua CPU (numero de cores e threads da mesma)

Tabela a ser inclusa

| Métrica                         | Valor |
| ------------------------------- | ----- |
| Tamanho (bytes)                 |       |
| Tempo de execução (1000 buscas) |       |
| Solução de busca mono-thread    |       |
| Solução de busca 2 threads      |       |
| Solução de busca 4 threads      |       |
| Usando comando find             |       |

Indicar nome no cabeçalho do arquivo.
