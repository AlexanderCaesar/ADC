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

void BuildLogFilePath(const char *path);
const char* GetOsVersion();
int64_t time_mdate();