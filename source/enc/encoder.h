/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/14
* @see
*/


#include"log.h"
#include"adc.h"

struct adc_encoder {};

class Encoder : public adc_encoder
{
public:

    int                m_poc; 
    adc_param          m_param;

    Encoder();
    ~Encoder()
    {
    };

    void create();
    void destroy();
    void printSummary();

    int encode();

};
