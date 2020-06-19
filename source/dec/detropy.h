/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/15
* @see
*/


#include "log.h"
#include "adc.h"
#include "nal.h"
#include "bitstream.h"
#include "common.h"

#pragma once

/*************************  CABAC ******************************************************/
#define PROB_LPS_BITS                     8

#define HALF_RANGE                        ( 1 << (PROB_LPS_BITS) )           //  256
#define MAX_LPS_RANGE                     ((1 << (PROB_LPS_BITS    )) - 1)   //  255
#define MAX_RANGE                         ((1 << (PROB_LPS_BITS + 1)) - 1)   //  511

typedef struct uavs3d_com_lbac_t {
    int32_t  range, low;
    uint8_t  *cur, *end;
    com_lbac_all_ctx_t ctx;
} com_lbac_t;


class Detropy 
{
public:
    adc_nal        m_nal;
    com_lbac_t     lbac_dec;
    int            m_occupancy;
    adc_param      *m_param;

    Detropy();
    ~Detropy()
    {
        if (m_nal.payload)
        {
            free(m_nal.payload);
            m_nal.payload = NULL;
        }
    }

    void setParam(adc_param  *param);
    void convertPayloadToRBSP(uint8_t* src, int src_len);
    int  decodeVPS(adc_param *p, adc_nal *nal);

    void lbac_dec_init(uint8_t *cur, uint8_t* end);
    void com_lbac_ctx_init();

    uint8_t decode_split_flag(uint32_t width, uint32_t height);
    uint8_t decode_direction_flag(uint32_t width, uint32_t height);
    int32_t decode_res(uint32_t width, uint32_t height);

};