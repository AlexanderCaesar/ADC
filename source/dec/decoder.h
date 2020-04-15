/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/15
* @see
*/


#include"log.h"
#include"adc.h"
#include"nal.h"
#include"bitstream.h"

#pragma once

struct adc_decoder {};

class Decoder : public adc_decoder
{
public:

    int                m_poc; 
    adc_param          m_param;
    adc_stats          m_stats;

    Decoder();
    ~Decoder()
    {
    };

    void create();
    void destroy();
    void printSummary();

    int  decodeVPS(adc_nal *nal);
    int  decode(adc_nal *nal);

};
