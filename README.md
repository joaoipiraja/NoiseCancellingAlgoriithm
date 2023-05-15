# Documentação para nlms_filter_with_lowpass
Esta é uma implementação do filtro NLMS com filtro passa-baixa em Python usando as bibliotecas NumPy, SciPy e Matplotlib. O filtro é usado para reduzir o ruído em um sinal de áudio.

# Teoria 

O Normalize Least Mean Square Filter (NLMS) é um algoritmo de filtro adaptativo usado para reduzir o ruído em sinais de áudio ou imagem. Ele funciona ajustando os coeficientes de um filtro digital de acordo com a diferença entre o sinal de entrada e o sinal de referência, que é o ruído a ser reduzido. O NLMS é um algoritmo de filtro adaptativo porque os coeficientes do filtro são atualizados continuamente com base nos dados de entrada, permitindo que ele se adapte às mudanças no sinal ao longo do tempo. A normalização é usada para evitar a instabilidade no caso de sinais de entrada com grande amplitude. O NLMS tem várias aplicações, como cancelamento de eco, redução de ruído de fala e reconhecimento de fala em ambientes ruidosos.

# Como usar
O filtro é implementado na função nlms_filter_with_lowpass(x, d, L, mu, beta=1, pre_emphasis=0.75, cutoff_freq=0.35), onde os argumentos são:

- x: sinal de entrada (array unidimensional).
- d: sinal de referência (array unidimensional).
- L: comprimento do filtro.
- mu: parâmetro de tamanho de passo.
- beta (opcional): parâmetro de regularização (padrão = 1).
- pre_emphasis (opcional): fator de pré-ênfase (padrão = 0,75).
- cutoff_freq (opcional): frequência de corte para o filtro passa-baixa (padrão = 0,35).

Aqui está uma breve descrição de cada variável:

- x_filtered: sinal de entrada filtrado com um filtro passa-baixa de Butterworth de ordem 4 com frequência de corte cutoff_freq.
- x_pre_emph: sinal de entrada pré-ênfase com fator pre_emphasis.
- w: vetor de pesos do filtro (comprimento L).
- energy: estimativa de energia do sinal.
- buffer: buffer de atraso que armazena as amostras de entrada anteriores.
- y: sinal de saída do filtro.
- e: erro de estimação do filtro.


# Exemplo de uso
Um exemplo de uso pode ser encontrado no código Python acima, que aplica o filtro NLMS com filtro passa-baixa em um arquivo de áudio com ruído para reduzir o ruído. O sinal de referência é outro arquivo de áudio que contém apenas o ruído. O sinal de entrada é normalizado antes de ser passado para a função nlms_filter_with_lowpass. O sinal filtrado é normalizado novamente e salvo em um novo arquivo de áudio com o nome especificado. O código também plota os sinais de entrada, referência e filtrado usando a biblioteca Matplotlib.
