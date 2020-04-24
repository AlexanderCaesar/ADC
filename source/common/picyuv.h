/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/23
* @see
*/

#include "common.h"
#include "adc.h"

#pragma once

struct adc_picyuv {};

class PicYuv : public adc_picyuv
{
public:

    pixel*   m_picBuf[3];  // full allocated buffers, including margins
    pixel*   m_picOrg[3];  // pointers to plane starts

    uint32_t m_picWidth;   
    uint32_t m_picHeight;  
    intptr_t m_stride[3];      

    uint32_t m_picCsp;    
    uint32_t m_hChromaShift;
    uint32_t m_vChromaShift;

    uint32_t m_marginX; 
    uint32_t m_marginY; 

    adc_param *m_param;

    PicYuv();

    int   create(adc_param* param);

    void  destroy();

    void  copyFromPicture(const adc_picture&, const adc_param& param);
    void  padPicture(const adc_param& param);

};
