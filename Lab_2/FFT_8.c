/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <xtime_l.h> // Timer for execution time calculations

#define FFT_Size 8


const float complex twiddle_factors[FFT_Size/2] = {1-0*I, 0.7071067811865476-0.7071067811865475*I, 0.0-1*I,  -0.7071067811865475-0.7071067811865476*I};

// This function reorders the input to get the output in the normal order
// Refer the handout for the desired input order
const int input_reorder[FFT_Size] = {0, 4, 2, 6, 1, 5, 3, 7};
void InputReorder(float complex dataIn[FFT_Size], float complex dataOut[FFT_Size])
{
	for (int i = 0 ; i < FFT_Size ; i++)
	{
		dataOut[i] = dataIn[input_reorder[i]];
	}
}

// For FFT of size FFT_Size, the number of butterfly stages are 2^stages = FFT_Size.
// For 8-point FFT, there are three butterfly stages.
void FFTStages(float complex FFT_input[FFT_Size], float complex FFT_output[FFT_Size])
{
	float complex stage1_out[FFT_Size], stage2_out[FFT_Size];
	// Stage 1
	for (int i = 0 ; i < FFT_Size ; i=i+2)
	{
		stage1_out[i] = FFT_input[i] + FFT_input[i+1];
		stage1_out[i+1] = FFT_input[i] - FFT_input[i+1];
	}
	// Stage 2
	for (int i = 0 ; i < FFT_Size ; i=i+4)
	{
		for (int j = 0; j < 2; ++j)
		{
			stage2_out[i+j] = stage1_out[i+j] + twiddle_factors[2*j]*stage1_out[i+j+2];
			stage2_out[i+2+j] = stage1_out[i+j] - twiddle_factors[2*j]*stage1_out[i+j+2];
		}

	}
	// Stage 3
	for (int i = 0 ; i < FFT_Size/2 ; i++)
	{
		FFT_output[i] = stage2_out[i] + twiddle_factors[i]*stage2_out[i+4];
		FFT_output[i+4] = stage2_out[i] - twiddle_factors[i]*stage2_out[i+4];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	// For FFT_Size point FFT, define the input.
	// You may modify the code to take the input from user via UART
    float complex FFT_input[FFT_Size] = {11+23*I,32+10*I,91+94*I,15+69*I,47+96*I,44+12*I,96+17*I,49+58*I};
    // FFT output will be stored in this variable
    float complex FFT_output[FFT_Size];
    // Variable for intermediate outputs
    float complex FFT_rev[FFT_Size];

    // to store the time at which certain processes starts and ends
    XTime time_PS_start , time_PS_end;

    // Print the FFT input on the UART
    printf("\n FFT input: \r\n");
	  for (int i = 0 ; i < FFT_Size ; i++)
	  {
		  printf("%f %f\n", crealf(FFT_input[i]), cimagf(FFT_input[i]));
	  }

	 XTime_SetTime(0);
     XTime_GetTime(&time_PS_start); // Capture the timer value at the start
    // As discussed in the handout, FFT involves two tasks:
    // 1) Reorder of the inputs to get output in the normal order
    // 2) Multiplications using multi-stage butterfly approach
	InputReorder(FFT_input, FFT_rev); // Task 1
    FFTStages(FFT_rev, FFT_output); // Task 2
    XTime_GetTime(&time_PS_end); // Capture the timer value at the start

    // Print the FFT output on the UART
    printf("\n FFT output: \r\n");
    for (int i = 0 ; i < FFT_Size ; i++)
    {
        printf("%f %f\n", crealf(FFT_output[i]), cimagf(FFT_output[i]));
    }

    printf("\n-----------EXECUTION TIME--------------------\n");
    float time_processor = 0;
	time_processor = (float)1.0 * (time_PS_end - time_PS_start) / (COUNTS_PER_SECOND/1000000);
	printf("Execution Time for PS in Micro-Seconds : %f\n" , time_processor);

    // Modify this code for large size FFT
    // How you can generalize the code for any FFT size (limited to power of two)
    // Receive the FFT size and FFT input from User
}


// Equivalent Matlab Code for FFT
// clc;
// clear;
// FFT_Size =8;
// input_real =[11,32,91,15,47,44,96,49];
// input_imag =[23,10,94,69,96,12,17,58];
// FFT_input =complex(input_real,input_imag);
// FFT_out=fft(FFT_input,FFT_Size);
// disp(real(FFT_out));
// disp(imag(FFT_out));
