
/** @file GPIO_DRV.c
 *  @brief GPIO Driver for the STM32F072RB.
 *  @author Dennis Law
 *  @date April 2016
 */
 
#include "GPIO_DRV.h"

/** @brief Reads the input value of a GPIO pin.
 *	@param gpio Base pointer to the GPIO to be read.
 *	@param pinNum The pin number to the GPIO pin.
 *	@returns 1 if the GPIO is logic high and 0 if otherwise.
 */
int GPIO_readPin(GPIO_TypeDef *gpio, int pinNum)
{
	return (int)((gpio->IDR >> pinNum) & 0x000000001);
}

/** @brief Reads the input values of a PORT.
 *	@param gpio Base pointer to the GPIO to be read.
 *	@param val Container for storing the output value.
 */
void GPIO_readPort(GPIO_TypeDef *gpio, uint16_t *val)
{
	*val = (uint16_t)((gpio->IDR) & 0x0000FFFF);
}

/** @brief Writes a digital value to a pin of a GPIO.
 *	@param gpio The base pointer to the GPIO of interest.
 *	@param pinNum The pin number of the GPIO pin of interest.
 *	@param val The digital value to be written. The values for this
 *	parameter is defined in GPIO_outVal_t.
 */
void GPIO_writePin(GPIO_TypeDef *gpio, int pinNum, GPIO_outVal_t val)
{
	if (val == GPIO_OUTVAL_LOW)
		gpio->BSRR |= (1ul << (16+pinNum));
	else
		gpio->BSRR |= (1ul << pinNum);
}

/** @brief Writes a digital value to all pins of a port.
 *	@param gpio The base pointer to the GPIO of interest.
 *	@param The value to be written to the GPIO port. Each bit in this
 *	variable maps to the corresponding pin number of the GPIO in the
 *	same order.
 */
void GPIO_writePort(GPIO_TypeDef *gpio, uint16_t val)
{
	gpio->ODR = (uint32_t)val;
}

/** @brief Enables the clock to the selected GPIO.
 *	@param gpio The base pointer to the GPIO of interest.
 */
void GPIO_enableClock(GPIO_TypeDef *gpio)
{
	if (gpio == GPIOA)
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	else if (gpio == GPIOB)
		RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	else if (gpio == GPIOC)
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	else if (gpio == GPIOD)
		RCC->AHBENR |= RCC_AHBENR_GPIODEN;
	else if (gpio == GPIOE)
		RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
	else if (gpio == GPIOF)
		RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
}

/**	@brief Configures the direction of a GPIO pin.
 *	@param gpio The base pointer to the GPIO to configure.
 *	@param pinNum The pin number of the GPIO pin to configure.
 *	@param dir The desired direction of the pin. The values of this
 *	parameter is defined in GPIO_dir_t.
 */
void GPIO_setDir(GPIO_TypeDef *gpio, int pinNum, GPIO_dir_t dir)
{
	switch (dir) {
	case GPIO_DIR_INPUT:
		gpio->MODER	&= ~((3ul << 2*pinNum));
		break;
	case GPIO_DIR_OUTPUT:
		gpio->MODER	&= ~((3ul << 2*pinNum));
		gpio->MODER |=	((1ul << 2*pinNum));
		break;
	}
}

/** @brief Sets the output type of a GPIO pin.
 *	@param gpio The base pointer to the GPIO to configure.
 *	@param pinNum the pin number of the GPIO pin to configure.
 *	@param type The desired configuration for the output type. Values for
 *	this parameter are defined in GPIO_outType_t.
 */
void GPIO_setOutType(GPIO_TypeDef *gpio, int pinNum, GPIO_outType_t type)
{
	switch (type) {
	case GPIO_OUTTYPE_PUSHPULL:
		gpio->OTYPER &= ~((1ul << pinNum));
		break;
	case GPIO_OUTTYPE_OPENDRAIN:
		gpio->OTYPER |=  ((1ul << pinNum));
		break;
	}
}

/** @brief Sets up the GPIO pin speed.
 *	@param gpio The base pointer to the GPIO to configure.
 *	@param pinNum The pin number of the GPIO pin to configure.
 *	@param speed The desired speed configuration. The values for this
 *	parameter is defined in GPIO_speed_t.
 */
void GPIO_setSpeed(GPIO_TypeDef *gpio, int pinNum, GPIO_speed_t speed)
{
	switch (speed) {
	case GPIO_SPEED_SLOW:
		gpio->OSPEEDR &= ~((3ul << 2*pinNum));
		break;
	case GPIO_SPEED_MEDIUM:
		gpio->OSPEEDR &= ~((3ul << 2*pinNum));
		gpio->OSPEEDR |=  ((1ul << 2*pinNum));
		break;
	case GPIO_SPEED_FAST:
		gpio->OSPEEDR |=  ((3ul << 2*pinNum));
		break;
	}
}

/** @brief Sets up the pull resistor configuration of a GPIO pin.
 *	@param gpio The base pointer to the GPIO to configure.
 *	@param pinNum The pin number of the GPIO pin to configure.
 *	@param res The desired pull resistor configuration. The values
 *	for this parameter is defined in GPIO_pullRes_t.
 */
void GPIO_setPullRes(GPIO_TypeDef *gpio, int pinNum, GPIO_pullRes_t res)
{
	switch (res) {
	case GPIO_PULLRES_DISABLED:
		gpio->PUPDR   &= ~((3ul << 2*pinNum));
		break;
	case GPIO_PULLRES_PULLUP:
		gpio->PUPDR   &= ~((3ul << 2*pinNum));
		gpio->PUPDR   |=  ((1ul << 2*pinNum));
		break;
	case GPIO_PULLRES_PULLDOWN:
		gpio->PUPDR   &= ~((3ul << 2*pinNum));
		gpio->PUPDR   |=  ((2ul << 2*pinNum));
		break;
	}
}

/** @brief Initialized a GPIO pin
 *	@param gpio The base pointer to the GPIO to initialize.
 *	@param pinNum The number of the GPIO pin to initialize.
 *	@param config The configuration of the pin.
 */
void GPIO_initPin(GPIO_TypeDef *gpio, int pinNum, struct GPIO_config conf)
{
	GPIO_enableClock(gpio);
	GPIO_setDir(gpio, pinNum, conf.dir);
	GPIO_setOutType(gpio, pinNum, conf.outType);
	GPIO_setSpeed(gpio, pinNum, conf.speed);
	GPIO_setPullRes(gpio, pinNum, conf.pullRes);
}
