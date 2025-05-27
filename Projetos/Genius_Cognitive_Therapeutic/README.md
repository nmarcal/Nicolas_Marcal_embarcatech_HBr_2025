# Sistema de Treinamento Cognitivo e Reflexos

## Objetivo do projeto

Desenvolver um sistema interativo que combina elementos visuais e sonoros para 
estimular a memória, os reflexos e a coordenação motora. A proposta é criar uma 
ferramenta acessível e versátil, voltada para o desenvolvimento cognitivo de usuários, 
como idosos ou pessoas em reabilitação. O sistema oferece partidas ajustáveis de 1 a 
10 rodadas, promovendo concentração e engajamento, e proporcionando uma 
experiência personalizada e eficaz para atender às necessidades individuais dos 
usuários.

---

## Lógica

O sistema funciona como um jogo no estilo "Genius", no qual o usuário deve 
observar as sequências de cores emitidas pelo LED RGB e reproduzi-las usando o 
joystick e os botões integrados à placa. Para cada rodada: 
- Uma sequência de cores é exibida e sons associados são emitidos pelo 
buzzer.
- O usuário reproduz a sequência. Se acertar, avança para a próxima rodada; 
se errar, o jogo reinicia.
- A dificuldade aumenta conforme o número de rodadas escolhidas. Ao 
completar as rodadas estipuladas o sistema parabeniza o usuário.

---

## Componentes e Pinagens

![lista_componentes1](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/main/Projetos/Genius_Cognitive_Therapeutic/assets/lista_componentes1.jpg?raw=true)
![lista_componentes2](https://github.com/nmarcal/Nicolas_Marcal_embarcatech_HBr_2025/blob/main/Projetos/Genius_Cognitive_Therapeutic/assets/lista_componentes2.jpg?raw=true)

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
5. A Pico reiniciará automaticamente e começará a executar o código;
6. A mensagem "Aperte o botão A" aparecerá no display Oled para iniciar o sistema.
---

## Resultados

Os testes realizados comprovaram que o sistema é altamente confiável e atende 
plenamente aos objetivos propostos. 

  • O jogo operou de forma consistente, gerando sequências aleatórias 
    corretas e capturando as entradas do jogador sem apresentar erros ou 
    falhas. 
    
  • Nenhum bug crítico foi identificado durante as fases de teste.

O projeto demonstrou um grande potencial para 
aplicações práticas, tanto no âmbito do entretenimento quanto no treinamento e 
reabilitação cognitiva. O sistema cumpre seus objetivos de forma eficaz e 
apresenta uma base sólida para futuras melhorias ou adaptações.

---

## Licença

GNU License - GNU GPL-3.0
