/**
 * @file wire1.h
 *
 * 1-Wire communication bus.
 *
 * @see https://www.maximintegrated.com/en/design/technical-documents/app-notes/1/126.html
 */

#pragma once

/**< Labels of 1-wire timing constants */
enum wire1_timing_index { A, B, C, D, E, F, G, H, I, J, WIRE1_TIMING_LEN };

typedef enum { WIRE1_PRESENCE, WIRE1_EMPTY } wire1_reset_t;

/**< 1-Wire standard speed timing constants */
#define WIRE1_A 6
#define WIRE1_B 64
#define WIRE1_C 60
#define WIRE1_D 10
#define WIRE1_E 9
#define WIRE1_F 55
#define WIRE1_G 0
#define WIRE1_H 480
#define WIRE1_I 70
#define WIRE1_J 410

/**
 * Initialize timing delays.
 *
 * @param clk_period ULP clock period (ps/tick)
 */
void wire1_set_timing_delay(uint32_t clk_period);

/**
 * Reset (initialize) 1-Wire bus.
 *
 * @param pin RTC GPIO number
 * @return Returns 0 on success (slave device is detected), otherwise 1.
 */
uint32_t wire1_reset(int pin);

/**
 * Write data.
 *
 * @param pin RTC GPIO number
 * @param n Number of bits to write [0-32]
 * @param data Data to write
 */
void wire1_write(int pin, uint32_t n, uint32_t data);

/**
 * Read data.
 *
 * @param pin RTC GPIO number
 * @param n Number of bits to read [0-32]
 * @return Data read is stored in bits [0:n-1]
 */
uint32_t wire1_read(int pin, uint32_t n);
