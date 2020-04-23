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


#pragma once 

class PicYuv;

class Frame
{
public:
        
    PicYuv*                m_reconPic;          

    /* Data associated with adc_picture */
    PicYuv*                m_fencPic;            
    int                    m_poc;                
    int                    m_encodeOrder;
    int64_t                m_pts;                //user provided presentation time stamp
    int64_t                m_reorderedPts;       //
    int64_t                m_dts;                //


    Frame*                 m_next;               // PicList doubly linked list pointers
    Frame*                 m_prev;
    adc_param*             m_param;              // Points to the latest param set for the frame.

    int64_t                m_encodeStartTime;

    Frame();

    bool create(adc_param *param);

    void destroy();
};
