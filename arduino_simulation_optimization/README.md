# Arduino Simulation Optimization

## Processo
- Medir para todas as plataformas utilizadas (PC-Linux e Arduino UNO)
   - Tempo de computação (DO ALGORITMO, excluindo partes de I/O, geração de dados de entrada, impressão etc)
   - Tamanho da memória de código utilizada pela aplicação (excluindo dados de entrada)
   - Tamanho da memória de dados utilizada pela aplicação 
   - Energia utilizada pela aplicação (Arduino UNO)
    [Procure fazer medições nas mesmas condições para as 2 plataformas]

- Otimizar na plataforma embarcada (Arduino UNO) 01 (UM) dos parâmetros medidos e fazer novo conjunto de medições.
- A otimização deve ser feita na parte do código que implementa o algoritmo da tarefa anterior. Nada que envolva (exclusivamente) dados de entrada ou visualização de resultados.
