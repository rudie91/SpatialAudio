/*
 * HRTF2.c
 *
 *  Created on: Mar 29, 2018
 *      Author: Yoon mo Yang
 * This C script generates spatial audio output based on HRTF
 * when azimuth ranging from 0 degree to 360 degrees
 */
#include "L138_LCDK_aic3106_init.h"
#include "math.h"
#include "HRTF2D.txt"

#define LENGTH 50
float x[LENGTH];


int t = 0; //time index(ptr)
int i;
float L; // left_channel output
float R; // right_channel output
volatile int theta = 0;
int16_t Linput,Rinput;
AIC31_data_type codec_data;

interrupt void interrupt4(void) // interrupt service routine
{
	L = 0;
	R = 0;

//input the actual data
  codec_data.uint = input_sample();
//store input sample to a variable
    Linput = codec_data.channel[LEFT];
    Rinput = codec_data.channel[RIGHT];

    // downmix to mono
    x = 0.5*(Linput + Rinput);

    // Filter with hrtf coefficients
    for (i = 0; i < LENGTH; i++)
    {
      L += hrtf_l[theta][i]*x[(t + (LENGTH - i))%LENGTH];
      R += hrtf_r[theta][i]*x[(t + (LENGTH - i))%LENGTH];
    }

    t = (t + 1)%LENGTH;

// output
    codec_data.channel[LEFT] = L;
    codec_data.channel[RIGHT] = R;

//final output stage
  output_sample(codec_data.uint);
  return;
}

int main(void)
{
  // initialize input vectors
	memset(x,0,LENGTH*sizeof(float));
// interrupt function triggers runs each time an audio sample is received
	L138_initialise_intr(FS_22050_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
