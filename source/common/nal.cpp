/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/14
* @see
*/

#pragma once

#include "nal.h"

NALList::NALList()
    : m_numNal(0)
    , m_buffer(NULL)
    , m_occupancy(0)
    , m_allocSize(0)
    , m_extraBuffer(NULL)
    , m_extraOccupancy(0)
    , m_extraAllocSize(0)
    , m_annexB(true)
{}

void NALList::reset()
{
    m_numNal = 0;
    m_occupancy = 0;
    m_extraOccupancy = 0;
    m_extraAllocSize = 0;

}
void NALList::serialize(NalUnitType nalUnitType, const Bitstream& bs)
{
    static const char startCodePrefix[] = { 0, 0, 0, 1 };

    uint32_t payloadSize = bs.getNumberOfWrittenBytes();
    const uint8_t* bpayload = bs.getFIFO();
    if (!bpayload)
        return;

    uint32_t nextSize = m_occupancy + sizeof(startCodePrefix) + 2 + payloadSize + (payloadSize >> 1) + m_extraOccupancy;
    if (nextSize > m_allocSize)
    {
        uint8_t *temp = (uint8_t*)malloc(nextSize*sizeof(uint8_t));
        if (temp)
        {
            memcpy(temp, m_buffer, m_occupancy);

            /* fixup existing payload pointers */
            for (uint32_t i = 0; i < m_numNal; i++)
                m_nal[i].payload = temp + (m_nal[i].payload - m_buffer);

            if (m_buffer)
            {
                free(m_buffer);
                m_buffer = NULL;
            }
            m_buffer = temp;
            m_allocSize = nextSize;
        }
        else
        {
            ERR("Unable to realloc access unit buffer\n");
            return;
        }
    }

    uint8_t *out = m_buffer + m_occupancy;
    uint32_t bytes = 0;

    if (!m_annexB)
    {
        /* Will write size later */
        bytes += 4;
    }
    else if (!m_numNal || nalUnitType == NAL_VPS || nalUnitType == NAL_FRAME)
    {
        memcpy(out, startCodePrefix, 4);
        bytes += 4;
    }
    else
    {
        memcpy(out, startCodePrefix + 1, 3);
        bytes += 3;
    }

    /* 8 bit NAL header: */
    out[bytes++] = (uint8_t)nalUnitType;

    /* 7.4.1 ...
    * Within the NAL unit, the following three-byte sequences shall not occur at
    * any byte-aligned position:
    *  - 0x000000
    *  - 0x000001
    *  - 0x000002 */
    for (uint32_t i = 0; i < payloadSize; i++)
    {
        if (i > 2 && !out[bytes - 2] && !out[bytes - 3] && out[bytes - 1] <= 0x03)
        {
            /* inject 0x03 to prevent emulating a start code */
            out[bytes] = out[bytes - 1];
            out[bytes - 1] = 0x03;
            bytes++;
        }

        out[bytes++] = bpayload[i];
    }

    if (m_extraOccupancy)
    {
        /* these bytes were escaped by serializeSubstreams */
        memcpy(out + bytes, m_extraBuffer, m_extraOccupancy);
        bytes += m_extraOccupancy;
        m_extraOccupancy = 0;
    }

    /* 7.4.1.1
    * ... when the last byte of the RBSP data is equal to 0x00 (which can
    * only occur when the RBSP ends in a cabac_zero_word), a final byte equal
    * to 0x03 is appended to the end of the data.  */
    if (!out[bytes - 1])
        out[bytes++] = 0x03;

    if (!m_annexB)
    {
        uint32_t dataSize = bytes - 4;
        out[0] = (uint8_t)(dataSize >> 24);
        out[1] = (uint8_t)(dataSize >> 16);
        out[2] = (uint8_t)(dataSize >> 8);
        out[3] = (uint8_t)dataSize;
    }

    m_occupancy += bytes;

    adc_nal& nal = m_nal[m_numNal++];
    nal.type = nalUnitType;
    nal.sizeBytes = bytes;
    nal.payload = out;
}
