/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/15
* @see
*/


#include "log.h"
#include "adc.h"
#include "nal.h"
#include "bitstream.h"

#pragma once

class Detropy 
{
public:
    adc_nal m_nal;
    int     m_occupancy;

    Detropy();
    ~Detropy()
    {
        if (m_nal.payload)
        {
            free(m_nal.payload);
            m_nal.payload = NULL;
        }
    }
    void convertPayloadToRBSP(uint8_t* src, int src_len);
    int  decodeVPS(adc_param *p, adc_nal *nal);

};