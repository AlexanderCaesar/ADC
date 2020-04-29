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
#include "adc_config.h"
#include "files.h"
#include "AnnexBread.h"

#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (adcconfig.parse_arg(argc, argv))
    {
        return 0;
    }

    LogSetLevel(LogLevel(adcconfig.adc_p.logLevel));
    LogSetCallback(NULL, adcconfig.log_file.c_str(), NULL);

    adc_decoder *decoder = adc_decoder_open(&adcconfig.adc_p);

    if (!decoder)
    {
        ERR("open decoder failed.");
        return 0;
    }

    Files files;

    if (files.opendecoderfile(adcconfig))
    {
        ERR("Failure open files\n");
        return 0;
    }

    ifstream bitstreamFile(adcconfig.input_file.c_str(), ifstream::in | ifstream::binary);
    if (!bitstreamFile)
    {
        ERR("failed to open bitstream file `%s' for reading\n", adcconfig.input_file.c_str());
        return 0;
    }

    InputByteStream bytestream(bitstreamFile);
    adc_nal nal;

    adc_picture pic_out;
    adc_picture *pic_recon = &pic_out;

    int nal_count = 0;
    while (!!bitstreamFile)
    {
        /* location serves to work around a design fault in the decoder, whereby
        * the process of reading a new slice that is the first slice of a new frame
        * requires the TDecTop::decode() method to be called again with the same
        * nal unit. */
        streampos location = bitstreamFile.tellg();
        AnnexBStats stats = AnnexBStats();

        vector<uint8_t> nalUnit;
        byteStreamNALUnit(bytestream, nalUnit, stats);

        // call actual decoding function
        bool bNewPicture = false;
        if (nalUnit.empty())
        {
            /* this can happen if the following occur:
            *  - empty input file
            *  - two back-to-back start_code_prefixes
            *  - start_code_prefix immediately followed by EOF
            */
            ERR("Warning: Attempt to decode an empty NAL unit\n");

            break;
        }

        nal.payload = nalUnit.data();
        nal.type = nalUnit.data()[0];
        nal.sizeBytes = nalUnit.size();

        if(adc_decoder_decode(decoder,&nal,pic_recon) < 0)
        {
            ERR("Decode Frame failed");
            return 0;
        }
        nal_count++;
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

    adc_decoder_close(decoder);
    files.closefile();
    return 0;
}