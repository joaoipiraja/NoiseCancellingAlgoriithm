# Arduino Simulation

## Processo
O algoritmo de início não estava rodando no arduino. Fizemos algumas alterações

- Definimos variáveis globais para ver se os vetores iriam exceder a memória SRAM e evitar overflow em execução
- Tiramos todas as referências de ponteiro
- Usamos a interface serial da forma mais simples possíveis `Serial.parsefloat` e `Serial.println(output[i], 7)`
- Diminuir o comprimento do filtro `FILTER_LENGHT` de 256 para 100
- Usar o Canal Serial para receber e enviar dados
    - Limitar a saída da 7 casas decimais 
    - Usar Pyserial e dividir a entrada de 1000 em pequenos chucks de 70 para serem processados aos poucos 
    - Comparamos a duas saídas com base no error entre as respostas 
    

### Problemas
- Estávamos querendo aumentar a precisão para mais de 7 casas decimais. Contudo, o vetor de caracteres recebidos e a função `atof` não estavam atrapalhando a precisão
- Um comprimento muito pequeno atrapalha a perfomance do filtro
- Overflow dos vetores de entrada e saída  durante a execução e ficava um ruído bem aparente no ínicio da saída
