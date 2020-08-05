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
#define ADC_CSP_I400           0  /* yuv 4:0:0 planar */
#define ADC_CSP_I420           1  /* yuv 4:2:0 planar */
#define ADC_CSP_I422           2  /* yuv 4:2:2 planar */
#define ADC_CSP_I444           3  /* yuv 4:4:4 planar */
#define ADC_CSP_COUNT          4  /* Number of supported internal color spaces */

#define MAX_NAL_UNITS          16

typedef enum
{
    NAL_VPS = 0,
    NAL_FRAME,
} NalUnitType;

typedef enum
{
    Y = 0,
    U,
    V
} YUVType;

/* The data within the payload is already NAL-encapsulated; the type is merely
 * in the struct for easy access by the calling application.  All data returned
 * in an adc_nal, including the data in payload, is no longer valid after the
 * next call to adc_encoder_encode.  Thus it must be used or copied before
 * calling adc_encoder_encode again. */
typedef struct adc_nal
{
    uint32_t type;        /* NalUnitType */
    uint32_t sizeBytes;   /* size in bytes */
    uint8_t* payload;
} adc_nal;

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

    /*Percent of mode count to be split 0~100 */
    int       pm;

    /* Enable the output the reconstruction frames. Default is disabled */
    int       bRec;

    int       chromaFormat;

    int       multiSplitCtx;

    int       multiResCtx;

    int       multiDirCtx;

} adc_param;

/* Used to pass pictures into the encoder, and to get picture data back out of
* the encoder.  The input and output semantics are different */
typedef struct adc_picture
{
    /* presentation time stamp: user-specified, returned on output */
    int64_t pts;

    /* display time stamp: ignored on input, copied from reordered pts. Returned
    * on output */
    int64_t dts;

    /* Must be specified on input pictures, the number of planes is determined
    * by the colorSpace value */
    uint8_t*   planes[3];

    /* Stride is the number of bytes between row starts */
    int     stride[3];

    /* Ignored on input, set to picture count, returned on output */
    int     poc;

    /* Must be specified on input pictures: ADC_CSP_I420 or other. It must
    * match the internal color space of the encoder. adc_picture_init() will
    * initialize this value to the internal color space */
    int     colorSpace;

    /* Width (in pixels) of the source pictures.  */
    int       sourceWidth;

    /* Height (in pixels) of the source pictures. */
    int       sourceHeight;

} adc_picture;


/*adc encoder*/
typedef struct adc_encoder adc_encoder;

adc_encoder *adc_encoder_open(adc_param *p);
void        adc_encoder_close(adc_encoder* enc);
int         adc_encoder_headers(adc_encoder *enc, adc_nal **pp_nal, uint32_t *pi_nal);
void        adc_picture_init(adc_param *param, adc_picture *pic);

/* adc_encoder_encode:
*      encode one picture.
*      *pi_nal is the number of NAL units outputted in pp_nal.
*      returns negative on error, 1 if a picture and access unit were output,
*      or zero if the encoder pipeline is still filling or is empty after flushing.
*      the payloads of all output NALs are guaranteed to be sequential in memory.
*      To flush the encoder and retrieve delayed output pictures, pass pic_in as NULL.
*      Once flushing has begun, all subsequent calls must pass pic_in as NULL. */
int adc_encoder_encode(adc_encoder *encoder, adc_nal **pp_nal, uint32_t *pi_nal, adc_picture *pic_in, adc_picture *pic_out);


/*adc decoder*/
typedef struct adc_decoder adc_decoder;

adc_decoder *adc_decoder_open(adc_param *p);
void        adc_decoder_close(adc_decoder* dec);
int         adc_decoder_headers(adc_decoder* dec, adc_nal *nal);
int         adc_decoder_decode(adc_decoder* dec, adc_nal *nal, adc_picture *pic_out);


#define ADC_BUILD "1.0.4"

#ifdef __cplusplus
}
#endif

#endif // ADC_H