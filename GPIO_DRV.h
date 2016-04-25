
/** @file GPIO_DRV.h
 *  @brief GPIO Driver include file for the STM32F072RB.
 *  @author Dennis Law
 *  @date April 2016
 */
 
#ifndef GPIO_DRV_H
#define GPIO_DRV_H

#include "stm32f0xx.h"

/** Enumearation for GPIO logic levels */
typedef enum GPIO_outVal {
	GPIO_OUTVAL_LOW	 = 0,
	GPIO_OUTVAL_HIGH = 1
} GPIO_outVal_t;

/** Enumeration for GPIO direction configurations. */
typedef enum GPIO_dir {
	GPIO_DIR_INPUT,
	GPIO_DIR_OUTPUT
} GPIO_dir_t;

/** Enumeration for GPIO output type configurations. */
typedef enum GPIO_outType {
	GPIO_OUTTYPE_PUSHPULL,
	GPIO_OUTTYPE_OPENDRAIN
} GPIO_outType_t;

/** Enumeration for GPIO speed configurations. */
typedef enum GPIO_speed {
	GPIO_SPEED_SLOW,
	GPIO_SPEED_MEDIUM,
	GPIO_SPEED_FAST
} GPIO_speed_t;

/** Enumeration for GPIO pull resistor configurations */
typedef enum GPIO_pullRes {
	GPIO_PULLRES_DISABLED,
	GPIO_PULLRES_PULLUP,
	GPIO_PULLRES_PULLDOWN
} GPIO_pullRes_t;

/** Configuration structure used for pin initialization */
struct GPIO_config {
	GPIO_dir_t dir;
	GPIO_outType_t outType;
	GPIO_speed_t speed;
	GPIO_pullRes_t pullRes;
};

int GPIO_readPin(GPIO_TypeDef *gpio, int pinNum);
void GPIO_readPort(GPIO_TypeDef *gpio, uint16_t *val);
void GPIO_writePin(GPIO_TypeDef *gpio, int pinNum, GPIO_outVal_t val);
void GPIO_writePort(GPIO_TypeDef *gpio, uint16_t val);

void GPIO_enableClock(GPIO_TypeDef *gpio);
void GPIO_setDir(GPIO_TypeDef *gpio, int pinNum, GPIO_dir_t dir);
void GPIO_setOutType(GPIO_TypeDef *gpio, int pinNum, GPIO_outType_t type);
void GPIO_setSpeed(GPIO_TypeDef *gpio, int pinNum, GPIO_speed_t speed);
void GPIO_setPullRes(GPIO_TypeDef *gpio, int pinNum, GPIO_pullRes_t res);

void GPIO_initPin(GPIO_TypeDef *gpio, int pinNum, struct GPIO_config conf);

#endif	/* GPIO_DRV_H */
