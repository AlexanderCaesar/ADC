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

#pragma once

class BitInterface
{
public:

    virtual void     write(uint32_t val, uint32_t numBits) = 0;
    virtual void     writeByte(uint32_t val) = 0;
    virtual void     resetBits() = 0;
    virtual uint32_t getNumberOfWrittenBits() const = 0;
    virtual void     writeAlignOne() = 0;
    virtual void     writeAlignZero() = 0;
    virtual ~BitInterface() {}
};

class SyntaxElementWriter
{
public:

    BitInterface* m_bitIf;

    SyntaxElementWriter() : m_bitIf(NULL) {}

    /* silently discard the name of the syntax element */
    inline void WRITE_CODE(uint32_t code, uint32_t length, const char *) { writeCode(code, length); }
    inline void WRITE_UVLC(uint32_t code, const char *) { writeUvlc(code); }
    inline void WRITE_SVLC(int32_t  code, const char *) { writeSvlc(code); }
    inline void WRITE_FLAG(bool flag, const char *) { writeFlag(flag); }

    void writeCode(uint32_t code, uint32_t length) { m_bitIf->write(code, length); }
    void writeUvlc(uint32_t code);
    void writeSvlc(int32_t code)                   { uint32_t ucode = (code <= 0) ? -code << 1 : (code << 1) - 1; writeUvlc(ucode); }
    void writeFlag(bool code)                      { m_bitIf->write(code, 1); }
};

class Bitstream : public BitInterface
{
public:

    Bitstream();
    ~Bitstream()
    { 
        if (m_fifo)
        {
            free(m_fifo);
            m_fifo = NULL;
        }
    }

    void     resetBits()                     { m_partialByteBits = m_byteOccupancy = 0; m_partialByte = 0; }

private:

    uint8_t *m_fifo;
    uint32_t m_byteAlloc;
    uint32_t m_byteOccupancy;
    uint32_t m_partialByteBits;
    uint8_t  m_partialByte;
};

