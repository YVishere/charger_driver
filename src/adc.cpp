#include "adc.h"

ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef sConfig;

/* This function must be called in setup(). 
 * @param instance ADC#, where # is the handle number. 
 */
void initADC(ADC_TypeDef* instance) {
    AdcHandle.Instance = instance;

    //deinitialize ADC
    if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK) {
        Error_Handler();
    }

    AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;          /* Asynchronous clock mode, input ADC clock not divided */
    AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */
    AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
    AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
    AdcHandle.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */
    AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
    AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
    AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
    AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
    AdcHandle.Init.DMAContinuousRequests = DISABLE;                       /* DMA one-shot mode selected (not applied to this example) */
    AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
    AdcHandle.Init.OversamplingMode      = DISABLE;                       /* No oversampling */

    // initialize ADC
    if (HAL_ADC_Init(&AdcHandle) != HAL_OK) {
        Error_Handler();
    }

    /* Run the ADC calibration in single-ended mode */
    if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK) {
        Error_Handler();
    }

    /*##-2- Configure ADC regular channel ######################################*/
    sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
    sConfig.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;    /* Sampling time (number of clock cycles unit) */
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
    sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */ 
    sConfig.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */

}



/* Reads the analog value of the specified pin. 
 * @param channel ADC_CHANNEL_#, where # is the number of the channel
 * @return ADC voltage represented as a float in the range [0.0, 1.0]
 */
float readADC(uint32_t channel) {
     sConfig.Channel = channel;                /* Sampled channel number */
    // configure channel
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
        Error_Handler();
    }

     /*##-3- Start the conversion process #######################################*/
    if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
        Error_Handler();
    }

    /*##-4- Wait for the end of conversion #####################################*/
    /*  For simplicity reasons, this example is just waiting till the end of the
        conversion, but application may perform other tasks while conversion
        operation is ongoing. */
    if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK) {
        Error_Handler();
    }
    else {
        /* ADC conversion completed */
        /*##-5- Get the converted value of regular channel  ########################*/
        return (float)HAL_ADC_GetValue(&AdcHandle) / ADC_MAX_VALUE;
    }
    
} 