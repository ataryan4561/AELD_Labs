

#include <stdio.h>
#include <stdbool.h>
#include "platform.h"
#include <stdlib.h>
#include <complex.h>
#include <xtime_l.h>
#include "fftsw.h"

// This function is designed to perform FFT operation on PS
// Default word length is single precision floating point
// You may explore double precision floating point and effect on execution time
void FFT_sw(float FFTIn_I[FFTSIZE], float FFTIn_R[FFTSIZE], float FFTOut_I[FFTSIZE], float FFTOut_R[FFTSIZE]);
int main()
{
    // Instead of constant input, we will generate the input for FFT
    float DataIN_R[FFTSIZE]; // Real part of the input
    float DataIN_I[FFTSIZE]; // Imaginary part of the input
    // Complex representation of the input
    float complex FFTIn_C[FFTSIZE];// input to the FFT in PS and PL

    for (int k = 0; k<FFTSIZE;k++){
    	FFTIn_C[k] = (k/4) + (k/2)*I;
    	DataIN_R[k] = creal(FFTIn_C[k]);
    	DataIN_I[k] = cimag(FFTIn_C[k]);
    }

    //FFT output
    float FFTOut_R[FFTSIZE];
    float FFTOut_I[FFTSIZE];

    // to store the time at which certain processes starts and ends
    XTime time_PS_start , time_PS_end;
    XTime_SetTime(0); // Reset the global timer
    //-------------------- PS PART --------------------------------//
    XTime_GetTime(&time_PS_start); // Capture the timer value at the start
    FFT_sw(DataIN_I, DataIN_R, FFTOut_I,FFTOut_R);
    XTime_GetTime(&time_PS_end); // Capture the timer value at the start


    // Print the FFT output on the UART
    printf("\n FFT output: \r\n");
    for (int j = 0 ; j < FFTSIZE ; j++)
    {
        printf("PS Output : %f + I%f \n " , FFTOut_R[j], FFTOut_I[j]);
        usleep(0.1); // Always add some buffer time between display
    }


    printf("\n-----------EXECUTION TIME--------------------\n");
    float time_processor = 0;
   	time_processor = (float)1.0 * (time_PS_end - time_PS_start) / (COUNTS_PER_SECOND/1000000);
   	printf("Execution Time for PS in Micro-Seconds : %f\n" , time_processor);

    return 0;

}

#define reverse(n) ((n & 0x1) << 9) | ((n & 0x2) << 7) | ((n & 0x4) << 5) | ((n & 0x8) << 3) | ((n & 0x10) << 1) | ((n & 0x20) >> 1) | ((n & 0x40) >> 3) | ((n & 0x80) >> 5) | ((n & 0x100) >> 7) | ((n & 0x200) >> 9)

void FFT_sw(float FFTIn_I[FFTSIZE], float FFTIn_R[FFTSIZE], float FFTOut_I[FFTSIZE], float FFTOut_R[FFTSIZE])
{

  DTYPE temp_R;   /*temporary storage complex variable*/
  DTYPE temp_I;   /*temporary storage complex variable*/

  int i,j;      /* loop indexes */
  int i_lower;    /* Index of lower point in butterfly */

  int stage;
  int subFFTSize; //Size of FFT in each stage of FFT
  int BFWidth;      /*Butterfly Width*/

  /*=====================BEGIN BIT REBERSAL===========================*/
  for (i = 0; i < FFTSIZE; ++i) {
    FFTOut_R[reverse(i)] = FFTIn_R[i];
    FFTOut_I[reverse(i)] = FFTIn_I[i];
  }
  /*++++++++++++++++++++++END OF BIT REVERSAL++++++++++++++++++++++++++*/

  /*=======================BEGIN: FFT=========================*/
  // Do FFTSTAGES of butterflies
  DTYPE BFWeight_R, BFWeight_I;
 // For N-point FFT, there are log2(N) stages
  stages:for(stage=1; stage<= FFTSTAGES; stage++)
  {
    subFFTSize = 1 << stage;    // DFT = 2^stage = points in sub DFT
    BFWidth = subFFTSize >> 1;       // Butterfly WIDTHS in sub-FFT (FFTSIZE of sub-FFT/2) no of weights

    // Perform butterflies for j-th stage
    // This loop runs for the iteration equal to BF width
    // In 4-point FFT, BF width is 1 in stage 1 and 2 in stage 2
    // In 8-point FFT, BF width is 1 in stage 1, 2 in stage 2 and 4 in stage 3
    butterfly:for(j=0; j<BFWidth; j++)
    {
    	//Note that weights of all butterfly units are same in a given stage
    	// We can reduce the number of memory read by using this for loop
    	BFWeight_R = W_real[j * (FFTSIZE>>stage)];
    	BFWeight_I = W_imag[j * (FFTSIZE>>stage)];

		// This loop is for all butterflies in a stage that use same W**k
		// In 4-point FFT, we have two BFs in stage 1
		// In 8-point FFT, we have four BFs in stage 1 and two BFs in stage 2
    	// Each butter fly weight affects two outputs and hence we have two outputs for each iteration
		subDFTSize:for(i =j ; i < FFTSIZE; i += subFFTSize) // This loop runs for FFTSIZE/SubFFTSize iterations
		  {
			i_lower = i + BFWidth;      //index of lower point in butterfly
			temp_R = FFTOut_R[i_lower] * BFWeight_R - FFTOut_I[i_lower] * BFWeight_I;
			temp_I = FFTOut_I[i_lower] * BFWeight_R + FFTOut_R[i_lower] * BFWeight_I;

			FFTOut_R[i_lower] = FFTOut_R[i] - temp_R;
			FFTOut_I[i_lower] = FFTOut_I[i] - temp_I;
			FFTOut_R[i] = FFTOut_R[i] + temp_R;
			FFTOut_I[i] = FFTOut_I[i] + temp_I;
		  }
	}
  }
}

