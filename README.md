# Exemplo 1 - Simulador em Python

O primeiro exemplo consiste em utilizar um simulador desenvolvido em Python que trata de se conectar ao servidor onde corre o Nexeed MES (IP 193.137.172.20, porta 55965) e enviar periodicamente um novo evento do tipo <b>partProcessed</b>.
Cada vez que um novo XML é enviado, tem-se a geração aleatória de um conjunto de diferentes variáveis (valores simulados não traduzem de todo os valores reais das grandezas)


# Exemplo 2 - Simulador com um ESP

O segundo exemplo consiste no envio para o MES de informações reais, obtidas através da leitura de temperatura de um termopar TC74 ligado ao ESP. O termopar está constantemente a medir a temperatura, enviando-a por I2C para o ESP. 
O ESP, devido à dificuldade que existe em se conectar à rede da UA eduroam (do tipo WPA2 Enterprise) <b>deve estar ligado a um hotspot da rede da UA</b>. Após a ligação estabelecida é que tenta estabelecer a ligação ao servidor do MES. 
Com o documento XML devidamente criado no ESP, basta ler a temperatura obtida no termopar e substituir na localização correta do XML pelo novo valor, e envia-se para o MES o novo valor da temperatura.


# Descrição dos diversos ficheiros

Para o <b>Exemplo 1 - Simulador em Python</b>, tem-se:
<li> Diretório <a href = https://github.com/zemaria2000/MES/tree/master/xml><strong>xml</strong></a> - Contém os formatos corretos dos ficheiros XML para os eventos <b>partProcessed</b> e <b>dataUploadRequired</b>;</li>
<li> Ficheiro <a href = https://github.com/zemaria2000/MES/blob/master/config.json><strong>config.json</strong></a> - Configurações para o ficheiro Python relativas à ligação ao servidor do MES;</li>
<li> Outros dois ficheiros JSON - contém outros ficheiros de configuração com informações importantes para o código, nomeadamente relativas ao envio dos XMLs; </li>
<li> Ficheiro <a href = https://github.com/zemaria2000/MES/blob/master/main.py><strong>main.py</strong></a> - ficheiro executável Python com o simulador;
  
É de salientar que, para quem quiser executar o simulador no seu computador, deve fazer a instalação das bibliotecas necessárias
  
--------------------------------------------------------------------------------------------------------------------------------------------------------------
No que toca ao <b>Exemplo 2 - Simulador com um ESP</b>, tem-se no diretório <a href = https://github.com/zemaria2000/MES/tree/master/ESP_Tests><strong>ESP_Tests</strong></a>:
<li> <a href = https://github.com/zemaria2000/MES/tree/master/ESP_Tests/Trabalho_Gon_alo><strong>Trabalho_Gonçalo</strong></a> - contém um ficheiro Arduino que envia um XML sempre com os mesmos valores, que serviu apenas para fazer o teste da conexão entre o ESP e o MES;</li>
<li> <a href = https://github.com/zemaria2000/MES/tree/master/ESP_Tests/Send_TC74_to_MES><strong>Send_TC74_to_MES</strong></a> - contém um ficheiro em Arduino com o qual é possível enviar a leitura da temperatura obtida no TC74 para o MES, através de um evento do tipo partProcessed.</li>

O setup experimental foi baseado no trabalho que pode ser consultado <a href = https://github.com/ThomasGeor/esp32_tc74_temperature_sensor><strong>aqui</strong></a>.



## Créditos
Grande parte do código desenvolvido é da autoria do <a href = https://github.com/goncaloavmatos><strong>goncaloavmatos</strong></a> 
  
Eu apenas fiz alguns ajustes e adaptações do seu código.
