/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/14
* @see
*/

#include "bitstream.h"

#define MIN_FIFO_SIZE 1000

Bitstream::Bitstream()
{
    m_fifo = (uint8_t*)malloc(MIN_FIFO_SIZE*sizeof(uint8_t));
    m_byteAlloc = MIN_FIFO_SIZE;
    resetBits();
}
void Bitstream::push_back(uint8_t val)
{
    if (!m_fifo)
        return;

    if (m_byteOccupancy >= m_byteAlloc)
    {
        /** reallocate buffer with doubled size */
        uint8_t *temp = (uint8_t*)malloc(m_byteAlloc * 2 * sizeof(uint8_t));
        if (temp)
        {
            memcpy(temp, m_fifo, m_byteOccupancy);
            if (m_fifo)
            {
                free(m_fifo);
                m_fifo = NULL;
            }
            m_fifo = temp;
            m_byteAlloc *= 2;
        }
        else
        {
            ERR("Unable to realloc bitstream buffer");
            return;
        }
    }
    m_fifo[m_byteOccupancy++] = val;
}

void Bitstream::write(uint32_t val, uint32_t numBits)
{
    uint32_t totalPartialBits = m_partialByteBits + numBits;
    uint32_t nextPartialBits = totalPartialBits & 7;
    uint8_t  nextHeldByte = val << (8 - nextPartialBits);
    uint32_t writeBytes = totalPartialBits >> 3;

    if (writeBytes)
    {
        /* topword aligns m_partialByte with the msb of val */
        uint32_t topword = (numBits - nextPartialBits) & ~7;
        uint32_t write_bits = (m_partialByte << topword) | (val >> nextPartialBits);

        switch (writeBytes)
        {
        case 4: push_back(write_bits >> 24);  // fall-through
        case 3: push_back(write_bits >> 16);  // fall-through
        case 2: push_back(write_bits >> 8);   // fall-through
        case 1: push_back(write_bits);
        }

        m_partialByte = nextHeldByte;
        m_partialByteBits = nextPartialBits;
    }
    else
    {
        m_partialByte |= nextHeldByte;
        m_partialByteBits = nextPartialBits;
    }
}

void Bitstream::writeByte(uint32_t val)
{
    // Only CABAC will call writeByte, the fifo must be byte aligned

    push_back(val);
}

void Bitstream::writeAlignOne()
{
    uint32_t numBits = (8 - m_partialByteBits) & 0x7;

    write((1 << numBits) - 1, numBits);
}

void Bitstream::writeAlignZero()
{
    if (m_partialByteBits)
    {
        push_back(m_partialByte);
        m_partialByte = 0;
        m_partialByteBits = 0;
    }
}

void Bitstream::writeByteAlignment()
{
    write(1, 1);
    writeAlignZero();
}

void SyntaxElementWriter::writeUvlc(uint32_t code)
{
    ++code;

    unsigned long idx;
    CLZ(idx, code);
    uint32_t length = (uint32_t)idx * 2 + 1;

    // Take care of cases where length > 32
    m_bitIf->write(0, length >> 1);
    m_bitIf->write(code, (length + 1) >> 1);
}

