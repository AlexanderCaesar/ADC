/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/14
* @see
*/


#include "files.h"
#include "log.h"

Files::Files()
{
    input = NULL;
    output = NULL;
    recon = NULL;
    width = 0;
    height = 0;
    buf = NULL;
    framesize = 0;
    chromaFormat = ADC_CSP_I420;
}

int Files::openfile(ADCConfig adconfig)
{
    width = adcconfig.adc_p.sourceWidth;
    height = adcconfig.adc_p.sourceHeight;

    chromaFormat = adcconfig.adc_p.chromaFormat;
    if (adcconfig.adc_p.chromaFormat == ADC_CSP_I420)
    {
        framesize  = width*height;
        framesize += (width>>1)*(height>>1);
        framesize += (width >> 1)*(height >> 1);
    }
    else
    {
        ERR("other chromaFormat to be added");
        return -1;
    }
    if (framesize > 0)
    {
        buf = (uint8_t*)malloc(framesize * sizeof(uint8_t));
    }
    else
    {
        ERR("buf check failed  framesize %d", framesize);
        return -1;
    }
    if (adcconfig.input_file.length() > 1)
    {
        input = fopen(adcconfig.input_file.c_str(), "rb");
        if (!input)
        {
            ERR("open file %s filed", adcconfig.input_file.c_str());
            return -1;
        }
    }

    if (adcconfig.output_file.length() > 1)
    {
        output = fopen(adcconfig.output_file.c_str(), "wb");
        if (!output)
        {
            ERR("open file %s filed", adcconfig.output_file.c_str());
            return -1;
        }
    }

    if (adcconfig.rec_file.length() > 1)
    {
        recon = fopen(adcconfig.rec_file.c_str(), "wb");
        if (!recon)
        {
            ERR("open file %s filed", adcconfig.rec_file.c_str());
            return -1;
        }
    }

    return 0;
}

int Files::opendecoderfile(ADCConfig adconfig)
{
    if (adcconfig.output_file.length() > 1)
    {
        output = fopen(adcconfig.output_file.c_str(), "wb");
        if (!output)
        {
            ERR("open file %s filed", adcconfig.output_file.c_str());
            return -1;
        }
    }

    if (adcconfig.rec_file.length() > 1)
    {
        recon = fopen(adcconfig.rec_file.c_str(), "wb");
        if (!recon)
        {
            ERR("open file %s filed", adcconfig.rec_file.c_str());
            return -1;
        }
    }

    return 0;
}

void Files::closefile()
{
    if (buf)
    {
        free(buf);
        buf = NULL;
    }
    if (input)
    {
        fclose(input);
        input = NULL;
    }
    if (output)
    {
        fclose(output);
        output = NULL;
    }
    if (recon)
    {
        fclose(recon);
        recon = NULL;
    }
}

int Files::writeNAL(FILE *fp, adc_nal* nal, uint32_t nalcount)
{
    if (!fp)
    {
        ERR("file open failed\n");
        return -1;
    }

    uint32_t bytes = 0;

    for (uint32_t i = 0; i < nalcount; i++)
    {
        fwrite((const void*)nal->payload, 1, nal->sizeBytes, fp);
        bytes += nal->sizeBytes;
        nal++;
    }

    return bytes;
}

int Files::writeHeaders(adc_nal* nal, uint32_t nalcount)
{
    return writeNAL(output, nal, nalcount);
}

int Files::readPicture(adc_picture& pic)
{
    pic.colorSpace = chromaFormat;
    pic.stride[0] = width;
    int size = fread(buf, 1, framesize, input);
    if (size < framesize)
    {
        return -1;
    }
    if (chromaFormat == ADC_CSP_I420)
    {
        pic.stride[1] = pic.stride[0] >> 1;
        pic.stride[2] = pic.stride[0] >> 1;
        pic.planes[0] = (uint8_t*)buf;
        pic.planes[1] = (uint8_t*)pic.planes[0] + pic.stride[0] * height;
        pic.planes[2] = (uint8_t*)pic.planes[1] + pic.stride[1] * (height >> 1);
    }
    else
    {
        ERR("other chromaFormat to be added");
        return -1; 
    }
    return 0;
}