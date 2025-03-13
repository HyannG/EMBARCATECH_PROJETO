# EMBARCATECH_PROJETO

Este projeto implementa um MP3 Player simples, utilizando o Raspberry Pi Pico W. Ele reproduz notas musicais pelo Buzzer Passivo da placa e é capaz de:

- Reproduzir 4 músicas diferentes: Tema do Darth Vader, Tema da Pantera Cor de Rosa, Tema do Poderoso Chefão e Never Gonna Give You Up.
- Enquanto a música toca, o LED brilha em uma cor referente à música.
- Aumentar o volume.
- Diminuir o volume.
- Parar a música.

---

## Funcionalidades

- Tocar quatro faixas diferentes.
- O usuário escolhe qual música irá tocar pela direção do joystick.
- Aumentar volume.
- Diminuir volume.
- Interromper a reprodução.
- O LED RGB brilha em uma cor dependendo da música selecionada.

---

## Hardware Utilizado

- Raspberry Pi Pico W
- Joystick
- Buzzer Passivo
- Botão A
- Botão B
- LED RGB

---

## Software Utilizado

- Raspberry Pi Pico SDK
- C
- Visual Studio Code
- hardware/adc
- hardware/pwm
- hardware/gpio
- pico/time

---

## Manual de Utilização

### Controles e Funcionalidades

O MP3 Player implementado no Raspberry Pi Pico W pode ser controlado por meio de um joystick e dois botões. Abaixo estão as instruções detalhadas para interagir com o dispositivo:

#### Botões

- **Botão A**  
  - **Ação**: Diminuir o volume.  
  - **Uso**: Pressione o botão A uma vez para reduzir o volume da música em reprodução.  

- **Botão B**  
  - **Ação**: Aumentar o volume.  
  - **Uso**: Pressione o botão B uma vez para aumentar o volume da música em reprodução.  

- **Botão A + Botão B (pressionados simultaneamente)**  
  - **Ação**: Parar a reprodução da música.  
  - **Uso**: Pressione ambos os botões ao mesmo tempo para interromper a música que está tocando.  

#### Joystick

O joystick é utilizado para selecionar a música a ser reproduzida. Cada direção do joystick corresponde a uma música específica:

- **Joystick para cima**  
  - **Música**: *Never Gonna Give You Up*.  
  - **LED**: Brilha em verde.  

- **Joystick para baixo**  
  - **Música**: *Tema da Pantera Cor de Rosa*.  
  - **LED**: Brilha em rosa.  

- **Joystick para a esquerda**  
  - **Música**: *Tema do Poderoso Chefão*.  
  - **LED**: Brilha em branco.  

- **Joystick para a direita**  
  - **Música**: *Tema do Darth Vader*.  
  - **LED**: Brilha em vermelho.  

### LED RGB

O LED RGB indica qual música está sendo reproduzida por meio de cores específicas. Cada música selecionada pelo joystick faz com que o LED brilhe em uma cor correspondente.

---

### Exemplo de Uso

1. **Selecionar uma música**: Mova o joystick na direção desejada para escolher uma música. O LED RGB acenderá na cor correspondente à música selecionada.
2. **Ajustar o volume**: Utilize os botões A e B para diminuir ou aumentar o volume, respectivamente.
3. **Parar a música**: Pressione os botões A e B simultaneamente para interromper a reprodução.

---

### Diagrama de Conexões

| Componente | Pino no Raspberry Pi Pico W |
|------------|-----------------------------|
| Buzzer | GPIO 21 |
| LED RGB (R) | GPIO 13 |
| LED RGB (G) | GPIO 11 |
| LED RGB (B) | GPIO 12 |
| Joystick (X) | ADC0 (GPIO 26) |
| Joystick (Y) | ADC1 (GPIO 27) |
| Botão A | GPIO 5 |
| Botão B | GPIO 6 |

---

# Estrutura do Projeto

- **PROJETO_EMBARCADO/**  
  - **vscode/** # Configurações do Visual Studio Code  
  - **build/** # Pasta de builds gerados pelo CMake  
  - **CMakeLists.txt** # Configuração do CMake  
  - **musicas.h** # Header armazenando as músicas  
  - **notas.h** # Header com definições de notas musicais  
  - **PROJETO_EMBARCADO.c** # Código principal do projeto  
  - **README.md** # Documentação do projeto  

## Fluxograma para utilização da placa

![Fluxograma](https://github.com/user-attachments/assets/507f6d8f-365f-49bd-a7e2-6e2554eded47)

## LINK DO GITHUB

[Repositório no GitHub](https://github.com/HyannG/EMBARCATECH_PROJETO)

