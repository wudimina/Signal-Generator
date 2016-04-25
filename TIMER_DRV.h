
/** @file TIMER_DRV.h
 *  @brief TIMER Driver include file for the STM32F072RB.
 *  @author Dennis Law
 *  @date April 2016
 */
 
#ifndef TIMER_DRV_H
#define TIMER_DRV_H

#include <stdio.h>
#include <stdbool.h>
#include "stm32f0xx.h"

/** Enumeration for Timer Modes */
typedef enum TIMER_mode {
	TIMER_MODE_ONEPULSE,
	TIMER_MODE_CONTINUOUS
} TIMER_mode_t;

/** Enumeration for Master Mode */
typedef enum TIMER_masterMode {
	TIMER_MASTERMODE_RESET,
	TIMER_MASTERMODE_ENABLE,
	TIMER_MASTERMODE_UPDATE
} TIMER_masterMode_t;

/** Enumeration for configurign UG Interrupt
 *	@note If set to enable, setting an event will cause
 *	an interrupt to trigger.
 */
typedef enum TIMER_UGInterrupt {
	TIMER_UGINTERRUPT_ENABLE,
	TIMER_UGINTERRUPT_DISABLE
} TIMER_UGInterrupt_t;

/** Configuration parameters for setting up the timer. */
struct TIMER_config {
	uint16_t count;
	uint16_t prescale;
	TIMER_mode_t mode;
	TIMER_masterMode_t mmode;
	TIMER_UGInterrupt_t UGInt;
	bool intEnable;
};

int TIMER_generateEvent(TIM_TypeDef *tim);
int TIMER_disable(TIM_TypeDef *tim);
int TIMER_enable(TIM_TypeDef *tim);

int TIMER_setMode(TIM_TypeDef *tim, TIMER_mode_t mode);
int TIMER_setMasterMode(TIM_TypeDef *tim, TIMER_masterMode_t mmode);
int TIMER_setCount(TIM_TypeDef *tim, int16_t val);
int TIMER_setPrescaler(TIM_TypeDef *tim, int16_t val);
int TIMER_setUGInterrupt(TIM_TypeDef *tim, TIMER_UGInterrupt_t UGInt);
int TIMER_enableClock(TIM_TypeDef *tim);

int TIMER_init(TIM_TypeDef *tim, struct TIMER_config conf,
				void (*callback)(void));

#endif	/* TIMER_DRV_H */
