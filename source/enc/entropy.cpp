/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/14
* @see
*/


#include"entropy.h"


Entropy::Entropy()
{
}

void Entropy::codeVPS(adc_param *p)
{
    WRITE_UVLC(p->sourceHeight, "pic_width_in_luma_samples");
    WRITE_UVLC(p->sourceWidth, "pic_height_in_luma_samples");
    WRITE_UVLC(p->chromaFormat, "chroma_format_idc");
}
