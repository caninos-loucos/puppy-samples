#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <cstdint>
#include <vector>

#include "songs.hpp"

#define BUTTON0_NODE DT_ALIAS(button0)
#if !DT_NODE_HAS_STATUS_OKAY(BUTTON0_NODE)
#error "Unsupported board: button0 devicetree alias is not defined"
#endif

#define BUZZER_NODE DT_ALIAS(buzzer)
#if !DT_NODE_HAS_STATUS_OKAY(BUZZER_NODE)
#error "Unsupported board: buzzer devicetree alias is not defined"
#endif

LOG_MODULE_REGISTER(app, LOG_LEVEL_ERR);

static const struct gpio_dt_spec button =
    GPIO_DT_SPEC_GET(BUTTON0_NODE, gpios);

static const struct pwm_dt_spec buzzer = 
    PWM_DT_SPEC_GET(BUZZER_NODE);

static Song nokia(60, melodyNokia, durationsNokia);
static Song birthday(60, melodyBirthday, durationsBirthday);
static Song starWars(60, melodyStarWars, durationsStarWars);
static Song neverGonnaGiveYouUp(56, melodyGiveYouUp, durationsGiveYouUp);
static Song tetris(60, melodyTetris, durationsTetris);

static std::vector<Song *> songs = {
    &nokia, &birthday, &starWars, 
    &neverGonnaGiveYouUp, &tetris
};

int main(void)
{
    int ret;

    if (!gpio_is_ready_dt(&button))
    {
        LOG_ERR("Button is not ready.");
        return 0;
    }
    if (!pwm_is_ready_dt(&buzzer))
    {
        LOG_ERR("PWM device is not ready.");
        return 0;
    }
    if (gpio_pin_configure_dt(&button, GPIO_INPUT) != 0)
    {
        LOG_ERR("gpio_pin_configure_dt() failed.");
        return 0;
    }
    for (uint32_t j = 0; j < songs.size(); j++)
    {
        if (songs[j]->durations.size() != songs[j]->melody.size())
        {
            LOG_ERR("Duration size is not equal to melody size.");
            return 0;
        }
    }
    
    uint32_t songIndex = 0;
    
    while (1)
    {
        for (uint32_t j = 0; j < songs[songIndex]->durations.size(); j++)
        {
            /*
             * Sets PWM at 0 duty cycle if there is a silent note,
             * otherwise sets 50% duty cycle for the specified frequency.
             */
            if (songs[songIndex]->melody[j] == 0) {
                ret = pwm_set_dt(&buzzer, 1e9, 0);
            }
            else
            {
                uint32_t pulsePeriod = 1e9 / songs[songIndex]->melody[j];
                ret = pwm_set_dt(&buzzer, pulsePeriod, pulsePeriod / 2);
            }
            if(ret != 0)
            {
                LOG_ERR("Could not set PWM.");
                return 0;
            }
            
            uint32_t duration = songs[songIndex]->durations[j];
            k_sleep(K_MSEC(60000 / (songs[songIndex]->bpm * duration)));
            
            if (gpio_pin_get_dt(&button))
            {
                songIndex = (songIndex + 1) % songs.size();
                break;
            }
        }
    }
    return 0;
}

