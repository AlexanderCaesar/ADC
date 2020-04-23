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
#include "frame.h"
#include "picyuv.h"


Frame::Frame()
{
    m_reconPic = NULL;
    m_next = NULL;
    m_prev = NULL;
    m_param = NULL;
    m_encodeStartTime = 0;
}

int Frame::create(adc_param *param)
{
    m_fencPic = new PicYuv;
    m_param = param;

    if (m_fencPic->create(param))
    {
        return -1;
    }

    return 0;
}

void Frame::destroy()
{
    if (m_fencPic)
    {
        m_fencPic->destroy();
        delete m_fencPic;
        m_fencPic = NULL;
    }
}
