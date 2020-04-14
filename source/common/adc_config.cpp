/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/13
* @see
*/

#include "adc_config.h"
#include "getopt.h"

ADCConfig adcconfig;
ADCConfig::ADCConfig()
{
    adc_p.bEnablePsnr = 1;
    adc_p.bEnableSsim = 0;
    adc_p.bRec = 0;
    adc_p.et = 0;
    adc_p.fpsNum = 25;
    adc_p.logLevel = 3;
    adc_p.totalFrames = 0;
    adc_p.chromaFormat = 1;
    log_file = "adc.log";
}

int ADCConfig::parse_arg(int argc, char* argv[])
{
    static struct option long_options[] = {
        { "version", no_argument, NULL, 'v' },
        { "help", no_argument, NULL, 'h' },
        { "input", required_argument, NULL, 'i' },
        { "output", required_argument, NULL, 'o' },
        { "rec", required_argument, NULL, 'R' },
        { "psnr", required_argument, NULL, 'P' },
        { "ssim", required_argument, NULL, 'S' },
        { "loglevel", required_argument, NULL, 'l' },
        { "fps", required_argument, NULL, 'f' },
        { "width", required_argument, NULL, 'W' },
        { "height", required_argument, NULL, 'H' },
        { "totalframes", required_argument, NULL, 't' },
        { "et", required_argument, NULL, 'e' },
        { "logfile", required_argument, NULL, 'g' },
        { 0, 0, 0, 0 }
    };

    int opt;
    int option_index = 0;
    char* optstring = "hl:v:h:i:o:R:l:P:S:f:W:H:t:e:g:"; //There must be an ":" in the end of string
    std::string temp;

    while ((opt = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 'v':
            printf("version %d\n", ADC_BUILD);
            return 1;
        case 'h':
            display_help();
            return 1;
        case 'i':
            input_file = optarg;
            break;

        case 'o':
            output_file = optarg;
            break;

        case 'R':
            rec_file = optarg;
            adc_p.bRec = 1;
            break;

        case 'P':
            adc_p.bEnablePsnr = atoi(optarg);
            break;

        case 'S':
            adc_p.bEnableSsim = atoi(optarg);
            break;

        case 'l':
            adc_p.logLevel = atoi(optarg);
            break;

        case 'f':
            adc_p.fpsNum = atoi(optarg);
            break;

        case 'W':
            adc_p.sourceWidth = atoi(optarg);
            break;

        case 'H':
            adc_p.sourceHeight = atoi(optarg);
            break;

        case 't':
            adc_p.totalFrames = atoi(optarg);
            break;

        case 'e':
            adc_p.et = atoi(optarg);
            break;

        case 'g':
            log_file = optarg;
            break;
        }
    }

    return 0;
}

void ADCConfig::display_help()
{
    printf("  --version\t\t display version\n");
    printf("  --help\t\t display this help\n");
    printf("stream options:\n");
    printf("  -i, --input <input filename>\n");
    printf("  -o, --output <output filename>\n");
    printf("  -R, --rec <reconstruction filename>\n");
    printf("  -P, --psnr <0,1> Enable the measurement and reporting of PSNR. Default is enabled\n");
    printf("  -S, --ssim <0,1> Enable the measurement and reporting of SSIM. Default is disabled\n");
    printf("  -l, --loglevel <loglevel> The level of logging detail emitted by the encoder.\n");
    printf("  -f, --fps <fps> Numerator and denominator of frame rate \n");
    printf("  -W, --width <width> Width (in pixels) of the source pictures.\n");
    printf("  -H, --height <height> Height (in pixels) of the source pictures.\n");
    printf("  -t, --totalframes <totalframes> Total Number of frames to be encoded\n");
    printf("  -e, --et <et> Error tolerance 0~255\n");
    printf("  -g, --logfile <logfile > logfile\n");
}