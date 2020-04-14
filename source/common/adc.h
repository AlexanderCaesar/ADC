/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/13
* @see
*/

#ifndef ADC_H
#define ADC_H
#include <stdint.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Supported internal color space types (according to semantics of chroma_format_idc) */
#define X265_CSP_I400           0  /* yuv 4:0:0 planar */
#define X265_CSP_I420           1  /* yuv 4:2:0 planar */
#define X265_CSP_I422           2  /* yuv 4:2:2 planar */
#define X265_CSP_I444           3  /* yuv 4:4:4 planar */
#define X265_CSP_COUNT          4  /* Number of supported internal color spaces */

/* Output statistics from encoder */
typedef struct adc_stats
{
    double                globalPsnrY;
    double                globalPsnrU;
    double                globalPsnrV;
    double                globalPsnr;
    double                globalSsim;
    double                elapsedEncodeTime;    /* wall time since encoder was opened */
    double                elapsedVideoTime;     /* encoded picture count / frame rate */
    double                bitrate;              /* accBits / elapsed video time */
    uint64_t              accBits;              /* total bits output thus far */
    uint32_t              encodedPictureCount;  /* number of output pictures thus far */
} adc_stats;

/* adc input parameters */

typedef struct adc_param
{
    /* Enable the measurement and reporting of PSNR. Default is enabled */
    int       bEnablePsnr;

    /* Enable the measurement and reporting of SSIM. Default is disabled */
    int       bEnableSsim;

    /* The level of logging detail emitted by the encoder.*/
    int       logLevel;


    /* Numerator and denominator of frame rate */
    uint32_t  fpsNum; 

    /* Width (in pixels) of the source pictures.  */
    int       sourceWidth; 

    /* Height (in pixels) of the source pictures. */
    int       sourceHeight;

    /* Total Number of frames to be encoded, calculated from the user input
    * In case, the input is read from a pipe, this can
    * remain as 0. */
    int       totalFrames;

    /*Error tolerance 0~255 */
    int       et;

    /* Enable the output the reconstruction frames. Default is disabled */
    int       bRec;

    int       chromaFormat;    

} adc_param;

/*adc encoder*/
typedef struct adc_encoder adc_encoder;

adc_encoder *adc_encoder_open(adc_param *p);
void adc_encoder_close(adc_encoder* enc);

/*adc decoder*/


#define ADC_BUILD 004

#ifdef __cplusplus
}
#endif

#endif // ADC_H