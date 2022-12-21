#ifndef __F1C100S_REG_CCU_H__
#define __F1C100S_REG_CCU_H__

#define F1C100S_CCU_BASE		(0x01c20000)

enum
{
    CCU_PLL_CPU_CTRL     = 0x000,
    CCU_PLL_AUDIO_CTRL   = 0x008,
    CCU_PLL_VIDEO_CTRL   = 0x010,
    CCU_PLL_VE_CTRL      = 0x018,
    CCU_PLL_DDR_CTRL     = 0x020,
    CCU_PLL_PERIPH_CTRL  = 0x028,
    CCU_CPU_CFG          = 0x050,
    CCU_AHB_APB_CFG      = 0x054,

    CCU_BUS_CLK_GATE0    = 0x060,
    CCU_BUS_CLK_GATE1    = 0x064,
    CCU_BUS_CLK_GATE2    = 0x068,

    CCU_SDMMC0_CLK       = 0x088,
    CCU_SDMMC1_CLK       = 0x08c,
    CCU_DAUDIO_CLK       = 0x0b0,
    CCU_SPDIF_CLK        = 0x0b4,
    CCU_I2S_CLK          = 0x0b8,
    CCU_USBPHY_CFG       = 0x0cc,
    CCU_DRAM_CLK_GATE    = 0x100,
    CCU_DEBE_CLK         = 0x104,
    CCU_DEFE_CLK         = 0x10c,
    CCU_TCON_CLK         = 0x118,
    CCU_DEINTERLACE_CLK  = 0x11c,
    CCU_TVE_CLK          = 0x120,
    CCU_TVD_CLK          = 0x124,
    CCU_CSI_CLK          = 0x134,
    CCU_VE_CLK           = 0x13c,
    CCU_ADDA_CLK         = 0x140,
    CCU_AVS_CLK          = 0x144,

    CCU_PLL_STABLE_TIME0 = 0x200,
    CCU_PLL_STABLE_TIME1 = 0x204,
    CCU_PLL_CPU_BIAS     = 0x220,
    CCU_PLL_AUDIO_BIAS   = 0x224,
    CCU_PLL_VIDEO_BIAS   = 0x228,
    CCU_PLL_VE_BIAS      = 0x22c,
    CCU_PLL_DDR0_BIAS    = 0x230,
    CCU_PLL_PERIPH_BIAS  = 0x234,
    CCU_PLL_CPU_TUN      = 0x250,
    CCU_PLL_DDR_TUN      = 0x260,
    CCU_PLL_AUDIO_PAT    = 0x284,
    CCU_PLL_VIDEO_PAT    = 0x288,
    CCU_PLL_DDR0_PAT     = 0x290,

    CCU_BUS_SOFT_RST0    = 0x2c0,
    CCU_BUS_SOFT_RST1    = 0x2c4,
    CCU_BUS_SOFT_RST2    = 0x2d0,
};
#endif /* __F1C100S_REG_CCU_H__ */
