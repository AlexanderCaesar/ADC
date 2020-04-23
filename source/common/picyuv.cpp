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
#include "picyuv.h"
#include "stdlib.h"
#include "log.h"


PicYuv::PicYuv()
{
    m_picBuf[0] = NULL;
    m_picBuf[1] = NULL;
    m_picBuf[2] = NULL;

    m_picOrg[0] = NULL;
    m_picOrg[1] = NULL;
    m_picOrg[2] = NULL;

    m_stride = 0;
    m_strideC = 0;
    m_hChromaShift = 0;
    m_vChromaShift = 0;
}


int PicYuv::create(adc_param* param)
{

    m_param = param;
    uint32_t picWidth = m_param->sourceWidth;
    uint32_t picHeight = m_param->sourceHeight;
    uint32_t picCsp = m_param->chromaFormat;
    m_picWidth = picWidth;
    m_picHeight = picHeight;
    m_hChromaShift = CHROMA_H_SHIFT(picCsp);
    m_vChromaShift = CHROMA_V_SHIFT(picCsp);
    m_picCsp = picCsp;

    m_marginX = 1; 
    m_marginY = 1;

    m_stride = m_picWidth + m_marginX;

    m_picBuf[0] = (uint8_t*)malloc(m_stride * (m_picHeight + m_marginY )*sizeof(pixel));

    if (!m_picBuf[0])
    {
        ERR("malloc failed");
        return -1;
    }
    m_picOrg[0] = m_picBuf[0] + m_marginY * m_stride + m_marginX;

    if (picCsp != ADC_CSP_I400)
    {
        m_strideC = (m_picWidth >> m_hChromaShift) + m_marginX;

        m_picBuf[1] = (uint8_t*)malloc(m_strideC * (((m_picHeight >> m_hChromaShift) + m_marginY))*sizeof(pixel));

        if (!m_picBuf[1])
        {
            ERR("malloc failed");
            return -1;
        }

        m_picBuf[2] = (uint8_t*)malloc(m_strideC * (((m_picHeight >> m_hChromaShift) + m_marginY))*sizeof(pixel));

        if (!m_picBuf[2])
        {
            ERR("malloc failed");
            return -1;
        }

        m_picOrg[1] = m_picBuf[1] + m_marginY * m_strideC + m_marginX;
        m_picOrg[2] = m_picBuf[2] + m_marginY * m_strideC + m_marginX;
    }
    else
    {
        m_picBuf[1] = m_picBuf[2] = NULL;
        m_picOrg[1] = m_picOrg[2] = NULL;
    }
    return 0;
}


void PicYuv::destroy()
{
    for (int i = 0; i < 3; i++)
    {
        if (m_picBuf[i])
        {
            free(m_picBuf[i]);
            m_picBuf[i] = NULL;
        }
    }
}
