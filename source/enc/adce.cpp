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
    adcconfig.parse_arg(argc, argv);

    LogSetLevel(LogLevel(adcconfig.adc_p.logLevel));
    LogSetCallback(NULL, "inf.log", "stat.log");

    if (open_encoder(&adcconfig.adc_p))
    {
        ERR("open encoder failed.");
        return 0;
    }
    USE("codestram");
    return 0;
}