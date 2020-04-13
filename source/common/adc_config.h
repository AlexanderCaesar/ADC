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

#include <string>
#include <cstdint>
#include "adc.h"

class ADCConfig
{
public:
    ADCConfig();
    int parse_arg(int argc, char* argv[]);
    void display_help();

    std::string input_file;
    std::string output_file;
    std::string rec_file;

    adc_param   adc_p;

};

extern ADCConfig adcconfig;
