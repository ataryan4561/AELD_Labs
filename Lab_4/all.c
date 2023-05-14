#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include<math.h>
#include <xtime_l.h>
#include "xparameters.h"
#include "xaxidma.h"
#define FFT_Size 512
#define FFTSTAGES 9
typedef float DTYPE;
typedef unsigned int INTTYPE;
const DTYPE W_real[]={1.0, 0.9999247018391445, 0.9996988186962042, 0.9993223845883495, 0.9987954562051724, 0.9981181129001492, 0.9972904566786902, 0.996312612182778, 0.9951847266721969, 0.9939069700023561, 0.99247953459871, 0.99090263542778, 0.989176509964781, 0.9873014181578584, 0.9852776423889412, 0.9831054874312163, 0.9807852804032304, 0.9783173707196277, 0.9757021300385286, 0.9729399522055602, 0.970031253194544, 0.9669764710448521, 0.9637760657954398, 0.9604305194155658, 0.9569403357322088, 0.9533060403541939, 0.9495281805930367, 0.9456073253805213, 0.9415440651830208, 0.937339011912575, 0.932992798834739, 0.9285060804732156, 0.9238795325112867, 0.9191138516900578, 0.9142097557035307, 0.9091679830905224, 0.9039892931234433, 0.8986744656939538, 0.8932243011955153, 0.8876396204028539, 0.881921264348355, 0.8760700941954066, 0.8700869911087115, 0.8639728561215867, 0.8577286100002721, 0.8513551931052652, 0.8448535652497071, 0.8382247055548381, 0.8314696123025452, 0.8245893027850253, 0.8175848131515837, 0.8104571982525948, 0.8032075314806449, 0.7958369046088836, 0.7883464276266062, 0.7807372285720945, 0.773010453362737, 0.765167265622459, 0.7572088465064846, 0.7491363945234594, 0.7409511253549592, 0.7326542716724128, 0.724247082951467, 0.7157308252838187, 0.7071067811865476, 0.6983762494089729, 0.6895405447370669, 0.680600997795453, 0.6715589548470183, 0.6624157775901718, 0.6531728429537769, 0.6438315428897915, 0.6343932841636455, 0.6248594881423865, 0.6152315905806268, 0.6055110414043255, 0.5956993044924335, 0.5857978574564389, 0.5758081914178453, 0.5657318107836132, 0.5555702330196023, 0.5453249884220465, 0.5349976198870973, 0.5245896826784688, 0.5141027441932217, 0.5035383837257176, 0.4928981922297841, 0.48218377207912283, 0.4713967368259978, 0.46053871095824, 0.4496113296546066, 0.4386162385385277, 0.4275550934302822, 0.4164295600976373, 0.40524131400498986, 0.3939920400610481, 0.38268343236508984, 0.3713171939518376, 0.3598950365349883, 0.3484186802494345, 0.33688985339222005, 0.325310292162263, 0.3136817403988916, 0.3020059493192282, 0.29028467725446233, 0.27851968938505306, 0.2667127574748984, 0.2548656596045146, 0.24298017990326398, 0.23105810828067128, 0.21910124015686977, 0.20711137619221856, 0.19509032201612833, 0.18303988795514106, 0.17096188876030136, 0.1588581433338614, 0.14673047445536175, 0.13458070850712622, 0.12241067519921628, 0.11022220729388318, 0.09801714032956077, 0.08579731234443988, 0.07356456359966745, 0.06132073630220865, 0.049067674327418126, 0.03680722294135899, 0.024541228522912264, 0.012271538285719944, 6.123233995736766e-17, -0.012271538285719823, -0.024541228522912142, -0.036807222941358866, -0.04906767432741801, -0.06132073630220853, -0.07356456359966733, -0.08579731234443976, -0.09801714032956065, -0.11022220729388306, -0.12241067519921615, -0.1345807085071261, -0.14673047445536164, -0.15885814333386128, -0.17096188876030124, -0.18303988795514092, -0.1950903220161282, -0.20711137619221845, -0.21910124015686966, -0.23105810828067114, -0.24298017990326387, -0.2548656596045145, -0.2667127574748983, -0.27851968938505295, -0.29028467725446216, -0.3020059493192281, -0.3136817403988914, -0.32531029216226287, -0.33688985339221994, -0.3484186802494344, -0.35989503653498817, -0.3713171939518375, -0.3826834323650897, -0.393992040061048, -0.40524131400498975, -0.416429560097637, -0.42755509343028186, -0.4386162385385274, -0.4496113296546067, -0.46053871095824006, -0.4713967368259977, -0.4821837720791227, -0.492898192229784, -0.5035383837257175, -0.5141027441932216, -0.5245896826784687, -0.534997619887097, -0.5453249884220462, -0.555570233019602, -0.5657318107836132, -0.5758081914178453, -0.5857978574564389, -0.5956993044924334, -0.6055110414043254, -0.6152315905806267, -0.6248594881423862, -0.6343932841636454, -0.6438315428897913, -0.6531728429537765, -0.6624157775901719, -0.6715589548470184, -0.680600997795453, -0.6895405447370669, -0.6983762494089728, -0.7071067811865475, -0.7157308252838186, -0.7242470829514668, -0.7326542716724127, -0.7409511253549589, -0.7491363945234591, -0.7572088465064847, -0.765167265622459, -0.773010453362737, -0.7807372285720945, -0.7883464276266062, -0.7958369046088835, -0.8032075314806448, -0.8104571982525947, -0.8175848131515836, -0.8245893027850251, -0.8314696123025453, -0.8382247055548381, -0.8448535652497071, -0.8513551931052652, -0.857728610000272, -0.8639728561215867, -0.8700869911087113, -0.8760700941954065, -0.8819212643483549, -0.8876396204028538, -0.8932243011955152, -0.8986744656939539, -0.9039892931234433, -0.9091679830905224, -0.9142097557035307, -0.9191138516900578, -0.9238795325112867, -0.9285060804732155, -0.9329927988347388, -0.9373390119125748, -0.9415440651830207, -0.9456073253805212, -0.9495281805930367, -0.9533060403541939, -0.9569403357322088, -0.9604305194155658, -0.9637760657954398, -0.9669764710448521, -0.970031253194544, -0.9729399522055601, -0.9757021300385285, -0.9783173707196275, -0.9807852804032304, -0.9831054874312163, -0.9852776423889412, -0.9873014181578584, -0.989176509964781, -0.99090263542778, -0.99247953459871, -0.9939069700023561, -0.9951847266721968, -0.996312612182778, -0.9972904566786902, -0.9981181129001492, -0.9987954562051724, -0.9993223845883495, -0.9996988186962042, -0.9999247018391445};
const DTYPE W_imag[]={-0.0, -0.012271538285719925, -0.024541228522912288, -0.03680722294135883, -0.049067674327418015, -0.06132073630220858, -0.07356456359966743, -0.0857973123444399, -0.0980171403295606, -0.11022220729388306, -0.1224106751992162, -0.13458070850712617, -0.14673047445536175, -0.15885814333386145, -0.17096188876030122, -0.18303988795514095, -0.19509032201612825, -0.20711137619221856, -0.2191012401568698, -0.2310581082806711, -0.24298017990326387, -0.25486565960451457, -0.26671275747489837, -0.27851968938505306, -0.29028467725446233, -0.3020059493192281, -0.3136817403988915, -0.3253102921622629, -0.33688985339222005, -0.34841868024943456, -0.3598950365349881, -0.37131719395183754, -0.3826834323650898, -0.3939920400610481, -0.40524131400498986, -0.41642956009763715, -0.4275550934302821, -0.43861623853852766, -0.44961132965460654, -0.46053871095824, -0.47139673682599764, -0.4821837720791227, -0.49289819222978404, -0.5035383837257176, -0.5141027441932217, -0.524589682678469, -0.5349976198870972, -0.5453249884220465, -0.5555702330196022, -0.5657318107836131, -0.5758081914178453, -0.5857978574564389, -0.5956993044924334, -0.6055110414043255, -0.6152315905806268, -0.6248594881423863, -0.6343932841636455, -0.6438315428897914, -0.6531728429537768, -0.6624157775901718, -0.6715589548470183, -0.680600997795453, -0.6895405447370668, -0.6983762494089728, -0.7071067811865476, -0.7157308252838186, -0.7242470829514669, -0.7326542716724128, -0.7409511253549591, -0.7491363945234593, -0.7572088465064846, -0.765167265622459, -0.7730104533627369, -0.7807372285720945, -0.7883464276266062, -0.7958369046088835, -0.8032075314806448, -0.8104571982525948, -0.8175848131515837, -0.8245893027850253, -0.8314696123025452, -0.838224705554838, -0.844853565249707, -0.8513551931052652, -0.8577286100002721, -0.8639728561215867, -0.8700869911087113, -0.8760700941954066, -0.8819212643483549, -0.8876396204028539, -0.8932243011955153, -0.8986744656939538, -0.9039892931234433, -0.9091679830905223, -0.9142097557035307, -0.9191138516900578, -0.9238795325112867, -0.9285060804732156, -0.9329927988347388, -0.937339011912575, -0.9415440651830208, -0.9456073253805213, -0.9495281805930367, -0.9533060403541938, -0.9569403357322089, -0.9604305194155658, -0.9637760657954398, -0.9669764710448521, -0.970031253194544, -0.9729399522055601, -0.9757021300385286, -0.9783173707196277, -0.9807852804032304, -0.9831054874312163, -0.9852776423889412, -0.9873014181578584, -0.989176509964781, -0.99090263542778, -0.99247953459871, -0.9939069700023561, -0.9951847266721968, -0.996312612182778, -0.9972904566786902, -0.9981181129001492, -0.9987954562051724, -0.9993223845883495, -0.9996988186962042, -0.9999247018391445, -1.0, -0.9999247018391445, -0.9996988186962042, -0.9993223845883495, -0.9987954562051724, -0.9981181129001492, -0.9972904566786902, -0.996312612182778, -0.9951847266721969, -0.9939069700023561, -0.99247953459871, -0.99090263542778, -0.989176509964781, -0.9873014181578584, -0.9852776423889412, -0.9831054874312163, -0.9807852804032304, -0.9783173707196277, -0.9757021300385286, -0.9729399522055602, -0.970031253194544, -0.9669764710448521, -0.9637760657954398, -0.9604305194155659, -0.9569403357322089, -0.9533060403541939, -0.9495281805930367, -0.9456073253805214, -0.9415440651830208, -0.937339011912575, -0.9329927988347388, -0.9285060804732156, -0.9238795325112867, -0.9191138516900578, -0.9142097557035307, -0.9091679830905225, -0.9039892931234434, -0.8986744656939539, -0.8932243011955152, -0.8876396204028539, -0.881921264348355, -0.8760700941954066, -0.8700869911087115, -0.8639728561215868, -0.8577286100002721, -0.8513551931052652, -0.8448535652497072, -0.8382247055548382, -0.8314696123025453, -0.8245893027850252, -0.8175848131515837, -0.8104571982525948, -0.8032075314806449, -0.7958369046088836, -0.7883464276266063, -0.7807372285720946, -0.7730104533627371, -0.7651672656224591, -0.7572088465064847, -0.7491363945234593, -0.740951125354959, -0.7326542716724128, -0.7242470829514669, -0.7157308252838187, -0.7071067811865476, -0.6983762494089729, -0.689540544737067, -0.6806009977954532, -0.6715589548470186, -0.662415777590172, -0.6531728429537766, -0.6438315428897914, -0.6343932841636455, -0.6248594881423863, -0.6152315905806269, -0.6055110414043257, -0.5956993044924335, -0.585797857456439, -0.5758081914178454, -0.5657318107836135, -0.5555702330196022, -0.5453249884220464, -0.5349976198870972, -0.524589682678469, -0.5141027441932218, -0.5035383837257176, -0.49289819222978415, -0.4821837720791229, -0.4713967368259978, -0.4605387109582402, -0.4496113296546069, -0.43861623853852755, -0.42755509343028203, -0.41642956009763715, -0.4052413140049899, -0.39399204006104815, -0.3826834323650899, -0.3713171939518377, -0.35989503653498833, -0.3484186802494348, -0.33688985339222033, -0.32531029216226326, -0.3136817403988914, -0.30200594931922803, -0.2902846772544624, -0.27851968938505317, -0.2667127574748985, -0.2548656596045147, -0.24298017990326407, -0.23105810828067133, -0.21910124015687005, -0.20711137619221884, -0.1950903220161286, -0.1830398879551409, -0.17096188876030122, -0.15885814333386147, -0.1467304744553618, -0.13458070850712628, -0.12241067519921635, -0.11022220729388324, -0.09801714032956083, -0.08579731234444016, -0.07356456359966773, -0.06132073630220849, -0.049067674327417966, -0.03680722294135883, -0.024541228522912326, -0.012271538285720007};
float DataIN_R[FFT_Size];
float DataIN_I[FFT_Size];
float complex FFTIn_C[FFT_Size];
float complex FFT_input[FFT_Size];
float FFTOut_R[FFT_Size];
float FFTOut_I[FFT_Size];
int reverse(int n)
{
    int mask=1;
    int ans=0;
    for(int i=0; i<FFTSTAGES; i++)
    {
        if(n&mask)
        {
            ans=ans+pow(2,FFTSTAGES-1-i);
        }
        mask=mask<<1;
    }
    return ans;
}
void FFT_input1()
{
 float temp_r, temp_i;
 XTime seed_value;
 XTime_GetTime(&seed_value);
 srand(seed_value);
 for (int i = 0 ; i < FFT_Size ; i++)
 {
    temp_r = rand()%2000;
    temp_i = rand()%2000;
    FFT_input[i] = temp_r + I*temp_i;
    FFTIn_C[i] = temp_r + I*temp_i;
    DataIN_R[i] = creal(FFTIn_C[i]);
    DataIN_I[i] = cimag(FFTIn_C[i]);
 }
}
void FFT_sw(float FFTIn_I[FFT_Size], float FFTIn_R[FFT_Size], float FFTOut_I[FFT_Size], float FFTOut_R[FFT_Size])
{
 DTYPE temp_R;
 DTYPE temp_I;
 int i,j;
 int i_lower;
 int stage;
 int subFFTSize;
 int BFWidth;
 for (i = 0; i < FFT_Size; ++i)
 {
    FFTOut_R[reverse(i)] = FFTIn_R[i];
    FFTOut_I[reverse(i)] = FFTIn_I[i];
 }
 DTYPE BFWeight_R, BFWeight_I;
 stages:for(stage=1; stage<= FFTSTAGES; stage++)
 {
 subFFTSize = 1 << stage;
 BFWidth = subFFTSize >> 1;
 butterfly:for(j=0; j<BFWidth; j++)
 {
    BFWeight_R = W_real[j * (FFT_Size>>stage)];
    BFWeight_I = W_imag[j * (FFT_Size>>stage)];
    subDFTSize:for(i =j ; i < FFT_Size; i += subFFTSize)
    {
        i_lower = i + BFWidth;
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
int FFTPS()
{
 XTime time_PS_start , time_PS_end;
 XTime_SetTime(0);
 XTime_GetTime(&time_PS_start);
 FFT_sw(DataIN_I, DataIN_R, FFTOut_I,FFTOut_R);
 XTime_GetTime(&time_PS_end);
 printf("\n FFT output: \r\n");
for (int j = 0 ; j < FFT_Size ; j++)
{
    printf("PS Output : %f + I%f \n " , FFTOut_R[j], FFTOut_I[j]);
    usleep(0.1);
}
 printf("\n-----------EXECUTION TIME--------------------\n");
 float time_processor = 0;
 time_processor = (float)1.0 * (time_PS_end - time_PS_start) / (COUNTS_PER_SECOND/1000000);
 printf("Execution Time for PS in Micro-Seconds : %f\n" , time_processor);
 printf("=======================FFT Input===================\n");
 for (int i = 0 ; i < FFT_Size ; i++)
 {
 printf("%f %f\n", crealf(FFT_input[i]), cimagf(FFT_input[i]));
 }
 return 0;
}
int FFTHPVSACP()
{
 float complex FFT_ACPoutput[FFT_Size];
 XAxiDma_Config *DMAACP_confptr;
 DMAACP_confptr = XAxiDma_LookupConfig(XPAR_AXI_DMA_ACP_DEVICE_ID);
 XAxiDma DMAACP_instance;
 int status = XAxiDma_CfgInitialize(&DMAACP_instance, DMAACP_confptr);
 if(status!=XST_SUCCESS)
 {
 printf("ACP DMA Init Failed\n");
 return 1;
 }
 XTime time_ACP_start , time_ACP_end;
 XTime_SetTime(0);
 XTime_GetTime(&time_ACP_start);
 Xil_DCacheFlushRange((UINTPTR)FFT_input,(sizeof(float)*2*FFT_Size));
 status = XAxiDma_SimpleTransfer(&DMAACP_instance, (UINTPTR)FFT_ACPoutput, FFT_Size*2*sizeof(float), XAXIDMA_DEVICE_TO_DMA);
 if(status!=XST_SUCCESS)
 {
 printf("ACP DMA Device to DMA Configuration Failed\n");
 return 1;
 }
 status = XAxiDma_SimpleTransfer(&DMAACP_instance, (UINTPTR)FFT_input, FFT_Size*2*sizeof(float), XAXIDMA_DMA_TO_DEVICE);
 if(status!=XST_SUCCESS)
 {
 printf("ACP DMA DMA to Device Configuration Failed\n");
 return 1;
 }
 Xil_DCacheInvalidateRange((UINTPTR)FFT_input,(sizeof(float)*2*FFT_Size));
 status = XAxiDma_ReadReg(XPAR_AXI_DMA_ACP_BASEADDR,0x04);
 status = status & 0x00000002;
 while (status!= 0x00000002)
 {
 status = XAxiDma_ReadReg(XPAR_AXI_DMA_ACP_BASEADDR,0x04);
 status = status & 0x00000002;
 }
 status = XAxiDma_ReadReg(XPAR_AXI_DMA_ACP_BASEADDR,0x34);
 status = status & 0x00000002;
 while (status!= 0x00000002)
 {
 status = XAxiDma_ReadReg(XPAR_AXI_DMA_ACP_BASEADDR,0x34);
 status = status & 0x00000002;
 }
 XTime_GetTime(&time_ACP_end);
 printf("\n-----------ACP FPGA EXECUTION TIME--------------------\n");
 float time_ACPFPGA = 0;
 time_ACPFPGA = (float)1.0 * (time_ACP_end - time_ACP_start) / (COUNTS_PER_SECOND/1000000);
 printf("Execution Time for ACP FPGA in Micro-Seconds : %f\n" ,time_ACPFPGA);
 printf("\nACP FPGA output: \r\n");
for (int i = 0 ; i < FFT_Size ; i++)
{
    printf("%f %f\n", crealf(FFT_ACPoutput[i]), cimagf(FFT_ACPoutput[i]));
}
 XTime time_HP_start, time_HP_end;
 float complex FFT_HPoutput[FFT_Size];
 XAxiDma_Config *DMAHP_confptr;
 DMAHP_confptr = XAxiDma_LookupConfig(XPAR_AXI_DMA_HP_DEVICE_ID);
 XAxiDma DMAHP_instance;
 int status1 = XAxiDma_CfgInitialize(&DMAHP_instance, DMAHP_confptr);
 if(status1!=XST_SUCCESS)
 {
 printf("HP DMA Init Failed\n");
 return 1;
 }
 XTime_SetTime(0);
 XTime_GetTime(&time_HP_start);
 Xil_DCacheFlushRange((UINTPTR)FFT_input,(sizeof(float)*2*FFT_Size));
 status1 = XAxiDma_SimpleTransfer(&DMAHP_instance, (UINTPTR)FFT_HPoutput, FFT_Size*2*sizeof(float), XAXIDMA_DEVICE_TO_DMA);
 if(status1!=XST_SUCCESS)
 {
 printf("HP DMA Device to DMA Configuration Failed\n");
 return 1;
 }
 status1 = XAxiDma_SimpleTransfer(&DMAHP_instance, (UINTPTR)FFT_input, FFT_Size*2*sizeof(float), XAXIDMA_DMA_TO_DEVICE);
 if(status1!=XST_SUCCESS)
 {
 printf("HP DMA DMA to Device Configuration Failed\n");
 return 1;
 }
 Xil_DCacheInvalidateRange((UINTPTR)FFT_input,(sizeof(float)*2*FFT_Size));
 status1 = XAxiDma_ReadReg(XPAR_AXI_DMA_HP_BASEADDR,0x04);
 status1 = status1 & 0x00000002;
 while (status1!= 0x00000002)
 {
 status1 = XAxiDma_ReadReg(XPAR_AXI_DMA_HP_BASEADDR,0x04);
 status1 = status1 & 0x00000002;
 }
 status1 = XAxiDma_ReadReg(XPAR_AXI_DMA_HP_BASEADDR,0x34);
 status1 = status1 & 0x00000002;
 while (status1!= 0x00000002)
 {
 status1 = XAxiDma_ReadReg(XPAR_AXI_DMA_HP_BASEADDR,0x34);
 status1 = status1 & 0x00000002;
 }
 XTime_GetTime(&time_HP_end);
 printf("\n-----------HP FPGA EXECUTION TIME--------------------\n");
 float time_HPFPGA = 0;
 time_HPFPGA = (float)1.0 * (time_HP_end -time_HP_start) / (COUNTS_PER_SECOND/1000000);
 printf("Execution Time for HP FPGA in Micro-Seconds : %f\n" , time_HPFPGA);
 printf("\nHP FPGA output: \r\n");
 for (int i = 0 ; i < FFT_Size ; i++)
 {
 printf("%f %f\n", crealf(FFT_HPoutput[i]), cimagf(FFT_HPoutput[i]));
 }
}
int main()
{
 init_platform();
 FFT_input1();
 FFTPS();
 FFTHPVSACP();
 cleanup_platform();
}