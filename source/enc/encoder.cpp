/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/13
* @see
*/


#include "log.h"
#include "adc.h"
#include "encoder.h"
#include "entropy.h"
#include "frame.h"
#include "picyuv.h"

adc_encoder *adc_encoder_open(adc_param *p)
{
    Encoder* encoder = NULL;

    encoder = new Encoder;

    memcpy(&encoder->m_param, p, sizeof(adc_param));

    encoder->create();

    INF("ADC encoder version %s", ADC_BUILD);
    INF("\n\
        psnr  %3d ssim   %3d    loglevel    %3d rec %2d \n\
        width %3d height %3d    totalframes %3d fps %2d\n\
        et    %3d chromaformat %d"
        ,encoder->m_param.bEnablePsnr
        , encoder->m_param.bEnableSsim
        , encoder->m_param.logLevel
        , encoder->m_param.bRec
        , encoder->m_param.sourceWidth
        , encoder->m_param.sourceHeight
        , encoder->m_param.totalFrames
        , encoder->m_param.fpsNum
        , encoder->m_param.et
        , encoder->m_param.chromaFormat);

    return encoder;
}

void adc_encoder_close(adc_encoder* enc)
{
    if (enc)
    {
        Encoder *encoder = static_cast<Encoder*>(enc);
        encoder->printSummary();
        encoder->destroy();
        delete encoder;
    }
}

int adc_encoder_headers(adc_encoder *enc, adc_nal **pp_nal, uint32_t *pi_nal)
{
    if (pp_nal && enc)
    {
        Encoder *encoder = static_cast<Encoder*>(enc);

        Entropy entropy;
        Bitstream bs;

        encoder->getStreamHeaders(encoder->m_nalList, entropy, bs);
        *pp_nal = &encoder->m_nalList.m_nal[0];
        if (pi_nal) *pi_nal = encoder->m_nalList.m_numNal;

        return encoder->m_nalList.m_occupancy;
    }
    return -1;
}

void adc_picture_init(adc_param *param, adc_picture *pic)
{
    memset(pic, 0, sizeof(adc_picture));

    pic->colorSpace = param->chromaFormat;
}

int adc_encoder_encode(adc_encoder *enc, adc_nal **pp_nal, uint32_t *pi_nal, adc_picture *pic_in, adc_picture *pic_out)
{
    if (!enc)
        return -1;

    Encoder *encoder = static_cast<Encoder*>(enc);
    int numEncoded = encoder->encode(pic_in, pic_out);

    {
        //to be done
    }

    return -1;
}

Encoder::Encoder()
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

void Encoder::create()
{
    m_dpb = new DPB(&m_param);
}
void Encoder::destroy()
{
    delete m_dpb;
}

void Encoder::printSummary()
{
    INF("SUMMARY");
}

void Encoder::getStreamHeaders(NALList& list, Entropy& entropy, Bitstream& bs)
{
    entropy.setBitstream(&bs);

    bs.resetBits();
    entropy.codeVPS(&m_param);
    bs.writeByteAlignment();
    list.serialize(NAL_VPS, bs);

    for (uint32_t i = 0; i < list.m_numNal; i++)
    {
        m_stats.accBits += list.m_nal[i].sizeBytes<<3;//*8
    }
}

int Encoder::quadtree(Frame* curFrame, uint32_t X, uint32_t Y, uint32_t width, uint32_t height,  YUVType yuv)
{
    int min = 0; 
    int max = 0;
    int mode = calCUMode(curFrame->m_fencPic->m_picOrg[yuv], width, height, curFrame->m_fencPic->m_stride[yuv],min,max);

    int split = (mode - min > m_param.et) || (max - mode > m_param.et);

    if (width > 1 && height > 1)
    {
        curFrame->m_partition[yuv][curFrame->m_part_len[yuv]++] = split;
    }
    if (!split)
    {
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

int Encoder::compressFrame()
{
    Frame* curFrame = m_dpb->m_picList.first();
    if (!curFrame)
        return -1;

    quadtree(curFrame, 0, 0, m_param.sourceWidth, m_param.sourceHeight, Y);
    quadtree(curFrame, 0, 0, m_param.sourceWidth >> 1, m_param.sourceHeight >> 1, U);
    quadtree(curFrame, 0, 0, m_param.sourceWidth >> 1, m_param.sourceHeight >> 1, V);

    return 0;
}

int Encoder::encode(const adc_picture* pic_in, adc_picture* pic_out)
{
    int ret = -1;
    if (pic_in)
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

        /* Copy input picture into a Frame and PicYuv, send to lookahead */
        inFrame->m_fencPic->copyFromPicture(*pic_in, m_param);

        inFrame->m_poc = ++m_poc;
        inFrame->m_pts = pic_in->pts;

        m_dpb->prepareEncode(inFrame);

        ret = compressFrame();

        m_dpb->recycleUnreferenced();
    }
    else
    {
        //to be added
    }
    return ret;
}
