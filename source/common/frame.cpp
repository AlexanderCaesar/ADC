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
#include <stdlib.h>


Frame::Frame()
{
    m_reconPic = NULL;
    m_next = NULL;
    m_prev = NULL;
    m_param = NULL;
    m_encodeStartTime = 0;

    for (int i = 0; i < 3; i++)
    {
        m_partition[i] = NULL;
        m_direction[i] = NULL;
        m_residual[i]  = NULL;
        m_part_len[i]  = 0;
        m_dir_len[i]   = 0;
        m_res_len[i]   = 0;
    }
}

int Frame::create(adc_param *param)
{
    m_fencPic = new PicYuv;
    m_param = param;

    if (m_fencPic->create(param))
    {
        return -1;
    }

    m_reconPic = new PicYuv;
    if (m_reconPic->create(param))
    {
        return -1;
    }

    m_reconPic->padPicture(*param);

    for (int i = 0; i < 3; i++)
    {
        m_partition[i] = (uint32_t*)malloc(m_param->sourceWidth * m_param->sourceHeight*sizeof(uint32_t));
        m_direction[i] = (uint32_t*)malloc(m_param->sourceWidth * m_param->sourceHeight*sizeof(uint32_t));
        m_residual[i] = (int32_t*)malloc(m_param->sourceWidth * m_param->sourceHeight*sizeof(int32_t));
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

    if (m_reconPic)
    {
        m_reconPic->destroy();
        delete m_reconPic;
        m_reconPic = NULL;
    }

    for (int i = 0; i < 3; i++)
    {
        if (m_partition[i])
        {
            free(m_partition[i]);
            m_partition[i] = NULL;
        }
        if (m_direction[i])
        {
            free(m_direction[i]);
            m_direction[i] = NULL;
        }
        if (m_residual[i])
        {
            free(m_residual[i]);
            m_residual[i] = NULL;
        }
    }
}

