/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/15
* @see
*/


#include"log.h"
#include"adc.h"
#include"nal.h"
#include"bitstream.h"
#include"detropy.h"
#include"dpb.h"

#pragma once

struct adc_decoder {};

class Decoder : public adc_decoder
{
public:

    int                m_poc; 
    adc_param          m_param;
    adc_stats          m_stats;
    Detropy            m_detropy;
    DPB*               m_dpb;

    Decoder();
    ~Decoder()
    {
    };

    void create();
    void destroy();
    void printSummary();

    int  decodeVPS(adc_nal *nal);
    int  quadtree(Frame* curFrame, uint32_t X, uint32_t Y, uint32_t width, uint32_t height, YUVType yuv);
    int  decodeFrame(adc_nal *nal, adc_picture *pic_out);
    int  decode(adc_nal *nal, adc_picture *pic_out);

};
