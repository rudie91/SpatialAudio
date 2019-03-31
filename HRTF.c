/*
 * HRTF.c
 *
 *  Created on: Mar 29, 2018
 *      Author: Yoon mo Yang
 * This C script generates spatial audio output based on HRTF
 * when azimuth = 120 degrees
 */

#include "L138_LCDK_aic3106_init.h"
#include "math.h"
#include "HRTF.txt"
#define LENGTH 50
float x_l[LENGTH];
float x_r[LENGTH];

int t = 0;
int i;
float L;
float R;

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

  for (i = 1; i < LENGTH; i++)
  {
	  L += hrtf_l8[0]*x[t] + hrtf_l8[i]*x[(t + (LENGTH - i))%LENGTH];
	  R += hrtf_r8[0]*x[t] + hrtf_r8[i]*x[(t + (LENGTH - i))%LENGTH];
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
	memset(x,0,LENGTH*sizeof(float));
  // interrupt function triggers runs each time an audio sample is received
	L138_initialise_intr(FS_22050_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
