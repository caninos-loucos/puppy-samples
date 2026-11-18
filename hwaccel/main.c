#include <zephyr/device.h>
#include <zephyr/drivers/hwaccel.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define BUF_LEN 32

int len[] = {BUF_LEN};

uint32_t Au32_buf[BUF_LEN];
uint32_t Bu32_buf[BUF_LEN];

accel_buffer_t Au32 = {
    .fmt = FMT_UINT32,
    .len = len,
    .dim = 1,
    .buf = Au32_buf,
};

accel_buffer_t Bu32 = {
    .fmt = FMT_UINT32,
    .len = len,
    .dim = 1,
    .buf = Bu32_buf,
};

uint16_t Au16_buf[BUF_LEN];
uint16_t Bu16_buf[BUF_LEN];

accel_buffer_t Au16 = {
    .fmt = FMT_UINT16,
    .len = len,
    .dim = 1,
    .buf = Au16_buf,
};

accel_buffer_t Bu16 = {
    .fmt = FMT_UINT16,
    .len = len,
    .dim = 1,
    .buf = Bu16_buf,
};

uint8_t Au8_buf[BUF_LEN];
uint8_t Bu8_buf[BUF_LEN];

accel_buffer_t Au8 = {
    .fmt = FMT_UINT8,
    .len = len,
    .dim = 1,
    .buf = Au8_buf,
};

accel_buffer_t Bu8 = {
    .fmt = FMT_UINT8,
    .len = len,
    .dim = 1,
    .buf = Bu8_buf,
};

int32_t A32_buf[BUF_LEN];
int32_t B32_buf[BUF_LEN];

accel_buffer_t A32 = {
    .fmt = FMT_INT32,
    .len = len,
    .dim = 1,
    .buf = A32_buf,
};

accel_buffer_t B32 = {
    .fmt = FMT_INT32,
    .len = len,
    .dim = 1,
    .buf = B32_buf,
};

int16_t A16_buf[BUF_LEN];
int16_t B16_buf[BUF_LEN];

accel_buffer_t A16 = {
    .fmt = FMT_INT16,
    .len = len,
    .dim = 1,
    .buf = A16_buf,
};

accel_buffer_t B16 = {
    .fmt = FMT_INT16,
    .len = len,
    .dim = 1,
    .buf = B16_buf,
};

int8_t A8_buf[BUF_LEN];
int8_t B8_buf[BUF_LEN];

accel_buffer_t A8 = {
    .fmt = FMT_INT8,
    .len = len,
    .dim = 1,
    .buf = A8_buf,
};

accel_buffer_t B8 = {
    .fmt = FMT_INT8,
    .len = len,
    .dim = 1,
    .buf = B8_buf,
};

volatile uint32_t out_buf[BUF_LEN];

const struct device *dev = DEVICE_DT_GET_ONE(caninos_puppy_udma_filter);

int main() {
  int ret;

  for (int i = 0; i < BUF_LEN; i++) {
    Au32_buf[i] = i;
    Bu32_buf[i] = i;
  }
  for (int i = 0; i < BUF_LEN; i++) {
    Au16_buf[i] = i;
    Bu16_buf[i] = i;
  }
  for (int i = 0; i < BUF_LEN; i++) {
    Au8_buf[i] = i;
    Bu8_buf[i] = i;
  }

  for (int i = 0; i < BUF_LEN; i++) {
    A32_buf[i] = i - BUF_LEN / 2;
    B32_buf[i] = i - BUF_LEN / 2;
  }
  for (int i = 0; i < BUF_LEN; i++) {
    A16_buf[i] = i - BUF_LEN / 2;
    B16_buf[i] = i - BUF_LEN / 2;
  }
  for (int i = 0; i < BUF_LEN; i++) {
    A8_buf[i] = i - BUF_LEN / 2;
    B8_buf[i] = i - BUF_LEN / 2;
  }

  printk("Getting accel caps\n");

  accel_hw_caps_t caps;
  ret = accel_query_hw_caps(dev, &caps);
  printk("Setting buffers\n");

  accel_buffer_t *in_bufs[] = {&Au32, &Bu32};

  printk("Buffers are:\n");
  printk("\tA: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", Au32_buf[i]);

  printk("\n\tB: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", Bu32_buf[i]);

  printk("\n");

  accel_buffer_t out = {
      .fmt = FMT_UINT32,
      .len = len,
      .dim = 1,
      .buf = (volatile uint32_t *)out_buf,
  };

  uint32_t scalar_len[] = {1};
  uint32_t scalar_value = 2;
  accel_buffer_t scalar = {
    .fmt = FMT_UINT32,
    .len = NULL,
    .dim = 0,
    .buf = (volatile uint32_t *)&scalar_value,
  };

  
  ret = accel_set_buffers(dev, in_bufs, 2, &out);

  printk("Setting Operations\n");

  // i think the function should be called set_ops to stay in line with set bufs
  ret = accel_configure_ops(dev, HW_OP_MAC);

  if (ret) {
    printk("Could not set ops, error %d\n", ret);
  }

  printk("Starting\n");

  accel_start(dev);

  while(accel_check_status(dev) != 0);

  printk("Let's see what's inside\n");

  printk("\tOUT: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", out_buf[i]);
  printk("\n");

  accel_abort(dev);

  printk("\nGood lets try sum\n");

  ret = accel_configure_ops(dev, HW_OP_SUM);

  accel_start(dev);

  while(accel_check_status(dev) != 0);

  printk("Let's see what's inside\n");

  printk("\tOUT: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", out_buf[i]);
  printk("\n");

  accel_abort(dev);

  printk("\nNow lets try square\n");

  ret = accel_configure_ops(dev, HW_OP_MULT);

  accel_start(dev);

  while(accel_check_status(dev) != 0);

  printk("Let's see what's inside\n");

  printk("\tOUT: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", out_buf[i]);
  printk("\n");

  accel_abort(dev);

  printk("\nNow lets try MAC\n");

  ret = accel_configure_ops(dev, HW_OP_SUM);

  accel_start(dev);

  while(accel_check_status(dev) != 0);

  printk("Let's see what's inside\n");

  printk("\tOUT: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", out_buf[i]);
  printk("\n");

  accel_abort(dev);
  
  printk("\nNow lets try subtract\n");

  ret = accel_configure_ops(dev, HW_OP_SUB);

  accel_start(dev);

  while(accel_check_status(dev) != 0);

  printk("Let's see what's inside\n");

  printk("\tOUT: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", out_buf[i]);
  printk("\n");
  
  accel_abort(dev);

  printk("\nNow lets try Square Sum\n");

  ret = accel_configure_ops(dev, HW_OP_SQUARE_SUM);

  accel_start(dev);

  while(accel_check_status(dev) != 0);

  printk("Let's see what's inside\n");

  printk("\tOUT: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", out_buf[i]);
  printk("\n");

  accel_abort(dev);

  printk("\nNow lets try Square Sub\n");
  
  ret = accel_configure_ops(dev, HW_OP_SQUARE_SUB);

  accel_start(dev);

  while(accel_check_status(dev) != 0);

  printk("Let's see what's inside\n");

  printk("\tOUT: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", out_buf[i]);
  printk("\n");

  accel_abort(dev);

  printk("\nLets try mult again\n");

  ret = accel_configure_ops(dev, HW_OP_MULT);

  accel_start(dev);

  while(accel_check_status(dev) != 0);

  printk("Let's see what's inside\n");

  printk("\tOUT: ");
  for (int i = 0; i < BUF_LEN; i++)
    printk("%d ", out_buf[i]);
  printk("\n");

  return ret;
}
