# EMBARCATECH_PROJETO

Este projeto implementa um MP3 Player simples, utilizando o Raspberry Pi Pico W. Ele reproduz notas musicais pelo Buzzer Passivo da placa e é capaz de:

- Reproduzir 4 músicas diferentes: Tema do Darth Vader, Tema da Pantera Cor de Rosa, Tema do Poderoso Chefão e Never Gonna Give You Up.
- Enquanto a música toca, o LED brilha em uma cor referente à música.
- Aumentar o volume.
- Diminuir o volume.
- Parar a música.

## Funcionalidades

- Tocar quatro faixas diferentes.
- O usuário escolhe qual música irá tocar pela direção do joystick.
- O botão A diminui o volume da música.
- O botão B aumenta o volume da música.
- Apertar os dois botões ao mesmo tempo interrompe a música.
- O LED RGB brilha em uma cor dependendo da música selecionada.

## Hardware Utilizado

- Raspberry Pi Pico W
- Joystick
- Buzzer Passivo
- Botão A
- Botão B
- LED RGB

## Software Utilizado

- Raspberry Pi Pico SDK
- C
- Visual Studio Code
- hardware/adc
- hardware/pwm
- hardware/gpio
- pico/time

  ## Diagrama de Conexões

| Componente   | Pino no Raspberry Pi Pico W |
|--------------|-----------------------------|
| Buzzer       | GPIO 21                     |
| LED RGB (R)  | GPIO 13                     |
| LED RGB (G)  | GPIO 11                     |
| LED RGB (B)  | GPIO 12                     |
| Joystick (X) | ADC0 (GPIO 26)              |
| Joystick (Y) | ADC1 (GPIO 27)              |
| Botão A      | GPIO 5                      |
| Botão B      | GPIO 6                      |

# Estrutura do Projeto

- **PROJETO_EMBARCADO/**
- **vscode/** # Configurações do Visual Studio Code
- **build/** # Pasta de builds gerados pelo CMake
- **CMakeLists.txt** # Configuração do CMake
- **musicas.h** # Header armazenando as músicas
- **notas.h** # Header com definições de notas musicais
- **PROJETO_EMBARCADO.c** # Código principal do projeto
- **README.md** # Documentação do projeto
