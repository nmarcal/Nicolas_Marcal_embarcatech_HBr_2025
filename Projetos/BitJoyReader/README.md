# Leitor de Sinais Analógicos do Joystick - BitJoyReader

## Objetivo do projeto

Faça um programa em C para ler os valores convertidos digitalmente do joystick da BitDogLab. Os valores podem ser mostrados no terminal ou então no display OLED.

---

## Lógica

 sistema funciona como um leitor de comandos analógicos, no qual o usuário move o joystick e observa os valores correspondentes no display OLED. Para cada movimento:
- Leitura: Os eixos X e Y são lidos continuamente pelo ADC (0 a 4095);
- Processamento: Uma zona morta (±100) filtra pequenas variações próximas ao centro (2048);
- Feedback: Valores fora da zona morta são exibidos no OLED.

---

## Componentes e Pinagens

![componentes_bitjoyreader](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/28053f7e21764f6adc83a7eaf08bf9c1da6132eb/Projetos/BitJoyReader/assets/lista_componentes_bitjoyreader.jpg)

---

## Imagens

<img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/28053f7e21764f6adc83a7eaf08bf9c1da6132eb/Projetos/BitJoyReader/assets/imagem_1_bitjoyreader.jpg?raw=true" alt="imagem_1" width="175" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/28053f7e21764f6adc83a7eaf08bf9c1da6132eb/Projetos/BitJoyReader/assets/imagem_2_bitjoyreader.jpg?raw=true" alt="imagem_2" width="175" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/28053f7e21764f6adc83a7eaf08bf9c1da6132eb/Projetos/BitJoyReader/assets/imagem_3_bitjoyreader.jpg?raw=true" alt="imagem_3" width="175" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/28053f7e21764f6adc83a7eaf08bf9c1da6132eb/Projetos/BitJoyReader/assets/imagem_4_bitjoyreader.jpg?raw=true" alt="imagem_4" width="175" height="200">


---

## Execução

1. Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make);
3. Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);
4. Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2);
5. A Pico reiniciará automaticamente e começará a executar o código.
   
---

## Licença

GNU License - GNU GPL-3.0
