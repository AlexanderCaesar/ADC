/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/13
* @see
*/

#pragma once

#include <stdint.h>
#include "adc.h"

typedef uint8_t  pixel;

#define CHROMA_H_SHIFT(x) (x == ADC_CSP_I420 || x == ADC_CSP_I422)
#define CHROMA_V_SHIFT(x) (x == ADC_CSP_I420)

#define COM_MAX(a,b)                   (((a) > (b)) ? (a) : (b))
#define COM_MIN(a,b)                   (((a) < (b)) ? (a) : (b))

#define com_mset(dst,v,size)      memset((dst), (v), (size))

#define PROB_BITS           11 // LPS_PROB(10-bit) + MPS(1-bit)
#define PROB_MASK           ((1 << PROB_BITS) - 1) // mask for LPS_PROB + MPS

typedef int16_t lbac_ctx_model_t;

/* context models for arithemetic coding */
typedef struct uavs3e_com_lbac_all_ctx_t {
    lbac_ctx_model_t   part_split_flag;
    lbac_ctx_model_t   dir_flag;
    lbac_ctx_model_t   sign_flag;
    lbac_ctx_model_t   res;
} com_lbac_all_ctx_t;

typedef const int16_t tab_s16;

void BuildLogFilePath(const char *path);
const char* GetOsVersion();
int64_t time_mdate();

/*The mode is that value that is repeated most often in the data set.*/
pixel calBoderMode(pixel *src, uint32_t width, uint32_t height, uint32_t stride);
int   calCUMode(pixel *src, uint32_t width, uint32_t height, uint32_t stride, int& min, int& max);
int   calBoderMode(pixel *src, uint32_t X, uint32_t Y, uint32_t width, uint32_t height, uint32_t stride);
int   calDirection(pixel *rec, int mode, uint32_t width, uint32_t height, uint32_t stride, uint32_t& direction);