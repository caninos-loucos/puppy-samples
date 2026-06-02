#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("Application running!"); 

    while (1)
    {
        printk("Hello World from Puppy!\n");
        k_msleep(1000);
    }

    return 0;
}
