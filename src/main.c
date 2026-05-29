#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    while (1)
    {
        printk("Hello World from Puppy!\n");
        k_usleep(1000000);
    }
    return 0;
}

