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

    LogSetLevel(LOG_LEVEL_USER);
    LogSetCallback(NULL, "inc.log", "stat.log");
    USE("decoder");
    return 0;
}