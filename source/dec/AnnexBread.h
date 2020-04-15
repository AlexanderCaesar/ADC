/**
* @file
* @brief
* @author    Copyright (c) 2010-2014, ITU/ISO/IEC
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/15
* @see
*/

/**
 \file     AnnexBread.h
 \brief    reading functions for Annex B byte streams
 */

#pragma once

#include <stdint.h>
#include <istream>
#include <vector>
#include <assert.h>

//! \ingroup TLibDecoder
//! \{

class InputByteStream
{
public:
  /**
   * Create a bytestream reader that will extract bytes from
   * istream.
   *
   * NB, it isn't safe to access istream while in use by a
   * InputByteStream.
   *
   * Side-effects: the exception mask of istream is set to eofbit
   */
  InputByteStream(std::istream& istream)
  : m_NumFutureBytes(0)
  , m_FutureBytes(0)
  , m_Input(istream)
  {
    istream.exceptions(std::istream::eofbit | std::istream::badbit);
  }

  /**
   * Reset the internal state.  Must be called if input stream is
   * modified externally to this class
   */
  void reset()
  {
    m_NumFutureBytes = 0;
    m_FutureBytes = 0;
  }

  /**
   * returns true if an EOF will be encountered within the next
   * n bytes.
   */
  bool eofBeforeNBytes(uint8_t n)
  {
    assert(n <= 4);
    if (m_NumFutureBytes >= n)
      return false;

    n -= m_NumFutureBytes;
    try
    {
      for (uint32_t i = 0; i < n; i++)
      {
        m_FutureBytes = (m_FutureBytes << 8) | m_Input.get();
        m_NumFutureBytes++;
      }
    }
    catch (...)
    {
      return true;
    }
    return false;
  }

  /**
   * return the next n bytes in the stream without advancing
   * the stream pointer.
   *
   * Returns: an unsigned integer representing an n byte bigendian
   * word.
   *
   * If an attempt is made to read past EOF, an n-byte word is
   * returned, but the portion that required input bytes beyond EOF
   * is undefined.
   *
   */
  uint32_t peekBytes(uint32_t n)
  {
    eofBeforeNBytes(n);
    return m_FutureBytes >> 8*(m_NumFutureBytes - n);
  }

  /**
   * consume and return one byte from the input.
   *
   * If bytestream is already at EOF prior to a call to readByte(),
   * an exception std::ios_base::failure is thrown.
   */
  uint8_t readByte()
  {
    if (!m_NumFutureBytes)
    {
      uint8_t byte = m_Input.get();
      return byte;
    }
    m_NumFutureBytes--;
    uint8_t wanted_byte = m_FutureBytes >> 8*m_NumFutureBytes;
    m_FutureBytes &= ~(0xff << 8*m_NumFutureBytes);
    return wanted_byte;
  }

  /**
   * consume and return n bytes from the input.  n bytes from
   * bytestream are interpreted as bigendian when assembling
   * the return value.
   */
  uint32_t readBytes(uint32_t n)
  {
    uint32_t val = 0;
    for (uint32_t i = 0; i < n; i++)
      val = (val << 8) | readByte();
    return val;
  }

private:
  uint32_t m_NumFutureBytes; /* number of valid bytes in m_FutureBytes */
  uint32_t m_FutureBytes; /* bytes that have been peeked */
  std::istream& m_Input; /* Input stream to read from */
};

/**
 * Statistics associated with AnnexB bytestreams
 */
struct AnnexBStats
{
  uint32_t m_numLeadingZero8BitsBytes;
  uint32_t m_numZeroByteBytes;
  uint32_t m_numStartCodePrefixBytes;
  uint32_t m_numBytesInNALUnit;
  uint32_t m_numTrailingZero8BitsBytes;

  AnnexBStats& operator+=(const AnnexBStats& rhs)
  {
    this->m_numLeadingZero8BitsBytes += rhs.m_numLeadingZero8BitsBytes;
    this->m_numZeroByteBytes += rhs.m_numZeroByteBytes;
    this->m_numStartCodePrefixBytes += rhs.m_numStartCodePrefixBytes;
    this->m_numBytesInNALUnit += rhs.m_numBytesInNALUnit;
    this->m_numTrailingZero8BitsBytes += rhs.m_numTrailingZero8BitsBytes;
    return *this;
  }
};

bool byteStreamNALUnit(InputByteStream& bs, std::vector<uint8_t>& nalUnit, AnnexBStats& stats);

//! \}
