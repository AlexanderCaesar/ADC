/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/14
* @see
*/


#include "log.h"
#include "adc.h"
#include "adc_config.h"

#pragma once

class Files 
{
public:
    FILE*    input;
    FILE*    output;
    FILE*    recon;
    uint32_t width;
    uint32_t height;
    int      framesize;
    uint8_t* buf;
    int      chromaFormat;

    Files();

    int   openfile(ADCConfig adconfig);
    int   opendecoderfile(ADCConfig adconfig);
    void  closefile();

    int   readPicture(adc_picture& pic);
    int   writeRecon(adc_picture* pic);

    int   writeNAL(FILE *fp, adc_nal* nal, uint32_t nalcount);
    int   writeHeaders(adc_nal* nal, uint32_t nalcount);
    int   writeFrames(adc_nal* nal, uint32_t nalcount);

};