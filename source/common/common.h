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

#define com_mset(dst,v,size)      memset((dst), (v), (size))

void BuildLogFilePath(const char *path);
const char* GetOsVersion();
int64_t time_mdate();

/*The mode is that value that is repeated most often in the data set.*/
pixel calBoderMode(pixel *src, uint32_t width, uint32_t height, uint32_t stride);
int   calCUMode(pixel *src, uint32_t width, uint32_t height, uint32_t stride, int& min, int& max);
int   calBoderMode(pixel *src, uint32_t X, uint32_t Y, uint32_t width, uint32_t height, uint32_t stride);
int   calDirection(pixel *rec, int mode, uint32_t width, uint32_t height, uint32_t stride, uint32_t& direction);