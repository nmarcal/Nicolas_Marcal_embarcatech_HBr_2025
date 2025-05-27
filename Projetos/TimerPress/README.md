# Contador decrescente com registro de eventos por interrupção - TimerPress

## Objetivo do projeto

Faça um programa, em linguagem C, que implemente um contador decrescente controlado por interrupção, com o seguinte comportamento:

Toda vez que o Botão A (GPIO5) for pressionado:
- O contador decrescente reinicia em 9 e o valor da contagem é mostrado no display OLED;
- O sistema entra em modo de contagem regressiva ativa, decrementando o contador de 1 em 1 a cada segundo até chegar em zero;
- Durante essa contagem (ou seja, de 9 até 0), o programa deve registrar quantas vezes o Botão B (GPIO6) foi pressionado. O valor deste registro de eventos de botão pressionado também deve ser mostrado no display OLED;
- Quando o contador atingir zero, o sistema congela e ignora temporariamente os cliques no Botão B (eles não devem ser acumulados fora do intervalo ativo).
  
O sistema permanece parado após a contagem, exibindo:
- O valor 0 no contador;
- A quantidade final de cliques no Botão B registrados durante o período de 9 segundo (contagem regressiva).
  
Somente ao pressionar novamente o Botão A, o processo todo se reinicia:
- O contador volta para 9;
- O número de cliques do Botão B é zerado;
- A contagem recomeça do início.

---

## Lógica

 Sistema funciona como um contador regressivo com registro de eventos, no qual o usuário inicia uma contagem e registra pressionamentos durante o intervalo. Para cada interação:

Inicialização:
- O botão A (GPIO 5) inicia a contagem regressiva de 9 a 0;
- O botão B (GPIO 6) registra pressionamentos durante a contagem ativa.

Processamento:
- Debounce de 50ms filtra ruídos nos botões;
- Timer de 1 segundo controla a contagem regressiva (decrementa counter);
- Quando counter = 0, desativa o registro de novos pressionamentos.

Feedback:
- Display OLED linha 1: Tempo restante;
- Display OLED linha 3: Total de pressionamentos em B.

---

## Componentes e Pinagens

![componentes_timerpress](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/0b4a39573739726ecc3754fe57346df71afe1b9c/Projetos/TimerPress/assets/lista_componentes_timerpress.jpg)

---

## Imagens

<img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/0b4a39573739726ecc3754fe57346df71afe1b9c/Projetos/TimerPress/assets/imagem_1_timerpress.jpg?raw=true" alt="imagem_1" width="175" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/0b4a39573739726ecc3754fe57346df71afe1b9c/Projetos/TimerPress/assets/imagem_2_timerpress.jpg?raw=true" alt="imagem_2" width="175" height="200"> <img src="https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/0b4a39573739726ecc3754fe57346df71afe1b9c/Projetos/TimerPress/assets/imagem_3_timerpress.jpg?raw=true" alt="imagem_3" width="175" height="200">

---

## Execução

1. Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make);
3. Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);
4. Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2);
5. A Pico reiniciará automaticamente e começará a executar o código;
6. A mensagem "Pressione A para iniciar" aparecerá no display Oled para iniciar o sistema.
   
---

## Licença

GNU License - GNU GPL-3.0
