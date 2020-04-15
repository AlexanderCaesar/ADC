/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/15
* @see
*/


#include"detropy.h"
#include"stdio.h"
#include"stdlib.h"


int u(uint8_t* data, unsigned bits, int start_bit) {
    int r = 0;
    int offset = start_bit;

    for (unsigned i = 0; i < bits; i++) {
        r <<= 1;

        if (data[offset / 8] & (0x80 >> offset % 8)) {
            r++;
        }

        offset++;
    }

    return r;
}

int ue(uint8_t* data, int& start_bit) {
    int zero_bits = 0;
    int start = start_bit;

    while (u(data, 1, start) == 0) {
        zero_bits++;
        start++;
    }

    start++;

    int r = (int)(pow(2, zero_bits) - 1 + u(data, zero_bits, start));
    //    dumpBits(data,  zero_bits * 2 + 1, start_bit);
    start_bit = start + zero_bits;
    return r;
}

int se(uint8_t* data, int& start_bit) {
    auto ue_val = ue(data, start_bit);
    return (int)(pow(-1, ue_val + 1) * ceil(ue_val / 2));
}

Detropy::Detropy()
{
    m_occupancy = 0;
    m_nal.type = 0;
    m_nal.payload = NULL;
    m_nal.sizeBytes = 0;
}

void Detropy::convertPayloadToRBSP(uint8_t* src, int src_len)
{
    int zeroCount = 0;

    if (m_occupancy < src_len * 2)
    {
        if (m_nal.payload)
        {
            free(m_nal.payload);
            m_nal.payload = NULL;
        }
        m_occupancy = src_len * 2;
        m_nal.payload = (uint8_t*)malloc(m_occupancy*sizeof(uint8_t));

    }
    m_nal.sizeBytes = 0;
    for (int i = 1; i < src_len; i++) //nal type 1 byte  hevc nal type 2bytes
    {
        if ((i + 2 < src_len) && (src[i] == 0x00 && src[i + 1] == 0x00 && src[i + 2] == 0x03))
        {
            m_nal.payload[m_nal.sizeBytes++] = src[i];
            m_nal.payload[m_nal.sizeBytes++] = src[i + 1];
            i += 2;
        }
        else
        {
            m_nal.payload[m_nal.sizeBytes++] = src[i];
        }
    }

}

int Detropy::decodeVPS(adc_param *p, adc_nal *nal)
{
    convertPayloadToRBSP(nal->payload, nal->sizeBytes);
    uint32_t uiCode = 0;
    int      start_bit = 0;

    uint8_t* ptr = m_nal.payload;
    p->sourceHeight = ue(ptr, start_bit);
    p->sourceWidth = ue(ptr, start_bit);
    p->chromaFormat = ue(ptr, start_bit);

    return 0;
}
