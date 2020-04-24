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
#include "frame.h"
#include "picyuv.h"

#include "dpb.h"


DPB::~DPB()
{
    while (!m_freeList.empty())
    {
        Frame* curFrame = m_freeList.popFront();
        curFrame->destroy();
        delete curFrame;
    }

    while (!m_picList.empty())
    {
        Frame* curFrame = m_picList.popFront();
        curFrame->destroy();
        delete curFrame;
    }
}

void DPB::prepareEncode(Frame *newFrame)
{
    m_picList.pushFront(*newFrame);
}
void DPB::recycleUnreferenced()
{
    Frame *iterFrame = m_picList.first();

    while (iterFrame)
    {
        Frame *curFrame = iterFrame;
        iterFrame = iterFrame->m_next;
        curFrame->m_bHasReferences = false; //to be added
        if (!curFrame->m_bHasReferences)
        {
            // iterator is invalidated by remove, restart scan
            m_picList.remove(*curFrame);
            iterFrame = m_picList.first();
            m_freeList.pushBack(*curFrame);
        }
    }

}
