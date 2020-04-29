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
#include"frame.h"

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

int adc_decoder_decode(adc_decoder* dec, adc_nal *nal, adc_picture *pic_out)
{
    if (nal && dec)
    {
        Decoder *decoder = static_cast<Decoder*>(dec);
        return decoder->decode(nal,pic_out);
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
    m_dpb = NULL;
}

void Decoder::create()
{
    m_dpb = new DPB(&m_param);
}

void Decoder::destroy()
{
    delete m_dpb;
}

void Decoder::printSummary()
{
    INF("SUMMARY");
}

int Decoder::decodeVPS(adc_nal *nal)
{
    return m_detropy.decodeVPS(&m_param,nal);
}



/*   adc_nal *pp = &nallist.m_nal[0];
uint8_t        *cur, *end;
com_lbac_t     lbac_dec;

cur = pp->payload + 4 + 1;
end = pp->payload + pp->sizeBytes;

lbac_dec_init(&lbac_dec, cur, end);
com_lbac_ctx_init(&(lbac_dec.ctx));

uint8_t split_flag = 0;

for (int i = 0; i < 8; i++)
{
split_flag = decode_split_flag(&lbac_dec);
}*/
int Decoder::decodeFrame(adc_nal *nal, adc_picture *pic_out)
{
    Frame* curFrame = m_dpb->m_picList.first();
    if (!curFrame)
        return -1;

    return 0;//m_detropy.decodeVPS(&m_param, nal);
}


int Decoder::decode(adc_nal *nal, adc_picture *pic_out)
{
    int   ret = -1;
    Frame *inFrame;

    if (m_dpb->m_freeList.empty())
    {
        inFrame = new Frame;
        inFrame->m_encodeStartTime = time_mdate();

        if (inFrame->create(&m_param))
        {
            ERR("memory allocation failure, aborting encode\n");
            inFrame->destroy();
            delete inFrame;
            return -1;
        }

    }
    else
    {
        inFrame = m_dpb->m_freeList.popBack();
        inFrame->m_encodeStartTime = time_mdate();
        for (int i = 0; i < 3; i++)
        {
            inFrame->m_part_len[i] = 0;
            inFrame->m_dir_len[i] = 0;
            inFrame->m_res_len[i] = 0;
        }
    }

    inFrame->m_poc = ++m_poc;

    m_dpb->prepareEncode(inFrame);

    switch (nal->type)
    {
    case NAL_VPS:
        return decodeVPS(nal);
        break;
    case NAL_FRAME:
        ret = decodeFrame(nal, pic_out);
        m_dpb->recycleUnreferenced();
        return ret;
    default:
        ERR("unkown nal type %d", nal->type);
        return -1;
    }
    return ret;
}
