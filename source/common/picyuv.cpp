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


bool PicYuv::create(adc_param* param, bool picAlloc, pixel *pixelbuf)
{
    return true;
}


void PicYuv::destroy()
{
}
