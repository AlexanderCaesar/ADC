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

    m_stride[0] = 0;
    m_stride[1] = 0;
    m_stride[2] = 0;

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

    m_stride[0] = m_picWidth + m_marginX;

    m_picBuf[0] = (uint8_t*)malloc(m_stride[0] * (m_picHeight + m_marginY)*sizeof(pixel));

    if (!m_picBuf[0])
    {
        ERR("malloc failed");
        return -1;
    }
    m_picOrg[0] = m_picBuf[0] + m_marginY *  m_stride[0] + m_marginX;

    if (picCsp != ADC_CSP_I400)
    {
        m_stride[1] = m_stride[2] = (m_picWidth >> m_hChromaShift) + m_marginX;

        m_picBuf[1] = (uint8_t*)malloc(m_stride[1] * (((m_picHeight >> m_hChromaShift) + m_marginY))*sizeof(pixel));

        if (!m_picBuf[1])
        {
            ERR("malloc failed");
            return -1;
        }

        m_picBuf[2] = (uint8_t*)malloc(m_stride[2] * (((m_picHeight >> m_hChromaShift) + m_marginY))*sizeof(pixel));

        if (!m_picBuf[2])
        {
            ERR("malloc failed");
            return -1;
        }

        m_picOrg[1] = m_picBuf[1] + m_marginY * m_stride[1] + m_marginX;
        m_picOrg[2] = m_picBuf[2] + m_marginY * m_stride[2] + m_marginX;
    }
    else
    {
        m_picBuf[1] = m_picBuf[2] = NULL;
        m_picOrg[1] = m_picOrg[2] = NULL;
    }
    return 0;
}

/* Copy pixels from an adc_picture into internal PicYuv instance.*/
void PicYuv::copyFromPicture(const adc_picture& pic, const adc_param& param)
{
    uint32_t width = m_picWidth;
    uint32_t height = m_picHeight;

    m_picCsp = pic.colorSpace;

    pixel *yPixel = m_picOrg[0];
    uint8_t *yChar = (uint8_t*)pic.planes[0];

    for (uint32_t r = 0; r < height; r++)
    {
        memcpy(yPixel, yChar, width * sizeof(pixel));

        yPixel += m_stride[0];
        yChar += pic.stride[0] / sizeof(*yChar);
    }

    if (param.chromaFormat != ADC_CSP_I400)
    {
        pixel *uPixel = m_picOrg[1];
        pixel *vPixel = m_picOrg[2];

        uint8_t *uChar = (uint8_t*)pic.planes[1];
        uint8_t *vChar = (uint8_t*)pic.planes[2];

        for (uint32_t r = 0; r < height >> m_vChromaShift; r++)
        {
            memcpy(uPixel, uChar, (width >> m_hChromaShift) * sizeof(pixel));
            memcpy(vPixel, vChar, (width >> m_hChromaShift) * sizeof(pixel));

            uPixel += m_stride[1];
            vPixel += m_stride[2];
            uChar += pic.stride[1] / sizeof(*uChar);
            vChar += pic.stride[2] / sizeof(*vChar);
        }
    }

    pixel *Y = m_picBuf[0];
    pixel *U = m_picBuf[1];
    pixel *V = m_picBuf[2];

    /* extend the top edge */
    for (uint32_t x = 0; x < (m_picWidth + m_marginX); x++)
    {
        Y[x] = 128;
    }

    /* extend the left edge */
    for (uint32_t y = 0; y < (m_picHeight + m_marginY); y++)
    {
        Y[0] = 128;
        Y += m_stride[0];
    }

    /* extend the top edge */
    for (uint32_t x = 0; x < ((m_picWidth >> m_hChromaShift) + m_marginX); x++)
    {
        U[x] = 128;
    }

    /* extend the left edge */
    for (uint32_t y = 0; y < ((m_picHeight >> m_vChromaShift) + m_marginY); y++)
    {
        U[0] = 128;
        U += m_stride[1];
    }


    /* extend the top edge */
    for (uint32_t x = 0; x < ((m_picWidth >> m_hChromaShift) + m_marginX); x++)
    {
        V[x] = 128;
    }

    /* extend the left edge */
    for (uint32_t y = 0; y < ((m_picHeight >> m_vChromaShift) + m_marginY); y++)
    {
        V[0] = 128;
        V += m_stride[2];
    }
}

/* padPicture.*/
void PicYuv::padPicture(const adc_param& param)
{
    uint32_t width = m_picWidth;
    uint32_t height = m_picHeight;

    pixel *Y = m_picBuf[0];
    pixel *U = m_picBuf[1];
    pixel *V = m_picBuf[2];

    /* extend the top edge */
    for (uint32_t x = 0; x < (m_picWidth + m_marginX); x++)
    {
        Y[x] = 128;
    }

    /* extend the left edge */
    for (uint32_t y = 0; y < (m_picHeight + m_marginY); y++)
    {
        Y[0] = 128;
        Y += m_stride[0];
    }

    /* extend the top edge */
    for (uint32_t x = 0; x < ((m_picWidth >> m_hChromaShift) + m_marginX); x++)
    {
        U[x] = 128;
    }

    /* extend the left edge */
    for (uint32_t y = 0; y < ((m_picHeight >> m_vChromaShift) + m_marginY); y++)
    {
        U[0] = 128;
        U += m_stride[1];
    }


    /* extend the top edge */
    for (uint32_t x = 0; x < ((m_picWidth >> m_hChromaShift) + m_marginX); x++)
    {
        V[x] = 128;
    }

    /* extend the left edge */
    for (uint32_t y = 0; y < ((m_picHeight >> m_vChromaShift) + m_marginY); y++)
    {
        V[0] = 128;
        V += m_stride[2];
    }
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
