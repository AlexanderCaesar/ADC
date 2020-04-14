/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/14
* @see
*/

#pragma once

#include "adc.h"
#include "stdlib.h"
#include "bitstream.h"

class NALList
{
public:

    adc_nal    m_nal[MAX_NAL_UNITS];
    uint32_t   m_numNal;

    uint8_t*    m_buffer;
    uint32_t    m_occupancy;
    uint32_t    m_allocSize;

    uint8_t*    m_extraBuffer;
    uint32_t    m_extraOccupancy;
    uint32_t    m_extraAllocSize;
    bool        m_annexB;

    NALList();
    ~NALList() { 
        if (m_buffer)
        {
            free(m_buffer);
            m_buffer = NULL;
        }
        if (m_extraBuffer)
        {
            free(m_extraBuffer);
            m_extraBuffer = NULL;
        }
    }
    void serialize(NalUnitType nalUnitType, const Bitstream& bs);
};
