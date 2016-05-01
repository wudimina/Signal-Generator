#include "WaveGen.h"

uint8_t IsParameterAllowed(uint8_t waveform_types, uint32_t frequency, float amplitude)
{
	uint32_t numberOfSample;
	uint32_t period_in_ns;
	
	period_in_ns=1000000000/frequency;
	
	if(amplitude>MAX_AMPLITUDE_FLOAT||amplitude<MIN_AMPLITUDE_FLOAT)
	{
		return 0;
	}
	
	numberOfSample = period_in_ns/DAC_SAMPLE_WAIT_TIME_NS;
	
	if(numberOfSample<MIN_SAMPLE_PER_CYCLE)
		return 0;
		
	if(numberOfSample>MAX_MEMORY_ALLOWED)
	{
		numberOfSample=period_in_ns/DAC_SAMPLE_MAX_DRAG_TIME_NS;
		if(numberOfSample>MAX_MEMORY_ALLOWED)
		{
			return 0;
		}
	}

		
	return 1;
}

static uint8_t ProcessWaveformParam(uint8_t waveform_types, uint32_t frequency, float amplitude, uint32_t* pTiming_ns, uint32_t* pNoofSample)
{
	uint32_t period_in_ns;

	period_in_ns=1000000000/frequency;
	
	if(amplitude>MAX_AMPLITUDE_FLOAT||amplitude<MIN_AMPLITUDE_FLOAT)
	{
		return 0;
	}
	
	*pNoofSample = period_in_ns/DAC_SAMPLE_WAIT_TIME_NS;
	
	if(*pNoofSample<MIN_SAMPLE_PER_CYCLE)
		return 0;
		
	if(*pNoofSample>MAX_MEMORY_ALLOWED)
	{
		*pNoofSample=period_in_ns/DAC_SAMPLE_MAX_DRAG_TIME_NS;
		if(*pNoofSample>MAX_MEMORY_ALLOWED)
		{
			return 0;
		}
		else
		{
			*pTiming_ns=period_in_ns/MAX_MEMORY_ALLOWED;
			*pNoofSample=MAX_MEMORY_ALLOWED;
		}
	}
	else
	{
		//timing = DAC_SAMPLE_WAIT_TIME_MS;
		*pTiming_ns =  period_in_ns/(*pNoofSample);
	}
		
	return 1;
}


static void DrawWaveform(uint8_t waveform_types, float amplitude, uint32_t timing_ns,	uint32_t noOfSample)
{
	return;
}

void GenerateWaveform(uint8_t waveform_types, uint32_t frequency, float amplitude)
{
	uint32_t timing_ns;
	uint32_t noOfSample;
	
	if(ProcessWaveformParam(waveform_types, frequency, amplitude, &timing_ns, &noOfSample))
	{
		DrawWaveform(waveform_types,amplitude,timing_ns,noOfSample);
	}
}

