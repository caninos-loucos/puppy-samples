#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/hwaccel.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

#define BUF_LEN 8

static int len[] = {BUF_LEN};
static int scalar_len[] = {1};

static int32_t Au32_buf[BUF_LEN] = {1, 2, 3, 4, 5, 6, 7, 8};
static int32_t Bu32_buf[BUF_LEN] = {11, 12, 13, 14, 15, 16, 17, 18};
static int32_t Cu32_buf[BUF_LEN] = {21, 22, 23, 24, 25, 26, 27, 28};

static accel_buffer_t Au32 = {
		.fmt = FMT_INT32, .len = len, .dim = 1, .buf = Au32_buf};
static accel_buffer_t Bu32 = {
		.fmt = FMT_INT32, .len = len, .dim = 1, .buf = Bu32_buf};
static accel_buffer_t Cu32 = {
		.fmt = FMT_INT32, .len = len, .dim = 1, .buf = Cu32_buf};

/* Scalars used by regsum/regmult/shift ops */
static uint32_t scalar_value = 5;
static uint32_t shift_value = 2;

static accel_buffer_t scalar = {
		.fmt = FMT_INT32, .len = scalar_len, .dim = 0, .buf = &scalar_value};
static accel_buffer_t shift = {
		.fmt = FMT_INT32, .len = scalar_len, .dim = 0, .buf = &shift_value};

static volatile uint32_t out_buf[BUF_LEN];
static accel_buffer_t out = {
		.fmt = FMT_INT32, .len = len, .dim = 1, .buf = out_buf};

static const struct {
	const char *name;
	accel_buffer_t *buf;
} buf_table[] = {
	{"a", &Au32},
	{"b", &Bu32},
	{"c", &Cu32},
	{"scalar", &scalar},
	{"shift", &shift}};

static const struct {
	const char *name;
	uint32_t ops;
	int nbufs;
} case_table[] = {
	{"sum", BIT(HW_OP_SUM), 2},
	{"sub", BIT(HW_OP_SUB), 2},
	{"mul", BIT(HW_OP_MULT), 2},
	{"mac", BIT(HW_OP_MULT) | BIT(HW_OP_ACCUMULATE), 2},
	{"sq", BIT(HW_OP_SQUARE), 1},
	{"sqsum", BIT(HW_OP_SQUARE) | BIT(HW_OP_SUM), 2},
	{"sqsub", BIT(HW_OP_SQUARE) | BIT(HW_OP_SUB), 2},
	{"sqacc", BIT(HW_OP_SQUARE) | BIT(HW_OP_ACCUMULATE), 1},
	{"sqsumsc", BIT(HW_OP_SQUARE) | BIT(HW_OP_SUM_SCALAR), 2},
	{"sumsc", BIT(HW_OP_SUM_SCALAR), 2},
	{"mulsc", BIT(HW_OP_MULT_SCALAR), 2},
	{"shsc", BIT(HW_OP_SHIFT), 2},
	{"sumscsq", BIT(HW_OP_SQUARE) | BIT(HW_OP_SUM_SCALAR), 2},
	{"sumscmul", BIT(HW_OP_MULT) | BIT(HW_OP_SUM_SCALAR), 3},
	{"mulscsumsc", BIT(HW_OP_MULT_SCALAR) | BIT(HW_OP_SUM_SCALAR), 3},
	{"mulscsum", BIT(HW_OP_MULT_SCALAR) | BIT(HW_OP_SUM), 3},
	{"mulscac", BIT(HW_OP_MULT_SCALAR) | BIT(HW_OP_ACCUMULATE), 3},
	{"mulscsub", BIT(HW_OP_MULT_SCALAR) | BIT(HW_OP_SUB), 3}};

accel_buffer_t *in[3];
int nbufs = 0;

K_SEM_DEFINE(sem, 0, 1);

void accel_callback(const struct device *dev, void *user_data) {
	k_sem_give(&sem);
}

const struct device *dev = DEVICE_DT_GET_ONE(caninos_puppy_udma_filter);

static int set_buffers(const struct shell *sh, size_t argc, char **argv) {

	for (int i = 0; i < (argc - 1); i++) {
		in[i] = NULL;
		ARRAY_FOR_EACH_PTR(buf_table, ptr) {
			if (strcmp(ptr->name, argv[i + 1]) == 0) {
				in[i] = ptr->buf;
				shell_info(sh, "Loading buffer %s", ptr->name);
			}
		}

		if (in[i] == NULL) {
			shell_error(sh, "Invalid Buffer %d", i);
			return -EINVAL;
		}
	}
	int ret = accel_set_buffers(dev, in, argc - 1, &out);

	if (ret != 0) {
		shell_error(sh, "Error: %d", ret);
	}

	return ret;
}

static int set_ops(const struct shell *sh, size_t argc, char **argv) {

	uint32_t ops = 0;

	ARRAY_FOR_EACH_PTR(case_table, ptr) {
		if (strcmp(ptr->name, argv[1]) == 0) {
			ops = ptr->ops;
			nbufs = ptr->nbufs;
			shell_info(sh, "Setting OP %s", ptr->name);
		}
	}

	if (ops == 0) {
		shell_error(sh, "Invalid operation");
		return -EINVAL;
	}

	int ret = accel_configure_ops(dev, ops);
	if (ret != 0) {
		shell_error(sh, "Error: %d", ret);
	}

	return ret;
}

static int run(const struct shell *sh, size_t argc, char **argv) {
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	int ret;

	shell_info(sh, "running...");
	accel_start(dev);

	ret = k_sem_take(&sem, K_SECONDS(1));

	if (ret == 0) {
		printf(" \nOUT: ");
		for (int i = 0; i < BUF_LEN; i++)
			printf("%d ", out_buf[i]);
		printf("\n");

	} else {
		shell_error(sh, "No response...");
	}

	return 0;
}

int main() {
	printf("\n\n----------Interactive Filter Accelerator Test----------\n\n");
	printf("Available modes:\n\t");

	ARRAY_FOR_EACH_PTR(case_table, ptr) { printf("%s ", ptr->name); }

	printf("\n");

	printf("Available Buffers:\n");

	ARRAY_FOR_EACH_PTR(buf_table, ptr) {
		printf("%s: ", ptr->name);
		for (int j = 0; j < ptr->buf->dim; j++) {
			printf("{ ");
			for (int i = 0; i < ptr->buf->len[j]; i++)
				printf("%d ", ((uint32_t *)(ptr->buf->buf))[i]);
			printf("}");
		}
		printf("\n");
	}

	if (accel_set_callback(dev, accel_callback, NULL) < 0) {
		LOG_ERR("Error setting callback!");
	}
}

SHELL_CMD_ARG_REGISTER(op, NULL, "Set ops", set_ops, 2, 0);
SHELL_CMD_ARG_REGISTER(buf, NULL, "Set buffers", set_buffers, 2, 2);
SHELL_CMD_REGISTER(run, NULL, "run", run);