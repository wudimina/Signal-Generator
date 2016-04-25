
/** @file DAC_DRV.h
 *  @brief DAC Driver include file for the STM32F072RB.
 *  @author Dennis Law
 *  @date April 2016
 */

#ifndef DAC_DRV_H
#define DAC_DRV_H

#include "stm32f0xx.h"

#define DAC_RESOLUTION_8_MASK				0x000000FF
#define DAC_RESOLUTION_12_MASK				0x00000FFF

/** Enumaration for DAC resolution */
typedef enum DAC_resolution {
	DAC_RESOLUTION_8,
	DAC_RESOLUTION_12
} DAC_resolution_t;

/** Enumeration for DAC triggering */
typedef enum DAC_trigger {
	DAC_TRIGGER_NONE,
	DAC_TRIGGER_SOFTWARE,
	DAC_TRIGGER_TIMER6,
	DAC_TRIGGER_TIMER7
} DAC_trigger_t;

/** Enumeration for DMA enable setting */
typedef enum DAC_dma {
	DAC_DMA_DISABLE,
	DAC_DMA_ENABLE,
} DAC_dma_t;

/** Config structure for DAC */
struct DAC_config {
	DAC_trigger_t trig;
	DAC_dma_t dma;
};

int DAC_readSingle(int chn, uint32_t *output);
void DAC_readDual(uint32_t *output1, uint32_t *output2);
int DAC_writeSingle(int chn, uint32_t data, DAC_resolution_t res);
int DAC_writeDual(uint32_t data1, uint32_t data2, DAC_resolution_t res);

int DAC_setSoftwareTriggerSingle(int chn);
void DAC_setSoftwareTriggerDual(void);

int DAC_disable(int chn);
int DAC_enable(int chn);

int DAC_configTrigger(int chn, DAC_trigger_t trig);
int DAC_configDMA(int chn, DAC_dma_t dma);

int DAC_init(int chn, struct DAC_config conf);

#endif /* DAC_DRV_H */
