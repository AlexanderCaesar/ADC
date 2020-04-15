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

    adc_decoder *decoder = adc_decoder_open(&adcconfig.adc_p);

    if (!decoder)
    {
        ERR("open decoder failed.");
        return 0;
    }

    adc_decoder_close(decoder);
    return 0;
}