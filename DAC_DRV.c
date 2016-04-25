
/** @file DAC_DRV.c
 *  @brief DAC Driver for the STM32F072RB.
 *  @author Dennis Law
 *  @date April 2016
 */

#include <assert.h>
#include "DAC_DRV.h"

/**	@brief Reads the output register of a single channel.
 *	@param chn The channel to read. The value is either 1 or 2.
 *	@param output The container for storing the value of DAC_DOR1
 *	register.
 *	@returns Returns 0 if successful and -1 if otherwise.
 */
int DAC_readSingle(int chn, uint32_t *output)
{
	if ((chn != 1) && (chn != 2))
		return -1;

	if (chn == 1)
		*output = (DAC->DOR1 & 0x00000FFF);
	else
		*output = (DAC->DOR2 & 0x00000FFF);

	return 0;
}

/** @brief Reads the output registers of both DAC channels.
 *	@param output1 The container for storing the value of DAC_DOR1.
 *	@param output2 The container for storing the value of DAC_DOR2.
 */
void DAC_readDual(uint32_t *output1, uint32_t *output2)
{
	*output1 = (DAC->DOR1 & 0x00000FFF);
	*output2 = (DAC->DOR2 & 0x00000FFF);
}

/** @brief Writes an analog value to either channel 1 or 2.
 *	@param chn The channel to write data to. The value is either 1 or 2.
 *	@param data The data to be written to the data register.
 *	@param res The resolution of the data to be written.
 *	@returns Returns 0 if successful and -1 if otherwise.
 *
 *	@details The user input value is written to either the 8 bit or 12 bit
 *	right aligned registers depending on the value of res. The output
 *	voltage is calculated using the formula below.
 *
 *		DAC(output) = V(DDA) * DOR / 4095
 */
int DAC_writeSingle(int chn, uint32_t data, DAC_resolution_t res)
{
	if ((chn != 1) && (chn != 2))
		return -1;
	
	switch (res) {
	case DAC_RESOLUTION_8:
		if (chn == 1)
			DAC->DHR8R1 = (data & DAC_RESOLUTION_8_MASK);
		else
			DAC->DHR8R2 = (data & DAC_RESOLUTION_8_MASK);
		break;
	
	case DAC_RESOLUTION_12:
		if (chn == 1)
			DAC->DHR12R1 = (data & DAC_RESOLUTION_12_MASK);
		else
			DAC->DHR12R2 = (data & DAC_RESOLUTION_12_MASK);
		break;
		
	default:
		return -1;
	}

	return 0;
}

/** @brief Writes to both channel 1 and channel 2 of the DAC.
 *	@param data1 The data to be written to channel 1.
 *	@param data2 The data to be written to channle 2.
 *	@param res The resolution of the data to be written.
 *	@returns Returns 0 if successful and -1 if otherwise.
 *
 *	@details This function utilizes the right-aligned Dual Mode
 *	registers, DHR8RD and DHR12RD. The resolution used is dependent
 *	on the value of res. The output
 *	voltage is calculated using the formula below.
 *
 *		DAC(output) = V(DDA) * DOR / 4095
 */
int DAC_writeDual(uint32_t data1, uint32_t data2, DAC_resolution_t res)
{
	switch (res) {
	case DAC_RESOLUTION_8:
		DAC->DHR8RD = (data1 & DAC_RESOLUTION_8_MASK) |
						((data2 & DAC_RESOLUTION_8_MASK) << 8);
		break;

	case DAC_RESOLUTION_12:
		DAC->DHR12RD = (data1 & DAC_RESOLUTION_12_MASK) |
						((data2 & DAC_RESOLUTION_12_MASK) << 16);
		break;

	default:
		return -1;
	}

	return 0;
}

/** @brief Disables the selected channel.
 *	@param chn The channe to disable. This value is either 1 or 2.
 *	@returns 0 if successful and -1 if otherwise.
 */
int DAC_disable(int chn)
{
	if ((chn != 1) && (chn != 2))
		return -1;
	
	if (chn == 1)
		DAC->CR &= ~(DAC_CR_EN1);
	else
		DAC->CR &= ~(DAC_CR_EN2);
	
	return 0;
}

/** @brief Enables the selected channel.
 *	@param chn The channel to enable. The value is either 1 or 2.
 *	@returns Returns 0 if successful and -1 if otherwise.
 */
int DAC_enable(int chn)
{
	if ((chn != 1) && (chn != 2))
		return -1;
	
	if (chn == 1)
		DAC->CR |= DAC_CR_EN1;
	else
		DAC->CR |= DAC_CR_EN2;
	
	return 0;
}

/**	@brief Sets the software trigger for the specified channel.
 *	@param chn The desired channel to trigger. It can be either 0 or 1.
 *
 *	@note Software triggering has to be configured for the desired
 *	channel during initialization.
 */
int DAC_setSoftwareTriggerSingle(int chn)
{
	if ((chn != 1) && (chn != 2))
		return -1;

	if (chn == 1)
		DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;
	else
		DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG2;

	return 0;
}

/** @brief Sets the software trigger for both DAC channels
 *	@note Software triggering has to be configured for both channels
 *	during initialization.
 */
void DAC_setSoftwareTriggerDual(void)
{
	DAC->SWTRIGR |= (DAC_SWTRIGR_SWTRIG1 | DAC_SWTRIGR_SWTRIG2);
}

/** @brief Initializes triggering for a DAC channel.
 *	@param chn The channel to configure. The value for this argument
 *	can either be 1 or 2.
 *	@param trig The type trigger to configure for the channel.
 * 	@returns 0 if successful and -1 if otherwise.
 */
int DAC_configTrigger(int chn, DAC_trigger_t trig)
{
	if ((chn != 1) && (chn != 2))
		return -1;

	if (chn == 1) {
		switch (trig) {
		case DAC_TRIGGER_NONE:
			DAC->CR &= ~(DAC_CR_TEN1);
			break;
		case DAC_TRIGGER_SOFTWARE:
			DAC->CR |= DAC_CR_TSEL1;
			DAC->CR |= DAC_CR_TEN1;
			break;
		case DAC_TRIGGER_TIMER6:
			DAC->CR &= ~(DAC_CR_TSEL1);
			DAC->CR |= DAC_CR_TEN1;
			break;
		case DAC_TRIGGER_TIMER7:
			DAC->CR &= ~(DAC_CR_TSEL1);
			DAC->CR |= DAC_CR_TSEL1_1;
			DAC->CR |= DAC_CR_TEN1;
			break;
		default:
			break;
		}
	} else if (chn == 2) {
		switch (trig) {
		case DAC_TRIGGER_NONE:
			DAC->CR &= ~(DAC_CR_TEN2);
			break;
		case DAC_TRIGGER_SOFTWARE:
			DAC->CR |= DAC_CR_TSEL2;
			DAC->CR |= DAC_CR_TEN2;
			break;
		case DAC_TRIGGER_TIMER6:
			DAC->CR &= ~(DAC_CR_TSEL2);
			DAC->CR |= DAC_CR_TEN2;
			break;
		case DAC_TRIGGER_TIMER7:
			DAC->CR &= ~(DAC_CR_TSEL2);
			DAC->CR |= DAC_CR_TSEL2_1;
			DAC->CR |= DAC_CR_TEN2;
			break;
		default:
			break;	
		}
	}

	return 0;
}

/**	@brief Enables or disables the DMA.
 *	@param chn The channel to configure.
 *	@dma The setting to either enable or disable the DMA.
 */
int DAC_configDMA(int chn, DAC_dma_t dma)
{
	/* Check if channel index is correct */
	if ((chn != 1) && (chn != 2))
		return -1;
	
	if (chn == 1) {
		switch (dma) {
		case DAC_DMA_DISABLE:
			DAC->CR &= ~(DAC_CR_DMAEN1);
			break;
		case DAC_DMA_ENABLE:
			DAC->CR |= DAC_CR_DMAEN1;
			break;
		default:
			return -1;
		}
	} else {
		switch (dma) {
		case DAC_DMA_DISABLE:
			DAC->CR &= ~(DAC_CR_DMAEN2);
			break;
		case DAC_DMA_ENABLE:
			DAC->CR |= DAC_CR_DMAEN2;
			break;
		default:
			return -1;
		}
	}
	
	return 0;
}

/**	@brief Initializes DAC.
 *	@param chn The channel to initialize. The value is either 1 or 2.
 *	@param conf Config structure for configuring the DAC.
 *	@returns Returns 0 if successful and -1 if otherwise.
 *
 *	@details Initializes either channel 1 or 2 of the DAC.
 */
int DAC_init(int chn, struct DAC_config conf)
{
	/* Check if channel index is correct */
	if ((chn != 1) && (chn != 2))
		return -1;

	/* Enable clock for DAC peripheral */
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	
	DAC_configTrigger(chn, conf.trig);
	DAC_configDMA(chn, conf.dma);
	
	if (chn == 1) {
		GPIOA->MODER |= GPIO_MODER_MODER4; 		/* Set pin as AIN */
		GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4); 	/* Disable pull resistor */
		DAC->CR |= DAC_CR_BOFF1; 	/* Enable output buffer */
	}
	else if (chn == 2) {
		GPIOA->MODER |= GPIO_MODER_MODER5; 		/* Set pin as AIN */
		GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5); 	/* Disable pull resistor */
		DAC->CR |= DAC_CR_BOFF2; 	/* Enable output buffer */
	}
	
	return 0;
}
