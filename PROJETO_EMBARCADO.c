#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "musicas.h"
#include <stdlib.h>
#include <stdio.h>

// PINOS
const int VRX = 26;          
const int VRY = 27;          
const int ADC_CHANNEL_0 = 0;
const int ADC_CHANNEL_1 = 1; 
const int SW = 22;          
const int BTN_VOL_DOWN = 5;  
const int BTN_VOL_UP = 6;    
const int LED_RED = 13;
const int LED_GREEN = 11;
const int LED_BLUE = 12;
bool stop_music = false;     
float volume_level = 0.5;    
uint32_t current_wrap = 0;


// inicializar o leds
void init_led() {
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_RED, 0);
    gpio_put(LED_GREEN, 0);
    gpio_put(LED_BLUE, 0);
}

// inicializar todos os pinos de input
void init_input() {
    adc_init();         
    adc_gpio_init(VRX); 
    adc_gpio_init(VRY); 
    gpio_init(SW);      
    gpio_set_dir(SW, GPIO_IN); 
    gpio_pull_up(SW);  


    gpio_init(BTN_VOL_DOWN);          
    gpio_set_dir(BTN_VOL_DOWN, GPIO_IN); 
    gpio_pull_up(BTN_VOL_DOWN);       

    gpio_init(BTN_VOL_UP);          
    gpio_set_dir(BTN_VOL_UP, GPIO_IN); 
    gpio_pull_up(BTN_VOL_UP);       
}

// seta a cor do led RGB

void set_led_color(bool red, bool green, bool blue) {
    gpio_put(LED_RED, red);
    gpio_put(LED_GREEN, green);
    gpio_put(LED_BLUE, blue);
}

// pega a posição do joystick

void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value) {
    adc_select_input(ADC_CHANNEL_0); 
    sleep_us(2);                     
    *vrx_value = adc_read();         
    adc_select_input(ADC_CHANNEL_1); 
    sleep_us(2);                     
    *vry_value = adc_read();         
}

// ajusta a frequência e volume

void pwm_set_freq(int freq) {
    uint slice_num = pwm_gpio_to_slice_num(21);
    uint chan = pwm_gpio_to_channel(21);
    uint32_t clock = 125000000; // 125 MHz
    uint32_t divider16 = clock / freq / 4096 + (clock % (freq * 4096) != 0);
    if (divider16 / 16 == 0)
        divider16 = 16;
    uint32_t wrap = clock * 16 / divider16 / freq - 1;
    current_wrap = wrap;
    pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, (uint)(current_wrap * volume_level));
}

// toca as notas da música selecionada

void play(int slice_num, int melodie[][2], int size, int tempo) {
    static bool prev_vol_down = false;
    static bool prev_vol_up = false;
    int wholenote = (60000 * 4) / tempo;

    for (int i = 0; i < size; i++) {
        int noteDuration = wholenote / melodie[i][1];
        if (melodie[i][1] < 0) {
            noteDuration = (int)(1.5 * (wholenote / abs(melodie[i][1])));
        }

        pwm_set_enabled(slice_num, false);
        pwm_set_freq(melodie[i][0]);
        pwm_set_enabled(slice_num, true);

        uint32_t start_time = to_ms_since_boot(get_absolute_time());
        uint32_t duration = noteDuration * 9 / 10;
        bool stop_note = false;

        while (to_ms_since_boot(get_absolute_time()) - start_time < duration) {
            bool btn_vol_down = !gpio_get(BTN_VOL_DOWN);
            bool btn_vol_up = !gpio_get(BTN_VOL_UP);

            if (btn_vol_down && btn_vol_up) {
                stop_music = true;
                printf("MÚSICA PAROU!\n");
                stop_note = true;
                sleep_ms(200); 
                break;
            } else {
                if (btn_vol_down && !prev_vol_down) {
                    volume_level -= 0.1;
                    if (volume_level < 0.0) volume_level = 0.0;

                    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(21), (uint)(current_wrap * volume_level));
                    printf("Volume diminuído para %.1f\n", volume_level);
                    sleep_ms(200);
                }
                if (btn_vol_up && !prev_vol_up) {
                    volume_level += 0.1;
                    if (volume_level > 1.0) volume_level = 1.0;
                    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(21), (uint)(current_wrap * volume_level));
                    printf("Volume aumentado para %.1f\n", volume_level);
                    sleep_ms(200); 
                }
            }

            prev_vol_down = btn_vol_down;
            prev_vol_up = btn_vol_up;
            sleep_ms(10); 
        }

        if (stop_music || stop_note) {
            pwm_set_enabled(slice_num, false);
            stop_music = false;
            set_led_color(0, 0, 0);
            return;
        }

        pwm_set_enabled(slice_num, false);
        sleep_ms(noteDuration / 10);
    }
}
// Funções para tocar cada música
void play_pink_panther(int slice_num) {
    int size = sizeof(PinkPanther) / sizeof(PinkPanther[0]);
    play(slice_num, PinkPanther, size, 120); // Tempo 120 BPM
}

void play_never_gonna_give_you_up(int slice_num) {
    int size = sizeof(NeverGonnaGiveYouUp) / sizeof(NeverGonnaGiveYouUp[0]);
    play(slice_num, NeverGonnaGiveYouUp, size, 114); // Tempo 114 BPM
}

void play_the_godfather(int slice_num) {
    int size = sizeof(TheGodfather) / sizeof(TheGodfather[0]);
    play(slice_num, TheGodfather, size, 80); // Tempo 80 BPM
}

void play_darth_vader(int slice_num) {
    int size = sizeof(DarthVader) / sizeof(DarthVader[0]);
    play(slice_num, DarthVader, size, 120); // Tempo 120 BPM
}
int main() {
    stdio_init_all();
    init_led();
    gpio_set_function(21, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(21);
    pwm_set_wrap(slice_num, 1023);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(21), 512);
    pwm_set_enabled(slice_num, true);

    init_input();

    while (1) {
        uint16_t vrx_value, vry_value;
        joystick_read_axis(&vrx_value, &vry_value);

        // Seleciona a música com base na posição do joystick
        if (vrx_value < 1000) {
            printf("Pink Panther\n");
            set_led_color(1, 0, 1); // Rosa 
            play_pink_panther(slice_num);
        } else if (vrx_value > 3000) {
            printf("Never Gonna Give You Up\n");
            set_led_color(0, 1, 0); // Verde
            play_never_gonna_give_you_up(slice_num);
            set_led_color(0, 0, 0); 
        } else if (vry_value < 1000) {
            printf("The Godfather\n");
            set_led_color(1, 1, 1); // Branco 
            play_the_godfather(slice_num);
            set_led_color(0, 0, 0); 
        } else if (vry_value > 3000) {
            printf("Darth Vader\n");
            set_led_color(1, 0, 0); // Vermelho
            play_darth_vader(slice_num);
            set_led_color(0, 0, 0); 
        }

        sleep_ms(100); 
    }  
}