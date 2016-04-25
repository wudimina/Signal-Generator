
/** @file DMA_DRV.h
 *  @brief DMA Driver include file for the STM32F072RB.
 *  @author Dennis Law
 *  @date April 2016
 */
 
#ifndef DMA_DRV_H
#define DMA_DRV_H
 
#include "stm32f0xx.h"

/** Configuration structure for DMA setting up DMA */
struct DMA_config {
	int numWrite;
	uint32_t *readMem;
	uint32_t *writeMem;
};

int DMA_extractBasePointer(int chn, DMA_Channel_TypeDef **dma);
int DMA_disable(int chn);
int DMA_enable(int chn);
int DMA_init(int chn, struct DMA_config conf);

#endif	/* DMA_DRV_H */
