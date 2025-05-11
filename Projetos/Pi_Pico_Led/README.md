# Estruturação de código para arquitetura modular - Pi Pico Led

## Objetivo do projeto

- Crie um driver led_embutido.c que use diretamente a API cyw43_arch.

- Implemente um hal_led.c com a função hal_led_toggle() que abstraia o driver.

- Escreva um main.c simples no diretório app/ que apenas use a função da HAL para piscar o LED.


---

## Componentes e Pinagens

![componentes_Pi_Pico_Led](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/3ce7e2e6115050c1d91a3aea1d1f884df37e27f8/Projetos/Pi_Pico_Led/assets/lista_componentes1.jpg)

---

## Imagem

<img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/00f1b14e7eae53b05ce703bc1f2951ce1bb5da4e/Projetos/Pi_Pico_Led/assets/pi_pico_led_1.jpg?raw=true" alt="imagem_1" width="150" height="200">

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

