/*
 * sys-clock.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

//#include <xboot.h>
#include <stdint.h>
#include <f1c100s/reg-ccu.h>
#include <io.h>

static inline void sdelay(int loops)
{
	__asm__ __volatile__ ("1:\n" "subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0"(loops));
}

static void wait_pll_stable(uint32_t base)
{
	uint32_t rval = 0;
	uint32_t time = 0xfff;

	do {
		rval = read32(base);
		time--;
	} while(time && !(rval & (1 << 28)));
}

static void clock_set_pll_cpu(uint32_t clk)
{
	uint32_t n, k, m, p;
	uint32_t rval = 0;
	uint32_t div = 0;

	if(clk > 720000000)
		clk = 720000000;

	if((clk % 24000000) == 0)
	{
		div = clk / 24000000;
		n = div - 1;
		k = 0;
		m = 0;
		p = 0;
	}
	else if((clk % 12000000) == 0)
	{
		m = 1;
		div = clk / 12000000;
		if((div % 3) == 0)
			k = 2;
		else if((div % 4) == 0)
			k = 3;
		else
			k = 1;
		n = (div / (k + 1)) - 1;
		p = 0;
	}
	else
	{
		div = clk / 24000000;
		n = div - 1;
		k = 0;
		m = 0;
		p = 0;
	}

	rval = read32(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL);
	rval &= ~((0x3 << 16) | (0x1f << 8) | (0x3 << 4) | (0x3 << 0));
	rval |= (1U << 31) | (p << 16) | (n << 8) | (k << 4) | m;
	write32(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL, rval);
	wait_pll_stable(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL);
}
void clk_reset_set(uint32_t reg, uint8_t bit)
{
    clear32(F1C100S_CCU_BASE+reg, (1 << bit));
}

void clk_reset_clear(uint32_t reg, uint8_t bit)
{
    set32(F1C100S_CCU_BASE+reg, (1 << bit));
}
void clk_enable(uint32_t reg, uint8_t bit)
{
    set32(F1C100S_CCU_BASE+reg, (1 << bit));
}
void clk_disable(uint32_t reg, uint8_t bit)
{
    clear32(F1C100S_CCU_BASE+reg, (1 << bit));
}

// SD card controller clock
static uint32_t pll_periph_get_freq(void) // +
{
    uint32_t reg = read32(F1C100S_CCU_BASE + CCU_PLL_PERIPH_CTRL);

    uint32_t mul = (reg >> 8) & 0x1F;
    uint32_t div = (reg >> 4) & 0x3;

    return (24000000 * (mul + 1) / (div + 1));
}
uint32_t clk_sdc_config(uint32_t reg, uint32_t freq)
{
    uint32_t in_freq = 0;
    uint32_t reg_val = (1 << 31);

    if (freq <= 24000000)
    {
        reg_val |= (0 << 24); // OSC24M
        in_freq = 24000000;
    }
    else
    {
        reg_val |= (1 << 24); // PLL_PERIPH
        in_freq = pll_periph_get_freq();
    }

    uint8_t div = in_freq / freq;
    if (in_freq % freq)
        div++;

    uint8_t prediv = 0;
    while (div > 16)
    {
        prediv++;
        if (prediv > 3)
            return 0;
        div = (div + 1) / 2;
    }

    /* determine delays */
    uint8_t samp_phase = 0;
    uint8_t out_phase = 0;
    if (freq <= 400000)
    {
        out_phase = 0;
        samp_phase = 0;
    }
    else if (freq <= 25000000)
    {
        out_phase = 0;
        samp_phase = 5;
    }
    else if (freq <= 52000000)
    {
        out_phase = 3;
        samp_phase = 4;
    }
    else /* freq > 52000000 */
    {
        out_phase = 1;
        samp_phase = 4;
    }
    reg_val |= (samp_phase << 20) | (out_phase << 8);
    reg_val |= (prediv << 16) | ((div - 1) << 0);

    write32(F1C100S_CCU_BASE+reg, reg_val);

    return in_freq / div;
}
void sys_clock_init(void)
{
	uint32_t val;

	write32(F1C100S_CCU_BASE + CCU_PLL_STABLE_TIME0, 0x1ff);
	write32(F1C100S_CCU_BASE + CCU_PLL_STABLE_TIME1, 0x1ff);

	val = read32(F1C100S_CCU_BASE + CCU_CPU_CFG);
	val &= ~(0x3 << 16);
	val |= (0x1 << 16);
	write32(F1C100S_CCU_BASE + CCU_CPU_CFG, val);
	sdelay(100);

	write32(F1C100S_CCU_BASE + CCU_PLL_VIDEO_CTRL, 0x81004107);
	sdelay(100);
	write32(F1C100S_CCU_BASE + CCU_PLL_PERIPH_CTRL, 0x80041800);
	sdelay(100);
	write32(F1C100S_CCU_BASE + CCU_AHB_APB_CFG, 0x00003180);
	sdelay(100);

	val = read32(F1C100S_CCU_BASE + CCU_DRAM_CLK_GATE);
	val |= (0x1 << 26) | (0x1 << 24);
	write32(F1C100S_CCU_BASE + CCU_DRAM_CLK_GATE, val);
	sdelay(100);

	clock_set_pll_cpu(408000000);
	val = read32(F1C100S_CCU_BASE + CCU_CPU_CFG);
	val &= ~(0x3 << 16);
	val |= (0x2 << 16);
	write32(F1C100S_CCU_BASE + CCU_CPU_CFG, val);
	sdelay(100);
}
