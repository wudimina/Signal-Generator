#include <math.h>

#include "WaveGen.h"


uint32_t DMAData[MAX_MEMORY_ALLOWED];

uint8_t IsParameterAllowed(enum WAVEFORM_TYPES waveform_types, uint32_t frequency, float amplitude)
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

static uint8_t ProcessWaveformParam(enum WAVEFORM_TYPES waveform_types, uint32_t frequency, float amplitude, uint32_t* pTiming_ns, uint32_t* pNoofSample)
{
	uint32_t period_in_ns;

	period_in_ns=1000000000/frequency;
	
	if(amplitude>MAX_AMPLITUDE_FLOAT||amplitude<MIN_AMPLITUDE_FLOAT)
	{
		return 0;
	}
	
	switch(waveform_types)
	{
		case WAVEFORM_TYPE_SINE:
		case WAVEFORM_TYPE_SAWTOOTH :
		case WAVEFORM_TYPE_TRIANGULAR:
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
		break;
		case WAVEFORM_TYPE_SQUARE:
			*pNoofSample=2;
			*pTiming_ns = period_in_ns/2;
		break;
		default:
			return 0;
	}
	
	return 1;
}

static void GenerateSawToothTable(uint32_t NoOfSample, uint32_t Amplitude_In_Resolution)
{
	uint32_t i;
	for(i=0;i<NoOfSample;i++)
	{
		DMAData[i]=(Amplitude_In_Resolution*i/NoOfSample);
	}
}

static void GenerateTriangularTable(uint32_t NoOfSample, uint32_t Amplitude_In_Resolution)
{
	uint32_t i;

	for(i=0;i<NoOfSample/2;i++)
	{
		DMAData[i]=2*(Amplitude_In_Resolution*i/NoOfSample);
	}

	for(i=0;i<NoOfSample/2;i++)
	{
		DMAData[i+NoOfSample/2]=Amplitude_In_Resolution-(2*(Amplitude_In_Resolution*i/NoOfSample));
	}
}

static void GenerateSineTable(uint32_t NoOfSample, uint32_t Amplitude_In_Resolution)
{
	uint32_t i;
	
	for(i=0;i<NoOfSample;i++)
	{
		DMAData[i]=(sin(i*2*PI_VALUE/NoOfSample)+1)*(Amplitude_In_Resolution+1)/2;
	}
}

static void GenerateSquareTable(uint32_t Amplitude_In_Resolution)
{
	DMAData[0]=0;
	DMAData[1]=Amplitude_In_Resolution;
}

static void GenerateWaveFormTable(enum WAVEFORM_TYPES waveform_types, uint32_t NoOfSample, uint32_t Amplitude_In_Resolution)
{
	switch (waveform_types)
	{
		case WAVEFORM_TYPE_SINE:
			GenerateSineTable(NoOfSample,Amplitude_In_Resolution);
		break;
		case WAVEFORM_TYPE_SAWTOOTH:
			GenerateSawToothTable(NoOfSample,Amplitude_In_Resolution);
		break;
		case WAVEFORM_TYPE_TRIANGULAR:
			GenerateTriangularTable(NoOfSample,Amplitude_In_Resolution);
		break;
		case WAVEFORM_TYPE_SQUARE:
			GenerateSquareTable(Amplitude_In_Resolution);
		break;
	}
}

static void ConfigureDAC(uint32_t noofsample, uint32_t periodinns)
{
	struct DAC_config dacConf;
	struct DMA_config dmaConf;
	struct TIMER_config timConf;
	
	uint32_t timercount;
	uint32_t timerprescalar;
	
	//disable all peripheral to make changes
	TIMER_disable(TIM6);
	DMA_disable(DMA_CHN);
	DAC_disable(DAC_CHN);
	

	/* Initialize DAC */
	dacConf.dma = DAC_DMA_ENABLE;
	dacConf.trig = DAC_TRIGGER_TIMER6;
	
	DAC_init(DAC_CHN, dacConf);
	DAC_enable(DAC_CHN);
	
	/* Initialize DMA */
	dmaConf.numWrite = noofsample;
	dmaConf.readMem = DMAData;
	dmaConf.writeMem = (uint32_t *)(&DAC->DHR12R1);

	DMA_init(DMA_CHN, dmaConf);
	DMA_enable(DMA_CHN);

	/* Initialize Timer */
	
	timerprescalar=1;
	//prescalar and count calculation
	while(1)
	{
		timercount=periodinns/(TIMER_TICK_NS*(timerprescalar+1));
		if(timercount>65535)
		{
			timerprescalar=timerprescalar*2;
		}
		else
		{
			break;
		}
	}
	
	
	timConf.count = timercount;
	timConf.prescale = timerprescalar;
	timConf.mode = TIMER_MODE_CONTINUOUS;
	timConf.mmode = TIMER_MASTERMODE_UPDATE;
	timConf.UGInt = TIMER_UGINTERRUPT_DISABLE;
	timConf.intEnable = false;
	
	TIMER_init(TIM6, timConf, NULL);
	TIMER_enable(TIM6);
}

static void DrawWaveform(enum WAVEFORM_TYPES waveform_types, uint32_t amplitude_in_resolution, uint32_t timing_ns,	uint32_t noOfSample)
{
	GenerateWaveFormTable(waveform_types,noOfSample,amplitude_in_resolution);
	ConfigureDAC(noOfSample, timing_ns);
}

void GenerateWaveform(enum WAVEFORM_TYPES waveform_types, uint32_t frequency, float amplitude)
{
	uint32_t timing_ns;
	uint32_t noOfSample;
	uint32_t amplitude_in_resolution;
	
	if(ProcessWaveformParam(waveform_types, frequency, amplitude, &timing_ns, &noOfSample))
	{
		amplitude_in_resolution = amplitude*DAC_RESOLUTION/DAC_VREF;
		DrawWaveform(waveform_types,amplitude_in_resolution,timing_ns,noOfSample);
	}
	else
	{
		TIMER_disable(TIM6);
	}
}

uint32_t GetMaxFreq(void)
{
		return MAX_FREQUENCY;
}

uint32_t GetMinFreq(void)
{
		return MIN_FREQUENCY;
}

float GetMaxAmplitude(void)
{
		return MAX_AMPLITUDE_FLOAT;
}

float GetMinAmplitude(void)
{
		return MIN_AMPLITUDE_FLOAT;
}

