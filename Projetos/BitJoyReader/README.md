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

![componentes_bitjoyreader](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/baa711a372729c03f74e5ff6cd14b28df2069e3f/Projetos/BitJoyReader/assets/componentes_bitjoyreader.jpg)

---

## Imagens e Vídeo

<img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/main/Projetos/Genius_Cognitive_Therapeutic/assets/amarelo.jpg?raw=true" alt="amarelo" width="150" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/main/Projetos/Genius_Cognitive_Therapeutic/assets/azul.jpg?raw=true" alt="azul" width="150" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/main/Projetos/Genius_Cognitive_Therapeutic/assets/verde.jpg?raw=true" alt="verde" width="150" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/main/Projetos/Genius_Cognitive_Therapeutic/assets/magenta.jpg?raw=true" alt="magenta" width="150" height="200">

Vídeo do sistema em uso: [Projeto Final - Unidade 7 - Embarcatech](https://www.youtube.com/watch?v=RCcmscVKlQI)



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
