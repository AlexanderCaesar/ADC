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
    
    adc_nal  p_nal;
    uint32_t nal;

    if (adc_encoder_headers(encoder, &p_nal, &nal) < 0)
    {
        ERR("Failure generating stream headers\n");
        return 0;
    }
    else
        ;//totalbytes += cliopt.output->writeHeaders(p_nal, nal);

    adc_encoder_close(encoder);
    return 0;
}