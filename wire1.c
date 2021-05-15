/**
 * @file wire1.c
 *
 * 1-Wire communication bus.
 *
 * @see https://www.maximintegrated.com/en/design/technical-documents/app-notes/1/126.html
 */

#include "ulp_riscv/ulp_riscv.h"
#include "ulp_riscv/ulp_riscv_utils.h"

#include "soc/rtc_io_reg.h"

#include "ulp_gpio.h"
#include "wire1.h"

volatile uint32_t delay[WIRE1_TIMING_LEN]; /**< Number of iterations to delay */

/**
 * Wait (sleep) n clock cycles.
 */
extern void wait(uint32_t n);

void wire1_set_timing_delay(uint32_t clk_period)
{
    const uint32_t standard_us[WIRE1_TIMING_LEN] = {
        [A] = WIRE1_A, [B] = WIRE1_B, [C] = WIRE1_C, [D] = WIRE1_D,
        [E] = WIRE1_E, [F] = WIRE1_F, [G] = WIRE1_G, [H] = WIRE1_H,
        [I] = WIRE1_I, [J] = WIRE1_J};

    uint32_t period = clk_period > 0 ? clk_period : 1540000;

    for (int i = 0; i < WIRE1_TIMING_LEN; ++i) {
        delay[i] = standard_us[i] * 1000000 / period;
    }
}

/**
 * Write one bit.
 */
static inline void wire1_write_bit(int pin, uint32_t bit)
{
    if (bit) {
        REG_SET_FIELD(RTC_GPIO_OUT_W1TC_REG, RTC_GPIO_OUT_DATA_W1TC, BIT(pin));
        wait(delay[A]);
        REG_SET_FIELD(RTC_GPIO_OUT_W1TS_REG, RTC_GPIO_OUT_DATA_W1TS, BIT(pin));
        wait(delay[B]);
    } else {
        REG_SET_FIELD(RTC_GPIO_OUT_W1TC_REG, RTC_GPIO_OUT_DATA_W1TC, BIT(pin));
        wait(delay[C]);
        REG_SET_FIELD(RTC_GPIO_OUT_W1TS_REG, RTC_GPIO_OUT_DATA_W1TS, BIT(pin));
        wait(delay[D]);
    }
}

/**
 * Read one bit.
 */
static inline uint32_t wire1_read_bit(int pin)
{
    uint32_t result;

    // master pulls data line low
    REG_SET_FIELD(RTC_GPIO_OUT_W1TC_REG, RTC_GPIO_OUT_DATA_W1TC, BIT(pin));
    wait(delay[A]);

    // master releases the data line
    REG_SET_FIELD(RTC_GPIO_OUT_W1TS_REG, RTC_GPIO_OUT_DATA_W1TS, BIT(pin));

    // slave will send result: wait and sample
    wait(delay[E]);
    result = ulp_gpio_get_level(pin) & 0x1;
    wait(delay[F]);

    return result;
}

uint32_t wire1_reset(int pin)
{
    uint32_t success;

    wait(delay[G]);

    // master pulls data line low
    REG_SET_FIELD(RTC_GPIO_OUT_W1TC_REG, RTC_GPIO_OUT_DATA_W1TC, BIT(pin));
    wait(delay[H]);

    // master releases the data line
    REG_SET_FIELD(RTC_GPIO_OUT_W1TS_REG, RTC_GPIO_OUT_DATA_W1TS, BIT(pin));

    // slave will pull down data line: wait and sample
    wait(delay[I]);
    success = ulp_gpio_get_level(pin);
    wait(delay[J]);

    return success;
}

void wire1_write(int pin, uint32_t n, uint32_t data)
{
    // send LSB to MSB
    for (uint32_t i = 0; i < n; ++i) {
        wire1_write_bit(pin, data & 0x01);
        data >>= 1;
    }
}

uint32_t wire1_read(int pin, uint32_t n)
{
    uint32_t result = 0;
    uint32_t msb = 1 << n - 1;

    // bits arrive LSB to MSB
    for (uint32_t i = 0; i < n; ++i) {
        result >>= 1;
        if (wire1_read_bit(pin)) {
            result |= msb;
        }
    }

    return result;
}
