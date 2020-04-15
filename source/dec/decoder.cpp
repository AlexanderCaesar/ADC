/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/15
* @see
*/


#include"log.h"
#include"adc.h"
#include"decoder.h"

adc_decoder *adc_decoder_open(adc_param *p)
{
    Decoder* decoder = NULL;

    decoder = new Decoder;

    memcpy(&decoder->m_param, p, sizeof(adc_param));

    decoder->create();

    INF("ADC decoder version %s", ADC_BUILD);

    return decoder;
}

void adc_decoder_close(adc_decoder* dec)
{
    if (dec)
    {
        Decoder *decoder = static_cast<Decoder*>(dec);
        decoder->printSummary();
        decoder->destroy();
        delete decoder;
    }
}

int adc_decoder_headers(adc_decoder* dec, adc_nal *nal)
{
    if (nal && dec)
    {
        Decoder *decoder = static_cast<Decoder*>(dec);
        return decoder->decodeVPS(nal);
    }
    return -1;
}

int adc_decoder_decode(adc_decoder* dec, adc_nal *nal)
{
    if (nal && dec)
    {
        Decoder *decoder = static_cast<Decoder*>(dec);
        return decoder->decode(nal);
    }
    return -1;
}
Decoder::Decoder()
{
    m_poc = -1;
    m_stats.globalPsnrY = 0;
    m_stats.globalPsnrU = 0;
    m_stats.globalPsnrV = 0;
    m_stats.globalPsnr = 0;
    m_stats.globalSsim = 0;
    m_stats.elapsedEncodeTime = 0;
    m_stats.elapsedVideoTime = 0;
    m_stats.bitrate = 0;
    m_stats.accBits = 0;
    m_stats.encodedPictureCount = 0;

}

void Decoder::create()
{

}
void Decoder::destroy()
{
}

void Decoder::printSummary()
{
    INF("SUMMARY");
}

int Decoder::decodeVPS(adc_nal *nal)
{
    return 0;
}
int Decoder::decode(adc_nal *nal)
{
    switch (nal->type)
    {
    case NAL_VPS:
        ;
        break;
    case NAL_FRAME:
        break;
    default:
        ERR("unkown nal type %d", nal->type);
        return -1;
    }
    return -1;
}
