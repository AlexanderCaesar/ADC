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
#include "common.h"

#pragma once

/* MCABAC (START) */
#define MAX_PROB            ((1 << PROB_BITS) - 1) // equal to PROB_LPS + PROB_MPS, 0x7FF
#define HALF_PROB           (MAX_PROB >> 1)
#define PROB_INIT           (HALF_PROB << 1) /* 1/2 of initialization = (HALF_PROB << 1)+ MPS(0) */
#define LG_PMPS_SHIFTNO     2
#define QUAR_HALF_PROB      (1 << (PROB_BITS-3))

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
    void codeSplit(uint32_t val);
    void codeDirection(uint32_t val);
    void codeDirRes(int32_t val);
};
