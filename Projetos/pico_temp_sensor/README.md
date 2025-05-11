# Teste unitário - Pico Temp Sensor

## Objetivo do projeto

O ADC interno da Raspberry Pi Pico W pode ser usado para medir a temperatura através de um canal especial (sensor embutido). O valor lido do ADC (12 bits) deve ser convertido para graus Celsius usando a fórmula da documentação oficial:

![formula_temp](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/072a3584dd07730be622c119470bfb10fd323c66/Projetos/pico_temp_sensor/assets/formula_temp.jpg?raw=true)

Essa função converte a tensão lida do sensor de temperatura para Celsius, assumindo Vref = 3.3V e resolução de 12 bits.

### Tarefa:

- Implemente a função float adc_to_celsius(uint16_t adc_val);

- Escreva uma função de teste unitário que verifica se a função retorna o valor correto (com margem de erro) para uma leitura simulada de ADC. Sugere-se o uso da biblioteca Unity para o teste unitário.

- Use um teste com valor de ADC conhecido (ex.: para 0.706 V, a temperatura deve ser 27 °C).

---

## Componentes e Pinagens

![componentes_Pico_Temp_Sensor](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/072a3584dd07730be622c119470bfb10fd323c66/Projetos/pico_temp_sensor/assets/lista_componentes1.jpg)

---

## Imagens
<img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/bee01a61ab17e8d2342140f4021349eb7a25012a/Projetos/pico_temp_sensor/assets/imagem_pico_temp_sensor.jpg?raw=true" alt="imagem_1" width="175" height="200">

![imagem_2](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/c8d0d0c5562afcc2bb5f0c507f45693fdc69d5be/Projetos/pico_temp_sensor/assets/imagem_tests.jpg)

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

---

### sdkVersion 1.5.1

