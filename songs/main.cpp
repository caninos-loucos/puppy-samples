#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <cstdint>
#include <vector>

#include "songs.hpp"

static const struct pwm_dt_spec buzzer = PWM_DT_SPEC_GET(DT_ALIAS(buzzer));

static const struct gpio_dt_spec button =
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(button0), gpios, {0});

static struct gpio_callback button_cb_data;

static Song nokia(120, melodyNokia, durationsNokia);
static Song birthday(120, melodyBirthday, durationsBirthday);
static Song starWars(80, melodyStarWars, durationsStarWars);
static Song neverGonnaGiveYouUp(56, melodyGiveYouUp, durationsGiveYouUp);
static Song tetris(60, melodyTetris, durationsTetris);

static std::vector<Song *> songs = {&nokia, &birthday, &starWars, 
                                    &neverGonnaGiveYouUp, &tetris};

static uint32_t songIndex = 0;

void button_pressed(const struct device *dev, struct gpio_callback *cb,
					uint32_t pins)
{	
  songIndex = (songIndex + 1) % songs.size();
	printk("Button pressed at %" PRIu32 ", playing song %d\n", k_cycle_get_32(), songIndex);
}

int main(void) {

  int ret;

  if (!gpio_is_ready_dt(&button)) {
    printk("Error: Button is not Ready\n");
    return 0;
  }

  if (!pwm_is_ready_dt(&buzzer)) {
    printk("Error: PWM device %s is not ready\n", buzzer.dev->name);
    return 0;
  }

  gpio_pin_configure_dt(&button, GPIO_INPUT);
	gpio_pin_interrupt_configure_dt(&button,
									GPIO_INT_EDGE_TO_ACTIVE);

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);

  while (1) {
    for (uint32_t j = 0; j < songs[songIndex]->durations.size(); j++) {
      ret = 0;

      printk("Note: %d Hz for 1/%d beats\n", songs[songIndex]->melody[j], songs[songIndex]->durations[j]);

      /* Sets PWM at 0 duty cycle if there is a silent note, otherwise sets 50% duty cycle for the specified frequency*/
      if(songs[songIndex]->melody[j] == 0) {
        ret = pwm_set_dt(&buzzer, 0, 0);
      }

      else {
        ret = pwm_set_dt(&buzzer, 1e9 / songs[songIndex]->melody[j], 1e9 / (songs[songIndex]->melody[j] * 2));
      }

      if(ret){
        printk("Couldn't set PWM!");
        return 0;
      }

      k_sleep(K_MSEC(60000 / (songs[songIndex]->bpm * songs[songIndex]->durations[j])));
    }
  }

  return 0;
}

