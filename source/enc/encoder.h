/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/14
* @see
*/


#include"log.h"
#include"adc.h"
#include"nal.h"
#include"entropy.h"
#include "dpb.h"

#pragma once

struct adc_encoder {};

class Encoder : public adc_encoder
{
public:

    int                m_poc; 
    adc_param          m_param;
    NALList            m_nalList;
    adc_stats          m_stats;

    DPB*               m_dpb;

    Encoder();
    ~Encoder()
    {
    };

    void create();
    void destroy();
    void printSummary();

    void getStreamHeaders(NALList& list, Entropy& entropy, Bitstream& bs);
    int  encode(const adc_picture* pic_in, adc_picture* pic_out);
    int  quadtree(Frame* curFrame, uint32_t X, uint32_t Y, uint32_t width, uint32_t height, uint32_t, YUVType yuv);
    int  compressFrame();

};
