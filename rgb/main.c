#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

static const struct pwm_dt_spec red_pwm_led =
	PWM_DT_SPEC_GET(DT_ALIAS(red_pwm_led));
static const struct pwm_dt_spec green_pwm_led =
	PWM_DT_SPEC_GET(DT_ALIAS(green_pwm_led));
static const struct pwm_dt_spec blue_pwm_led =
	PWM_DT_SPEC_GET(DT_ALIAS(blue_pwm_led));

#define BUTTON0_NODE DT_ALIAS(button0)
#if !DT_NODE_HAS_STATUS_OKAY(BUTTON0_NODE)
#error "Unsupported board: button0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(BUTTON0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

static bool rainbow_mode = false;

void button_pressed(const struct device *dev, struct gpio_callback *cb,
					uint32_t pins)
{
	rainbow_mode = !rainbow_mode;
	printk("Button pressed at %" PRIu32 ", rainbow mode %s\n", k_cycle_get_32(), rainbow_mode ? "on" : "off");
}

int main(void)
{
	if (!pwm_is_ready_dt(&red_pwm_led) || !pwm_is_ready_dt(&green_pwm_led) || !pwm_is_ready_dt(&blue_pwm_led) || !gpio_is_ready_dt(&button))
	{
		printk("Error: one or more devices not ready, check devicetree\n");
		return 0;
	}

	/* Initialize pulse widths(duty cycles) to zero */
	uint32_t pulse_red = 0, pulse_green = 0, pulse_blue = 0;

	printk("PWM-based RGB LED control\n");

	gpio_pin_configure_dt(&button, GPIO_INPUT);

	gpio_pin_interrupt_configure_dt(&button,
									GPIO_INT_EDGE_TO_ACTIVE);

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);

	while (1)
	{
		if (rainbow_mode == true)
		{
			/* 	Red LED varies at 15% intensity per iteration
				Green varies at 10%
				Blue varies at 5%	*/

			pulse_red += (red_pwm_led.period * 15) / 100;
			pulse_red = pulse_red % red_pwm_led.period;
			if (!pwm_set_pulse_dt(&red_pwm_led, pulse_red))
			{
				printk("Could not set RED LED!");
				return 0;
			}

			pulse_green += (green_pwm_led.period * 10) / 100;
			pulse_green = pulse_green % green_pwm_led.period;
			if (!pwm_set_pulse_dt(&green_pwm_led,
								  pulse_green))
			{
				printk("Could not set GREEN LED!");
				return 0;
			}

			pulse_blue += (blue_pwm_led.period * 5) / 100;
			pulse_blue = pulse_blue % blue_pwm_led.period;
			if (!pwm_set_pulse_dt(&blue_pwm_led,
								  pulse_blue))
			{
				printk("Could not set BLUE LED!");
				return 0;
			}
		}

		k_sleep(K_MSEC(30));
	}

	return 0;
}
