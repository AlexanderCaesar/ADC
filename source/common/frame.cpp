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

bool Frame::create(adc_param *param)
{
    return true;
}

void Frame::destroy()
{

}
