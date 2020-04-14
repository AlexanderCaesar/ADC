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
