# Pico Temp Sensor - Teste unitário

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

**Execução Simplificada**  

1. **Abra o projeto no VS Code**  
   - Certifique-se de ter o **MinGW** instalado (para compilar no Windows).  

2. **Compile o programa de testes**  
   - No terminal do VS Code (**Ctrl + `**), execute:  
     ```powershell
     gcc src/app/temperature_sensor.c src/tests/test_temperature.c src/include/unity/unity.c -I src/app -I src/include/unity -o tests.exe -lm
     ```

3. **Execute os testes**  
   - No mesmo terminal, rode:  
     ```powershell
     .\tests.exe
     ```  
   - Saída esperada (se tudo passar):  
     ```
     test_adc_conversion:PASS  
     test_adc_extremes:PASS  
     test_adc_intermediate:PASS  
     -----------------------  
     3 Tests 0 Failures 0 Ignored  
     OK  
     ```
---

## Licença

GNU License - GNU GPL-3.0

---

### sdkVersion 1.5.1

