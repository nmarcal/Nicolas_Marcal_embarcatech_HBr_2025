# Monitor da temperatura interna da MCU - RP2040 Thermometer

## Objetivo do projeto

Faça um programa em C para ler a temperatura interna do RP2040. Converta a leitura do ADC em um valor em ºC.

---

## Lógica

 Sistema funciona como um termômetro digital, no qual o sensor interno do RP2040 mede a temperatura e exibe o valor no display OLED. Para cada medição:
 
Leitura:
- O ADC (Conversor Analógico-Digital) lê o valor bruto do sensor de temperatura.

Processamento:
- Converte o valor ADC para tensão (0-3.3V);
- Aplica fórmula de calibração do RP2040 para obter a temperatura em Celsius;
- Arredonda o valor para um número inteiro antes da exibição.

Feedback:
- Terminal Serial: Exibe a temperatura com uma casa decimal (para debug);
- Display OLED: Mostra apenas o valor inteiro (ex: "Temp 27 C").

### Baseado no Exemplo 0 do Ebook [Conversores A/D](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/55325bc779a6c883225aefc7e85041756fc25491/Projetos/RP2040_Thermometer/docs/24.37M2%20%E2%80%93%20U4C8O123E%20-%20Ebook.pdf) do curso Embarcatech Fase 1 - HBR.

---

## Componentes e Pinagens

![componentes_RP2040_thermometer](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/1cad82778a3d4be7664d2af7ff3eb444e6ee64c8/Projetos/RP2040_Thermometer/assets/lista_componentes_RP2040_thermometer.jpg)

---

## Imagens

<img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/1cad82778a3d4be7664d2af7ff3eb444e6ee64c8/Projetos/RP2040_Thermometer/assets/imagem_1_RP2040_thermometer.jpg?raw=true" alt="imagem_1" width="175" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/1cad82778a3d4be7664d2af7ff3eb444e6ee64c8/Projetos/RP2040_Thermometer/assets/imagem_2_RP2040_thermometer.jpg?raw=true" alt="imagem_2" width="175" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/1cad82778a3d4be7664d2af7ff3eb444e6ee64c8/Projetos/RP2040_Thermometer/assets/imagem_3_RP2040_thermometer.jpg?raw=true" alt="imagem_3" width="175" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/1cad82778a3d4be7664d2af7ff3eb444e6ee64c8/Projetos/RP2040_Thermometer/assets/imagem_4_RP2040_thermometer.jpg?raw=true" alt="imagem_4" width="175" height="200">


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
