/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/23
* @see
*/

#include "common.h"
#include "adc.h"

#pragma once 


class Frame;

class PicList
{
protected:

    Frame*   m_start;
    Frame*   m_end;
    int      m_count;

public:

    PicList()
    {
        m_start = NULL;
        m_end   = NULL;
        m_count = 0;
    }

    /** Push picture to end of the list */
    void pushBack(Frame& pic);

    /** Push picture to beginning of the list */
    void pushFront(Frame& pic);

    /** Pop picture from end of the list */
    Frame* popBack();

    /** Pop picture from beginning of the list */
    Frame* popFront();

    /** Find frame with specified POC */
    Frame* getPOC(int poc);

    /** Get the current Frame from the list **/
    Frame* getCurFrame(void);

    /** Remove picture from list */
    void remove(Frame& pic);

    Frame* first()        { return m_start;   }

    Frame* last()         { return m_end;     }

    int size()            { return m_count;   }

    bool empty() const    { return !m_count;  }

    operator bool() const { return !!m_count; }
};

