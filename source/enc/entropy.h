/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/14
* @see
*/


#include "log.h"
#include "adc.h"
#include "nal.h"
#include "bitstream.h"

#pragma once

/* MCABAC (START) */
#define PROB_BITS           11 // LPS_PROB(10-bit) + MPS(1-bit)
#define PROB_MASK           ((1 << PROB_BITS) - 1) // mask for LPS_PROB + MPS
#define MAX_PROB            ((1 << PROB_BITS) - 1) // equal to PROB_LPS + PROB_MPS, 0x7FF
#define HALF_PROB           (MAX_PROB >> 1)
#define PROB_INIT           (HALF_PROB << 1) /* 1/2 of initialization = (HALF_PROB << 1)+ MPS(0) */
#define LG_PMPS_SHIFTNO     2
#define QUAR_HALF_PROB      (1 << (PROB_BITS-3))

typedef int16_t lbac_ctx_model_t;

/* context models for arithemetic coding */
typedef struct uavs3e_com_lbac_all_ctx_t {
    lbac_ctx_model_t   part_split_flag;
    lbac_ctx_model_t   dir_flag;
    lbac_ctx_model_t   sign_flag;
    lbac_ctx_model_t   res;
} com_lbac_all_ctx_t;

typedef struct adc_lbac_t {
    uint32_t            range;
    uint32_t            code;
    int32_t             left_bits;
    uint32_t            stacked_ff;
    uint32_t            pending_byte;
    uint32_t            is_pending_byte;
    uint32_t            bitcounter;
    com_lbac_all_ctx_t   h;
} lbac_t;


class Entropy : public SyntaxElementWriter
{
public:

    lbac_t lbac_enc;

    Entropy();

    void com_lbac_ctx_init(com_lbac_all_ctx_t *lbac_ctx);
    void lbac_reset();
    void setBitstream(Bitstream* p)    { m_bitIf = p; }

    void lbac_encode_bin(uint32_t bin, lbac_t *lbac, lbac_ctx_model_t *model, Bitstream  *bs);
    void lbac_finish(lbac_t *lbac, Bitstream  *bs);

    void codeVPS(adc_param *p);
};


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

void lbac_dec_init(com_lbac_t *lbac, uint8_t *cur, uint8_t* end);
uint8_t decode_split_flag(com_lbac_t * lbac);