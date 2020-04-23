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
#include "piclist.h"
#include "frame.h"

void PicList::pushFront(Frame& curFrame)
{
    curFrame.m_next = m_start;
    curFrame.m_prev = NULL;

    if (m_count)
    {
        m_start->m_prev = &curFrame;
        m_start = &curFrame;
    }
    else
    {
        m_start = m_end = &curFrame;
    }
    m_count++;
}

void PicList::pushBack(Frame& curFrame)
{
    curFrame.m_next = NULL;
    curFrame.m_prev = m_end;

    if (m_count)
    {
        m_end->m_next = &curFrame;
        m_end = &curFrame;
    }
    else
    {
        m_start = m_end = &curFrame;
    }
    m_count++;
}

Frame *PicList::popFront()
{
    if (m_start)
    {
        Frame *temp = m_start;
        m_count--;

        if (m_count)
        {
            m_start = m_start->m_next;
            m_start->m_prev = NULL;
        }
        else
        {
            m_start = m_end = NULL;
        }
        temp->m_next = temp->m_prev = NULL;
        return temp;
    }
    else
        return NULL;
}

Frame* PicList::getPOC(int poc)
{
    Frame *curFrame = m_start;
    while (curFrame && curFrame->m_poc != poc)
        curFrame = curFrame->m_next;
    return curFrame;
}

Frame *PicList::popBack()
{
    if (m_end)
    {
        Frame* temp = m_end;
        m_count--;

        if (m_count)
        {
            m_end = m_end->m_prev;
            m_end->m_next = NULL;
        }
        else
        {
            m_start = m_end = NULL;
        }
        temp->m_next = temp->m_prev = NULL;
        return temp;
    }
    else
        return NULL;
}

Frame* PicList::getCurFrame(void)
{
    Frame *curFrame = m_start;
    if (curFrame != NULL)
        return curFrame;
    else
        return NULL;
}

void PicList::remove(Frame& curFrame)
{
    m_count--;
    if (m_count)
    {
        if (m_start == &curFrame)
            m_start = curFrame.m_next;
        if (m_end == &curFrame)
            m_end = curFrame.m_prev;

        if (curFrame.m_next)
            curFrame.m_next->m_prev = curFrame.m_prev;
        if (curFrame.m_prev)
            curFrame.m_prev->m_next = curFrame.m_next;
    }
    else
    {
        m_start = m_end = NULL;
    }

    curFrame.m_next = curFrame.m_prev = NULL;
}
