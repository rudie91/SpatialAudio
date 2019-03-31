/*
 * amplitude_IIDITD.c
 *
 *  Created on: Mar 29, 2018
 *      Author: Yoon mo Yang
 * This C script generates patial audio output which is based on
 * amplitude panning and IID&ITD.
 */
#include "L138_LCDK_aic3106_init.h"
#include <stdio.h>
#include "math.h"
#define PI 3.14159
#define FS 44100

int16_t Linput,Rinput;
AIC31_data_type codec_data;

// output
float y_L;
float y_R;

//amplitude panning: uncomment the line below
//volatile float lambda;

//IID and ITD
volatile float azimuth;
float a = 0.11; // half of the distance b/w the two ears
float c = 343;
int ITD;
float IID_C; //Close side from the sound source
float IID_F; //Far side from the sound source
//int M = 37; // when ITD is max: 0.11/343*(pi/2+sin(pi/2))*fs
int ptr = 0;
float x_L[37];
float x_R[37];

interrupt void interrupt4(void) // interrupt service routine
{

// input the actual data
  codec_data.uint = input_sample();
// store input sample to a variable
    Linput = codec_data.channel[LEFT];
    Rinput = codec_data.channel[RIGHT];

// downmix to mono
    x = 0.5*(Linput + Rinput); 

/*
// amplitude panning: uncomment the line below
    y_L = x*cosf(lambda*(float)PI/2);
    y_R = x*sinf(lambda*(float)PI/2);*/

// IID & ITD
    ITD = round(a/c*(azimuth+sinf(azimuth))*FS); //time difference
    if(azimuth == 0) //when there is no angle: sound source is in front of the listener
    {
    	y_L =(float) x;
    	y_R =(float) x;
    }

    else if(azimuth > 0) // sound source is on the right side of the listener
    {
    	IID_C = (1+1/3*sinf(azimuth)); //amplitude for right (closer)
    	IID_F = (1-1/3*sinf(azimuth)); //amplitude for left (far)

    	x_R[ptr] = (float)x;
    	y_R = IID_C*x_R[ptr]; //no delay

    	x_L[ptr] = (float)x;
        y_L = IID_F*x_L[(ptr+1)%(ITD)]; //delay on the left
    	ptr = (ptr+1)%ITD;

    }

    else //sound source is on the left side of the listener
    {
    	IID_C = (1-1/3*sinf(azimuth)); //amplitude for left (closer)
    	IID_F = (1+1/3*sinf(azimuth)); //amplitude for right (far)
    	x_R[ptr] = (float)x;
    	y_R = x_R[(ptr+1)%(ITD)]*IID_F; //delay on the right

    	x_L[ptr] = (float)x;
    	y_L = IID_C*x_L[ptr]; //no delay
    	ptr = (ptr+1)%ITD;
    }

// output
    codec_data.channel[LEFT] =  (int16_t)y_L;
    codec_data.channel[RIGHT] = (int16_t)y_R;

//final output stage
  output_sample(codec_data.uint);
  return;
}

int main(void)
{
	memset(x_L,0,37*sizeof(float));
	memset(x_R,0,37*sizeof(float));

	// interrupt function triggers runs each time an audio sample is received
	L138_initialise_intr(FS_44100_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
	while(1);
}


