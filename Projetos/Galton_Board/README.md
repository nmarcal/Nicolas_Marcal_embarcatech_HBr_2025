# Galton Board - Projetos de Sistemas Embarcados - EmbarcaTech 2025

Autor: **Nícolas Augusto Pichinelli Marçal**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, Maio de 2025

---

## Objetivo do projeto

Simular uma Galton Board no display OLED da BitDogLab, usando uma abordagem de Digital Twin. Isso significa construir um modelo computacional que se comporte de maneira semelhante ao dispositivo físico, permitindo observar a distribuição das "bolas" ao final do tabuleiro — e compará-las com a teoria da distribuição normal.

---

## Lógica 

O sistema simula uma Máquina de Galton digital, onde bolas virtuais caem através de uma matriz de pinos e se acumulam em canaletas na base. A dinâmica do funcionamento segue este fluxo:

### Inicialização:

- 100 bolas são liberadas sequencialmente do topo do display OLED

- Cada bola desce aleatoriamente, colidindo com pinos e outras bolas

### Processo de Queda:

- As bolas sofrem influência de física simulada (gravidade, colisões, aleatoriedade)

- Ao atingir a base, são contabilizadas em uma das 5 canaletas

### Visualização dos Resultados:

- Modo Simulação: mostra o movimento das bolas em tempo real com contador

- Modo Histograma (acionado pelo botão B): exibe a distribuição estatística final, com barras verticais que representam a quantidade de bolas por canaleta e os valores numéricos exatos abaixo de cada coluna

### Finalização:

- Quando todas as bolas são coletadas, o sistema exibe "Completo!"

- O histograma permanece disponível para análise da distribuição binomial característica

A complexidade aumenta naturalmente conforme mais bolas interagem simultaneamente na tela (até 10 bolas concorrentes). O sistema demonstra visualmente os princípios de probabilidade e distribuição estatística

---

## Componentes:

![componentes_Galton_Board](https://github.com/EmbarcaTech-2025/lab01_galton_board-nmarcal/blob/061cf471ceaa03de75cc0a1d5c6e7479a0b51f4d/assets/lista_componentes1.jpg?raw=true)

---

## Imagens e Vídeos:

<img src="https://github.com/EmbarcaTech-2025/lab01_galton_board-nmarcal/blob/d2bcfeadcc0300a9fe0e928f7fc61f4a918bde31/assets/Galton_Board_1.jpg?raw=true" alt="Imagem_1" width="150" height="200"> <img src="https://github.com/EmbarcaTech-2025/lab01_galton_board-nmarcal/blob/d2bcfeadcc0300a9fe0e928f7fc61f4a918bde31/assets/Galton_Board_2.jpg?raw=true" alt="Imagem_2" width="150" height="200"> <img src="https://github.com/EmbarcaTech-2025/lab01_galton_board-nmarcal/blob/d2bcfeadcc0300a9fe0e928f7fc61f4a918bde31/assets/Galton_Board_3.jpg?raw=true" alt="Imagem_3" width="150" height="200">

Vídeo do sistema em uso: [Galton Board - Semana 8 - Embarcatech 2025 2º Fase](https://www.youtube.com/watch?v=iCuJRc8VhTc&feature=youtu.be)


---
## Execução:

1. Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make);
3. Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);
4. Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2);
5. A Pico reiniciará automaticamente e começará a executar o código;

---

## 📜 Licença
MIT License - MIT GPL-3.0.

---
### sdkVersion 1.5.1

