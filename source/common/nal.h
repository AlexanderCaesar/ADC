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

class NALList
{
public:

    adc_nal    m_nal[MAX_NAL_UNITS];
    uint32_t   m_numNal;

    uint8_t*   m_buffer;

    NALList();
    ~NALList() { 
        if (m_buffer)
        {
            free(m_buffer);
            m_buffer = NULL;

        }
    }
    void NALList::serialize();
};
