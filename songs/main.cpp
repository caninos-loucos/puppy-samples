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

static Song nokia(144, melodyNokia, durationsNokia);
static Song birthday(144, melodyBirthday, durationsBirthday);
static Song starWars(144, melodyStarWars, durationsStarWars);

static std::vector<Song *> songs = {&nokia, &birthday, &starWars};
static uint32_t songIndex = 0;

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

  while (1) {
    for (uint32_t j = 0; j < songs[songIndex]->durations.size(); j++) {

      printk("a");

      ret = pwm_set_dt(&buzzer, 1e9 / songs[songIndex]->melody[j],
                       1e9 / (songs[songIndex]->melody[j] * 2));
      k_sleep(K_MSEC(50 * songs[songIndex]->durations[j]));

      if (!gpio_pin_get_dt(&button)) {
        songIndex = (songIndex + 1) % songs.size();
        j = 0;
      }
    }
  }

  return 0;
}

