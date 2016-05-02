#include "DAC_DRV.h"
#include "DMA_DRV.h"
#include "TIMER_DRV.h"


#define DAC_CHN			1
#define DMA_CHN			3

#define TIMER_TICK_NS		21
#define CLOCK_SPEED			42000000

#define DAC_RESOLUTION 4096
#define DAC_VREF				3.3



enum WAVEFORM_TYPES
{
	WAVEFORM_TYPE_SINE=0,
	WAVEFORM_TYPE_SAWTOOTH,
	WAVEFORM_TYPE_TRIANGULAR,
	WAVEFORM_TYPE_SQUARE
};

#define PI_VALUE 3.14159


#define MAX_AMPLITUDE_DECIMAL_PLACES 

#define MAX_AMPLITUDE_FLOAT		3.3
#define MIN_AMPLITUDE_FLOAT		1.0

#define MIN_SAMPLE_PER_CYCLE		50
#define DAC_SAMPLE_WAIT_TIME_NS		10000
#define DAC_SAMPLE_MAX_DRAG_TIME_NS	1000000
#define MAX_MEMORY_ALLOWED			2000

#define MAX_FREQUENCY (1000000000/(DAC_SAMPLE_WAIT_TIME_NS*MIN_SAMPLE_PER_CYCLE))
#define MIN_FREQUENCY 1

extern uint8_t IsParameterAllowed(enum WAVEFORM_TYPES waveform_types, uint32_t frequency, float amplitude);
extern void GenerateWaveform(enum WAVEFORM_TYPES waveform_types, uint32_t frequency, float amplitude);
extern uint32_t GetMaxFreq(void);
extern uint32_t GetMinFreq(void);
extern float GetMaxAmplitude(void);
extern float GetMinAmplitude(void);
