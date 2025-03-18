# EMBARCATECH_PROJETO

Este projeto implementa um MP3 Player utilizando o Raspberry Pi Pico W. Ele reproduz músicas por meio de um buzzer passivo e possui:

- Reprodução de 4 músicas: Tema de Darth Vader, Pantera Cor-de-Rosa, Poderoso Chefão e Never Gonna Give You Up
- Controle por joystick para seleção de músicas
- Sistema de volume com feedback visual na matriz de LEDs
- Modo mudo ativável por botão ou som ambiente
- Interface visual em display OLED
- LEDs RGB indicativos de estado

---

## Funcionalidades Principais

- ▶️ Reprodução de 4 faixas musicais
- 🎛️ Controle de volume com botões dedicados
- 🔇 Modo mudo ativável por clique no joystick
- 🔊 Desmutar automático por detecção de som ambiente
- 🌈 LED RGB com cor específica para cada música
- 📊 Matriz de LEDs como indicador de volume
- 🖥️ Display OLED com informações da faixa
- ⏹️ Parar reprodução com combinação de botões

---

## Hardware Utilizado

- Raspberry Pi Pico W
- Joystick analógico
- Buzzer passivo
- Botões para volume (2 unidades)
- LED RGB
- Microfone analógico
- Matriz de LEDs WS2812B (5x5)
- Display OLED 128x32 (I2C)
- Conversor ADC
- Circuito PWM para áudio

---

## Software Utilizado

- Raspberry Pi Pico SDK
- Linguagem C
- Visual Studio Code
- Bibliotecas:
  - `hardware/adc`
  - `hardware/pwm`
  - `hardware/i2c`
  - `pico/time`
  - `ssd1306` (display)
  - `ws2818b` (LEDs endereçáveis)

---

## Manual de Operação

### Controles e Interface

1. **Seleção de Música (Joystick)**:
   - ⬅️ Esquerda: Tema do Poderoso Chefão (LED branco)
   - ➡️ Direita: Tema de Darth Vader (LED vermelho)
   - ⬆️ Cima: Never Gonna Give You Up (LED verde)
   - ⬇️ Baixo: Pantera Cor-de-Rosa (LED rosa)

2. **Controle de Volume**:
   - 🔽 Botão A (GPIO 5): Diminui volume em 25%
   - 🔼 Botão B (GPIO 6): Aumenta volume em 25%
   - ⏹️ A+B: Para a reprodução atual

3. **Modo Mudo**:
   - 🔘 Clique no joystick: Alterna entre mudo/volume
   - 🎤 Som ambiente: Detecção automática de som alto desativa o mudo

4. **Feedback Visual**:
   - 🟥 Matriz de LEDs: Exibe 5 níveis de volume
   - 🟢 Display OLED: Mostra nome da música e estado do mudo
   - 🌈 LED RGB: Indica música selecionada

---

## Diagrama de Conexões

| Componente       | Pino no Pico W |
|------------------|----------------|
| Buzzer           | GPIO 21        |
| LED RGB (R)      | GPIO 13        |
| LED RGB (G)      | GPIO 11        |
| LED RGB (B)      | GPIO 12        |
| Joystick (X)     | ADC0 (GPIO 26) |
| Joystick (Y)     | ADC1 (GPIO 27) |
| Botão Volume -   | GPIO 5         |
| Botão Volume +   | GPIO 6         |
| Matriz de LEDs   | GPIO 7         |
| Display OLED SDA | GPIO 14        |
| Display OLED SCL | GPIO 15        |
| Microfone        | ADC2 (GPIO 28) |

---

## Estrutura do Projeto


- **PROJETO_EMBARCADO/**  
  - **vscode/** # Configurações do Visual Studio Code  
  - **build/** # Pasta de builds gerados pelo CMake  
  - **CMakeLists.txt** # Configuração do CMake  
  - **musicas.h** # Header armazenando as músicas  
  - **notas.h** # Header com definições de notas musicais  
  - **PROJETO_EMBARCADO.c** # Código principal do projeto  
  - **README.md** # Documentação do projeto  


