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

class Entropy : public SyntaxElementWriter
{
public:

    Entropy();

    void setBitstream(Bitstream* p)    { m_bitIf = p; }

    void Entropy::codeVPS(adc_param *p);

};