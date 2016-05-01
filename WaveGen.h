#include "DAC_DRV.h"

enum
{
	WAVEFORM_TYPE_SINE=0,
	WAVEFORM_TYPE_SAWTOOTH,
	WAVEFORM_TYPE_TRIANGULAR,
	WAVEFORM_TYPE_SQUARE
};


#define MAX_AMPLITUDE_DECIMAL_PLACES 

#define MAX_AMPLITUDE_FLOAT		3.3
#define MIN_AMPLITUDE_FLOAT		1.0

#define MIN_SAMPLE_PER_CYCLE		10
#define DAC_SAMPLE_WAIT_TIME_NS		1000
#define DAC_SAMPLE_MAX_DRAG_TIME_NS	10000
#define MAX_MEMORY_ALLOWED			1000

extern uint8_t IsParameterAllowed(uint8_t waveform_types, uint32_t frequency, float amplitude);
extern void GenerateWaveform(uint8_t waveform_types, uint32_t frequency, float amplitude);
