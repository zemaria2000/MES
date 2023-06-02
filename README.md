# Exemplo 1 - Simulador em Python

O primeiro exemplo consiste em utilizar um simulador desenvolvido em Python que trata de se conectar ao servidor onde corre o Nexeed MES (IP 193.137.172.20, porta 55965) e enviar periodicamente um novo evento do tipo <b>partProcessed</b>.
Cada vez que um novo XML é enviado, tem-se a geração aleatória de um conjunto de diferentes variáveis (valores simulados não traduzem de todo os valores reais das grandezas)


# Exemplo 2 - Simulador com um ESP

O segundo exemplo consiste no envio para o MES de informações reais, obtidas através da leitura de temperatura de um termopar TC74 ligado ao ESP. O termopar está constantemente a medir a temperatura, enviando-a por I2C para o ESP. 
O ESP, devido à dificuldade que existe em se conectar à rede da UA eduroam (do tipo WPA2 Enterprise) <b>deve estar ligado a um hotspot da rede da UA</b>. Após a ligação estabelecida é que tenta estabelecer a ligação ao servidor do MES. 
Com o documento XML devidamente criado no ESP, basta ler a temperatura obtida no termopar e substituir na localização correta do XML pelo novo valor, e envia-se para o MES o novo valor da temperatura.
