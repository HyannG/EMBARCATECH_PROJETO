# EMBARCATECH_PROJETO

Este projeto implementa um MP3 Player simples utilizando o Raspberry Pi Pico W. Ele reproduz notas musicais pelo Buzzer Passivo da placa e é capaz de:

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
- hardware/adc
- hardware/pwm
- hardware/gpio
- pico/time
