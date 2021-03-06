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
#include"picyuv.h"

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
    m_detropy.setParam(&m_param);
    return m_detropy.decodeVPS(&m_param,nal);
}

int Decoder::quadtree(Frame* curFrame, uint32_t X, uint32_t Y, uint32_t width, uint32_t height, YUVType yuv)
{
    int min = 0;
    int max = 0;

    if (width == 0 || height == 0)
    {
        return -1;
    }

    uint32_t split = 0;
    

    if ((width + height) > 2)
    {
        curFrame->m_partition[yuv][curFrame->m_part_len[yuv]++] = split;
        split = m_detropy.decode_split_flag(width,height);
    }

    if (!split)
    {
        pixel* rec = curFrame->m_reconPic->m_picOrg[yuv] + Y*curFrame->m_fencPic->m_stride[yuv] + X;

        int ref_mode = calBoderMode(rec, X, Y, width, height, curFrame->m_fencPic->m_stride[yuv]);

        uint32_t direction = 0;
        if (ref_mode < 0)
        {
            direction = m_detropy.decode_direction_flag(width, height);
            ref_mode = getDirectionRef(rec,width, height, curFrame->m_fencPic->m_stride[yuv], direction);
            curFrame->m_direction[yuv][curFrame->m_dir_len[yuv]++] = direction;
        }

        int32_t res = m_detropy.decode_res(width, height);
        curFrame->m_residual[yuv][curFrame->m_res_len[yuv]++] = res;
        int32_t mode = res + ref_mode;
        curFrame->m_reconPic->copyModePixel(X, Y, width, height, yuv, mode);
        return 0;
    }
    else
    {
        uint32_t ww = width >> 1;
        uint32_t hh = height >> 1;
        for (uint32_t subPartIdx = 0; subPartIdx < 4; subPartIdx++)
        {
            uint32_t XX = X + ww * (subPartIdx >> 1);
            uint32_t YY = Y + hh * (subPartIdx & 1);

            uint32_t www = ww + (width % 2) * (subPartIdx >> 1);
            uint32_t hhh = hh + (height % 2) * (subPartIdx & 1);

            quadtree(curFrame, XX, YY, www, hhh, yuv);

        }
        return 0;
    }
}

int Decoder::decodeFrame(adc_nal *nal, adc_picture *pic_out)
{

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

    Frame* curFrame = m_dpb->m_picList.first();
    if (!curFrame)
        return -1;

    m_detropy.convertPayloadToRBSP(nal->payload, nal->sizeBytes);
    m_detropy.lbac_dec_init(m_detropy.m_nal.payload, m_detropy.m_nal.payload + m_detropy.m_nal.sizeBytes);
    m_detropy.com_lbac_ctx_init();

    quadtree(curFrame, 0, 0, m_param.sourceWidth, m_param.sourceHeight, Y);
    quadtree(curFrame, 0, 0, m_param.sourceWidth >> 1, m_param.sourceHeight >> 1, U);
    quadtree(curFrame, 0, 0, m_param.sourceWidth >> 1, m_param.sourceHeight >> 1, V);

    if (pic_out)
    {
        pic_out->colorSpace = m_param.chromaFormat;
        pic_out->poc = curFrame->m_poc;
        pic_out->sourceHeight = m_param.sourceHeight;
        pic_out->sourceWidth = m_param.sourceWidth;
        for (int i = 0; i < 3; i++)
        {
            pic_out->planes[i] = inFrame->m_reconPic->m_picOrg[i];
            pic_out->stride[i] = inFrame->m_reconPic->m_stride[i];
        }
    }

    m_dpb->recycleUnreferenced();

    return 0;
}


int Decoder::decode(adc_nal *nal, adc_picture *pic_out)
{
    int   ret = -1;

    switch (nal->type)
    {
    case NAL_VPS:
        return decodeVPS(nal);
        break;
    case NAL_FRAME:
        ret = decodeFrame(nal, pic_out);
        return ret;
    default:
        ERR("unkown nal type %d", nal->type);
        return -1;
    }
    return ret;
}
