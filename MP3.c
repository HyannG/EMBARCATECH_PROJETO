#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "hardware/clocks.h"
#include <string.h>
#include "pico/time.h"
#include <math.h>
#include "musicas.h"
#include <stdlib.h>
#include <stdio.h>
#include "hardware/pio.h"
#include "ws2818b.pio.h"
#define LED_COUNT 25
#define LED_PIN 7

struct pixel_t {
    uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
pixel_t leds[LED_COUNT];

PIO np_pio;
uint sm;

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
#define MIC_PIN 28               
const float SOUND_OFFSET = 1.65; 
const float SOUND_THRESHOLD = 0.5; 
const float ADC_REF = 3.3;         
const int ADC_RES = 4095;    

bool music_playing = false;  
bool is_muted = false;

// funções da matriz de led

void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
    memset(leds, 0, sizeof(leds));
}

void npSetLED(uint index, uint8_t r, uint8_t g, uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

void npClear() {
    memset(leds, 0, sizeof(leds));
}

void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
}

// Inicializar os sensores e atenuadores
void init_all() {
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_RED, 0);
    gpio_put(LED_GREEN, 0);
    gpio_put(LED_BLUE, 0);

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

    adc_gpio_init(MIC_PIN);
    adc_select_input(2);     
}

// Seta a cor do LED RGB
void set_led_color(bool red, bool green, bool blue) {
    gpio_put(LED_RED, red);
    gpio_put(LED_GREEN, green);
    gpio_put(LED_BLUE, blue);
}

// Pega a posição do joystick
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value) {
    adc_select_input(ADC_CHANNEL_0); 
    sleep_us(2);                     
    *vrx_value = adc_read();         
    adc_select_input(ADC_CHANNEL_1); 
    sleep_us(2);                     
    *vry_value = adc_read();         
}

// Ajusta a frequência e volume
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
    pwm_set_chan_level(slice_num, chan, (uint)(current_wrap * volume_level * 0.6));
}


// Exibição do volume pela matriz de led
void update_volume_leds() {
    npClear();
    if (!is_muted) {
        int level = (int)(volume_level * 5.0f);
        level = (level > 5) ? 5 : (level < 0) ? 0 : level;
        
        for (int row = 0; row < level; row++) {
            for (int col = 0; col < 5; col++) {
                int index = row * 5 + col;
                npSetLED(index, 0, 50, 0); 
            }
        }
    }
    npWrite();
}

// Função para ativar/desativar o mudo
void toggle_mute(uint slice_num, bool mute) {
    is_muted = mute;
    if (is_muted) {
        pwm_set_enabled(slice_num, false); 
        pwm_set_gpio_level(21, 0); 
        gpio_set_dir(21, GPIO_IN); // Configura o pino como entrada para evitar ruídos
    } else {
        gpio_set_dir(21, GPIO_OUT); 
        pwm_set_enabled(slice_num, true); 
    }
    update_volume_leds();
    printf("Mudo: %s\n", is_muted ? "LIGADO" : "DESLIGADO");
}

// Função para atualizar o display com o nome da música
void update_display_music(uint8_t *ssd, struct render_area *frame_area, const char *music_name) {
    memset(ssd, 0, ssd1306_buffer_length); // Limpa o display
    ssd1306_draw_string(ssd, 0, 0, "MP3 PLAYER");
    ssd1306_draw_string(ssd, 0, 16, music_name); 
    if (is_muted) {
        ssd1306_draw_string(ssd, 0, 32, "Mudo"); 
    }
    render_on_display(ssd, frame_area);
}

// Função para resetar o display
void reset_display(uint8_t *ssd, struct render_area *frame_area) {
    memset(ssd, 0, ssd1306_buffer_length); // Limpa o display
    ssd1306_draw_string(ssd, 0, 0, "MP3 PLAYER");
    ssd1306_draw_string(ssd, 0, 16, "Embarcatech"); 
    render_on_display(ssd, frame_area);
}

// função para desmutar com sons
bool check_sound_trigger() {
    adc_select_input(2);
    uint16_t raw_adc = adc_read();
    float voltage = (raw_adc * ADC_REF) / ADC_RES;
    float sound_level = fabs(voltage - SOUND_OFFSET); 
    return (sound_level > SOUND_THRESHOLD);
}

// Toca as notas da música selecionada
void play(int slice_num, int melodie[][2], int size, int tempo, const char *music_name, uint8_t *ssd, struct render_area *frame_area) {
    static bool prev_vol_down = false;
    static bool prev_vol_up = false;
    static bool prev_sw = true;
    int wholenote = (60000 * 4) / tempo;

    update_display_music(ssd, frame_area, music_name);

    for (int i = 0; i < size; i++) {
        int noteDuration = wholenote / melodie[i][1];
        if (melodie[i][1] < 0) {
            noteDuration = (int)(1.5 * (wholenote / abs(melodie[i][1])));
        }

        pwm_set_enabled(slice_num, false);
        pwm_set_freq(melodie[i][0]);
        pwm_set_enabled(slice_num, !is_muted);

        uint32_t start_time = to_ms_since_boot(get_absolute_time());
        uint32_t duration = noteDuration * 9 / 10;
        bool stop_note = false;

        while (to_ms_since_boot(get_absolute_time()) - start_time < duration) {
            bool btn_vol_down = !gpio_get(BTN_VOL_DOWN);
            bool btn_vol_up = !gpio_get(BTN_VOL_UP);
            bool btn_sw = !gpio_get(SW);

            if (is_muted && check_sound_trigger()) {
                toggle_mute(slice_num, false); 
                update_display_music(ssd, frame_area, music_name);
                printf("Mudo DESLIGADO por detecção de som!\n");
                sleep_ms(1000); 
            }

            if (btn_sw && !prev_sw) {
                toggle_mute(slice_num, !is_muted); 
                update_display_music(ssd, frame_area, music_name);
                printf("Mudo: %s\n", is_muted ? "LIGADO" : "DESLIGADO");
                sleep_ms(200); 
            }
            prev_sw = btn_sw;
        
            if (btn_vol_down && btn_vol_up) {
                stop_music = true;
                printf("MÚSICA PAROU!\n");
                stop_note = true;
                sleep_ms(200); 
                break;
            } else {
                if (btn_vol_down && !prev_vol_down) {
                    volume_level -= 0.25; 
                    if (volume_level < 0.0) volume_level = 0.0; 
                    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(21), (uint)(current_wrap * volume_level * 0.6));
                    printf("Volume diminuído para %.2f\n", volume_level);
                    update_volume_leds();
                    sleep_ms(200);
                }
                if (btn_vol_up && !prev_vol_up) {
                    volume_level += 0.25; 
                    if (volume_level > 1.0) volume_level = 1.0; 
                    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(21), (uint)(current_wrap * volume_level * 0.6));
                    printf("Volume aumentado para %.2f\n", volume_level);
                    update_volume_leds();
                    sleep_ms(200); 
                }
            }
        
            prev_vol_down = btn_vol_down;
            prev_vol_up = btn_vol_up;
            sleep_ms(10); 
        }

        if (stop_music || stop_note) {
            toggle_mute(slice_num, true); 
            stop_music = false;
            set_led_color(0, 0, 0);
            reset_display(ssd, frame_area); 
            return;
        }

        pwm_set_enabled(slice_num, false);
        sleep_ms(noteDuration / 10);
    }

    toggle_mute(slice_num, true); 
    reset_display(ssd, frame_area); 
}

typedef enum {
    PINK_PANTHER,
    NEVER_GONNA_GIVE_YOU_UP,
    THE_GODFATHER,
    DARTH_VADER
} song_id;

void play_music(song_id song, int slice_num, uint8_t *ssd, struct render_area *frame_area) {
    int size;
    int tempo;
    const char *name;
    int (*melody)[2];

    switch(song) {
        case PINK_PANTHER:
            melody = PinkPanther;
            size = sizeof(PinkPanther) / sizeof(PinkPanther[0]);
            tempo = 120;
            name = "Pink Panther";
            break;
            
        case NEVER_GONNA_GIVE_YOU_UP:
            melody = NeverGonnaGiveYouUp;
            size = sizeof(NeverGonnaGiveYouUp) / sizeof(NeverGonnaGiveYouUp[0]);
            tempo = 114;
            name = "Never Gonna Give You Up";
            break;
            
        case THE_GODFATHER:
            melody = TheGodfather;
            size = sizeof(TheGodfather) / sizeof(TheGodfather[0]);
            tempo = 80;
            name = "The Godfather";
            break;
            
        case DARTH_VADER:
            melody = DarthVader;
            size = sizeof(DarthVader) / sizeof(DarthVader[0]);
            tempo = 120;
            name = "Darth Vader";
            break;
    }  
    play(slice_num, melody, size, tempo, name, ssd, frame_area);
}

int main() {
    stdio_init_all();
    init_all();
    npInit(LED_PIN);

    // Inicializa o PWM
    gpio_set_function(21, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(21);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); 
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(21, 0);

    toggle_mute(slice_num, true);

    // Inicializa o I2C para o display
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(14, GPIO_FUNC_I2C); // SDA
    gpio_set_function(15, GPIO_FUNC_I2C); // SCL
    gpio_pull_up(14);
    gpio_pull_up(15);
    ssd1306_init();

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // Limpar o display
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    // Mensagem inicial
    ssd1306_draw_string(ssd, 0, 0, "MP3 PLAYER");
    ssd1306_draw_string(ssd, 0, 16, "Embarcatech");
    render_on_display(ssd, &frame_area);

    while (1) {
        uint16_t vrx_value, vry_value;
        joystick_read_axis(&vrx_value, &vry_value);
    
        if (!music_playing) {
            if (vrx_value < 1000) {
                printf("Pink Panther\n");
                set_led_color(1, 0, 1);
                music_playing = true;
                toggle_mute(slice_num, false);
                play_music(PINK_PANTHER, slice_num, ssd, &frame_area);
                music_playing = false;
            } else if (vrx_value > 3000) {
                printf("Never Gonna Give You Up\n");
                set_led_color(0, 1, 0);
                music_playing = true;
                toggle_mute(slice_num, false);
                play_music(NEVER_GONNA_GIVE_YOU_UP, slice_num, ssd, &frame_area);
                music_playing = false;
            } else if (vry_value < 1000) {
                printf("The Godfather\n");
                set_led_color(1, 1, 1);
                music_playing = true;
                toggle_mute(slice_num, false);
                play_music(THE_GODFATHER, slice_num, ssd, &frame_area);
                music_playing = false;
            } else if (vry_value > 3000) {
                printf("Darth Vader\n");
                set_led_color(1, 0, 0);
                music_playing = true;
                toggle_mute(slice_num, false);
                play_music(DARTH_VADER, slice_num, ssd, &frame_area);
                music_playing = false;
            }
        }
        sleep_ms(100);
    }
}