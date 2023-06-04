# Arduino Simulation


## Processo
O algoritmo de início não estava rodando no arduino. Fozemos algumas alterações

- Definimos variáveis globais para ver se os vetores iriam exceder a memória SRAM e evitar overflow em execução
- Usamos a interface serial para printar os dados e utilizamos `dtostrf` para formatar a saída para 7 casas decimais 
- Tirar todas as referências de ponteiro
- Diminuir o comprimento do filtro `L` de 256 para 200
- Usar o Canal Serial para receber e enviar dados
    - Limitar a saída da 7 casas decimais 
    - Usar Pyserial e dividir a entrada de 1000 em pequenos chucks de 5 para serem processados aos poucos 
    - 
    - Comparamos a duas saídas com base no error entre as respostas 
    

### Problemas
- Estávamos querendo aumentar a precisão para mais de 7 casas decimais. Contudo, o vetor de caracteres recebidos e a função `atof` não estavam atrapalhando a precisão. Então:
- Estávamos colocando o comprimento do filtro para 67 e isso comprometia o processamento do sinal
- Estava ocorrendo overflow dos vetores de entrada e saída  durante a execução e ficava um ruído bem aparente
