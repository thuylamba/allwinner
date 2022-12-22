#ifndef __F1C100S_CLOCK_H__
#define __F1C100S_CLOCK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "io.h"
uint32_t clk_sdc_config(uint32_t reg, uint32_t freq);

void clk_reset_set(uint32_t reg, uint8_t bit);
void clk_reset_clear(uint32_t reg, uint8_t bit);
void clk_enable(uint32_t reg, uint8_t bit);
void clk_disable(uint32_t reg, uint8_t bit);
#ifdef __cplusplus
}
#endif

#endif /* __F1C100S_CLOCK_H__ */
