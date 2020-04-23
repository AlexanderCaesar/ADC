/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/23
* @see
*/

#include "piclist.h"

#pragma once

class Frame;

class DPB//Decoding Picture Buffer
{
public:


    PicList            m_picList;
    PicList            m_freeList;

    DPB(adc_param *param)
    {
    }

    ~DPB();

};

