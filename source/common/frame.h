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
    int64_t                m_pts;                //user provided presentation time stamp
    int64_t                m_dts;                //


    Frame*                 m_next;               // PicList doubly linked list pointers
    Frame*                 m_prev;
    adc_param*             m_param;              // Points to the latest param set for the frame.

    uint32_t*              m_partition[3];  
    uint32_t*              m_direction[3];
    int32_t*               m_residual[3];

    uint32_t               m_part_len[3];
    uint32_t               m_dir_len[3];
    uint32_t               m_res_len[3];



    int64_t                m_encodeStartTime;
    int                    m_bHasReferences;    /* used during DPB updates */

    Frame();

    int  create(adc_param *param);

    void destroy();
};
