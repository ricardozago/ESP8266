# Guia inicial do ESP8266

O ESP8266 é um microcontrolador com grande capacidade de processamento (80 MHz de processador com 96 KB de RAM para dados), com suporte a redes Wifi. [Seu artigo na Wikipédia](https://en.wikipedia.org/wiki/ESP8266) é um bom ponto de partida para entender melhor. Ele está disponível em vários empacotamentos, onde a versão chamada de ESP-01 pode ser utilizada como um chip para fornecer acesso Wifi para o Arduino (ironicamente possuindo muito mais capacidade de processamento que o próprio Arduino Uno R3). O ESP8266 sozinho pode ser utilizado no lugar do Arduino utilizando a mesma IDE para processamento, no entanto nosso foco aqui é utilizá-lo como um periférico no Arduino.

Pode ser necessário atualizar o Firmware no ESP-01. A versão mais atual (para a versão com 512 KB, 4 Mb de memória ROM, esp8266_nonos_sdk_v2.0.0_16_08_10) pode ser encontrada na pasta do Github ou [aqui](http://espressif.com/en/support/download/sdks-demos). Aparentemente a versão 2.1 não é compatível com a versão de 4 Mb de memória ROM. Os arquivos para atualização do Firmware eagle.flash.bin e eagle.irom0text.bin estarão no arquivo compactado na pasta bin/at/noboot. Também serão utilizados os arquivos esp_init_data_default.bin e blank.bin na pasta /bin. Para facilitar eles estão na pasta [Firmware AT 4 Mb](https://github.com/ricardozago/ESP8266/tree/master/Firmware%20AT%204%20Mb) deste repositório.

Para gravar o firmware podem ser utilizados diversos softwares. Existe uma ferramenta muito rica escrita em Python chamada [esptool.py](https://github.com/espressif/esptool), no entanto usaremos o gravador (flash_download_tools_v3.4.9.2_0), disponível na pasta ou em seu [site oficial](http://www.espressif.com/en/support/download/other-tools). Para o ESP entrar no modo de gravação da imagem é necessário iniciá-lo com a porta GPIO0 aterrada. Também é possível colocar o reset no terra, aterrar o GPIO0, remover o reset do terra e posteriormente o GPIO0.

O [site do Filipeflop](http://blog.filipeflop.com/wireless/upgrade-de-firmware-do-modulo-esp8266.html) demonstra a montagem do circuito necessária. Também existe um projeto de um shield para o Arduino Uno R3 para facilitar a gravação, disponível [nesta pasta](https://github.com/ricardozago/ESP8266/tree/master/ESP-01%20Firmware%20Flasher), uma imagem da placa é mostrada abaixo.

![Gravador Placa](https://github.com/ricardozago/ESP8266/blob/master/ESP-01%20Firmware%20Flasher/Fabrica%C3%A7%C3%A3o/Top.png)

Para realizar o microcontrolador Atmel do Arduino não interferir na gravação do Firmware é necessário enviar um Sketch vazio para o Arduino. Como o padrão:

    void setup() {
      // put your setup code here, to run once:

    }

    void loop() {
      // put your main code here, to run repeatedly:

    }

As configurações do gravador de ROM podem ser as mesmas da imagem abaixo:

![Gravador](https://github.com/ricardozago/ESP8266/blob/master/Imagens/Gravador_ESP.png)

# Ainda na interface do gravador

O baudrate padrão do ESP é 115200, pode-se utilizar um software de terminal para comunicação, como o [Termite](https://www.compuphase.com/software_termite.htm). Entre nas configurações do termite e em Transmitted text marque a opção:

    Append CR-LF
    
Esta opção adiciona um retorno de cursor e uma nova linha ao comando, necessário para o ESP entender que você acabou de enviar o comando. Para verificar a comunicação com o terminal envia o comando:

    AT

O ESP deverá responder com:

    OK

O Arduino Uno R3, possui apenas uma porta de comunicação UART, que é conectada do chip FTDI para conversão para USB. O ESP pode ser conectado nessa mesma interface, mas isso acarreta problemas de comunicação e dificuldade de programação, já que você terá 3 sistemas "conversando" em uma interface onde apenas deveriam existir 2 sistemas. Uma solução é utilizar uma biblioteca que transforma um GPIO qualquer do Arquino em porta serial, o que acarreta problemas de comunicação em baud-rates elevados, portanto é necessário dominuir o baudrate da interface UART do ESP. O comando abaixo, em um firmware atualizado faz isso:

    AT+UART_DEF=19200,8,1,0,0
    
Agora você já está pronto para conectar o ESP diretamente no Arduino. Utilize o Shield, cujo projeto está disponível na [pasta](https://github.com/ricardozago/ESP8266/tree/master/Arduino%20Shield).

![Shield com ESP](https://github.com/ricardozago/ESP8266/blob/master/Arduino%20Shield/Fabricar/Top.jpg)
![Shield fabricado](https://github.com/ricardozago/ESP8266/blob/master/Imagens/PCB.jpg)
![Shield fabricado com ESP](https://github.com/ricardozago/ESP8266/blob/master/Imagens/PCBA.jpg)

O Sketch [disponível aqui](https://github.com/ricardozago/ESP8266/tree/master/ESP-Espelho) copia os comandos enviados via terminal para o Arduino para o ESP e vice-versa. Isto permite conversar diretamente com o ESP e enteder melhor o seu funcionamento.

Inicialmente envie o comando:

    AT+GMR

A versão do firmware será exibida, por exemplo:

    AT version:1.3.0.0(Jul 14 2016 18:54:01)
    SDK version:2.0.0(656edbf)
    compile time:Jul 19 2016 18:43:55
    OK

Utilize o comando abaixo para verificar em qual modo de operação o ESP está funcionando.

    AT+CWMODE?

Os possíveis modos de operação são:

1 = Cliente
2 = Ponto de acesso
3 = Cliente + Ponto de acesso

O comando abaixo passa para o modo cliente:

    AT+CWMODE=1
    
É possível fazer uma busca nas redes disponíveis com:

    AT+CWLAP
    
Ele retornará uma lista de redes disponíveis. Por exemplo:

    +CWLAP:(5,"eduroam",-86,"d8:c7:c8:78:19:02",1,6,0)
    +CWLAP:(0,"Unicamp-Configuracao",-64,"d8:c7:c8:78:18:d0",1,15,0)

O comando abaixo se conecta a alguma das redes disponíveis:

    AT+CWJAP="ssid","pass"
    
Mostra o IP:

    AT+CIFSR
    
O abaixo se desconecta:

    AT+CWQAP
    
Lembrando, as configurações são salvas. Quando você desliga e liga o ESP, ele continua com as mesmas configurações anteriores, como o baud rate e a última rede conectada.

[Este tutorial do Filipeflop](http://blog.filipeflop.com/wireless/esp8266-arduino-tutorial.html) é um bom ponto de partida para criação de um simples webserver. Apenas alterei o modo de conexão, onde no Filipeflop é necessário um roteador para realizar a conexão, enquanto no meu o próprio ESP é o roteador e os dispositivos se conectam nele. Minha versão modificada está disponível [aqui](https://github.com/ricardozago/ESP8266/tree/master/Arduino%20%2B%20ESP%20-%20p%C3%A1gina%20WEB/Ler_Portas).

Para [alterar o nome da rede](https://github.com/espressif/ESP8266_AT/wiki/CWSAP) no modo AP utilize o comando:

    AT+CWSAP="nome_da_rede","senha",chl,ecn
    
onde se a ecn  é igual a 0 a criptografia está desabilitada. Caso queira habilitar: WEP (1), WPA-PSK (2), WPA2-PSK (3) e WPA/WPA2-PSK (4). Já chl é o canal que a rede é criada.

[Esta página](http://allaboutee.com/2015/01/02/esp8266-arduino-led-control-from-webpage/) possui um interessante exemplo para atuar utilizando uma página Web com o Arduino+ESP. Também disponível [neste repositório](https://github.com/ricardozago/ESP8266/tree/master/Arduino%20%2B%20ESP%20-%20p%C3%A1gina%20WEB/Atuar_IO).

# Observações

Todos os layouts de placa desta pasta utilizam o software [Kicad](http://kicad-pcb.org/), a placas foram desenvolvida utilizando como modelo o Shield para arduino disponível como exemplo do Kicad.

Seu layout foi feito a pedido do professor [Fabiano Fruett](http://www.dsif.fee.unicamp.br/~fabiano/) para uso em disciplinas na Faculdade de Engenharia Elétrica e de Computação (FEEC) da Unicamp.
