/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/13
* @see
*/

#include"log.h"
#include"adc.h"
#include "adc_config.h"
#include "files.h"

#include<stdio.h>
#include<stdlib.h>


int main(int argc, char *argv[])
{
    if (adcconfig.parse_arg(argc, argv))
    {
        return 0;
    }

    LogSetLevel(LogLevel(adcconfig.adc_p.logLevel));
    LogSetCallback(NULL, adcconfig.log_file.c_str(), NULL);

    adc_encoder *encoder = adc_encoder_open(&adcconfig.adc_p);

    if (!encoder)
    {
        ERR("open encoder failed.");
        return 0;
    }
    
    adc_nal* p_nal;
    uint32_t nal;

    Files files;

    if (files.openfile(adcconfig))
    {
        ERR("Failure open files\n");
        return 0;
    }

    if (adc_encoder_headers(encoder, &p_nal, &nal) < 0)
    {
        ERR("Failure generating stream headers\n");
        return 0;
    }
    else
        files.writeHeaders(p_nal, nal);

    adc_picture pic_orig;
    adc_picture *pic_in = &pic_orig;

    adc_picture_init(&adcconfig.adc_p, pic_in);

    int inFrameCount = 0;

    while (pic_in)
    {
        pic_orig.poc = inFrameCount;

        if (adcconfig.adc_p.totalFrames && inFrameCount >= adcconfig.adc_p.totalFrames)
            pic_in = NULL;
        else if (!files.readPicture(pic_orig))
            inFrameCount++;
        else
            pic_in = NULL;

    }

    adc_encoder_close(encoder);

    files.closefile();
    return 0;
}