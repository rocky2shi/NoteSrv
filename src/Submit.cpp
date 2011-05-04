// Rocky 2010-05-11 21:42:15
#include "Page.h"
#include "Submit.h"
namespace PAGE_SPACE
{



// 注册（把子类对象加入m_queue）
int Submit::Manage::Register(const string &page, const string &element, Submit *sub)
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
    assert(NULL == m_queue[ page ][ element ]);   // 这时的位置应为空
    m_queue[ page ][ element ] = sub;  // 加入队列
    sub->m_page = page;
    sub->m_element = element;
    // 这时日志模块还不可用 [XXX]
    //printf("Register: page=[%s], element=[%s], obj=[%p]... [OK]\n",
    //                                page.c_str(), element.c_str(), sub);
    return OK;
}


// 取处理元素
Submit *Submit::Manage::Get(const string &page, const string &element)
{
    Factory::iterator it = m_queue.find( page );
    if(m_queue.end() != it)
    {
        map<string, Submit *> &elements = it->second;
        map<string, Submit *>::iterator itElement = elements.find( element );
        if(elements.end() != itElement)
        {
            return itElement->second;
        }
    }
    return NULL;
}




/*****************************************************************************/



Submit::Submit()
{
}

Submit::Submit(const string &page, const string &element)
{
    instance()->Register(page, element, this);
}

Submit::~Submit()
{
}



// 类初始化（的程序启动时执行一次）
int Submit::init()
{
    return OK;
}

// 跟据传入的id[page,tag]从工厂中产生一个处理对象
Submit *Submit::New(const string &page, const string &element)
{
    // 在工厂中找创建器，并创建子类对象；
    Submit *parent = instance()->Get(page, element);
    if(NULL == parent)
    {
        LOG_ERROR("Not exist obj: page=[%s] element=[%s]", page.c_str(), element.c_str());
        return NULL;
    }
    Submit *sub = parent->DoNew();
    if(NULL == sub)
    {
        return NULL;
    }
    // 传递值到子对象
    sub->m_page = parent->m_page;
    sub->m_element = parent->m_element;
    return sub;
}


// 各子类负责实现自已初始化操作（子类应重实现此接口）
int Submit::DoInit()
{
    LOG_ERROR("Must implement on child class");
    return ERR;
}

// 各子类负责实现自已的对象创建器（子类应重实现此接口）
Submit *Submit::DoNew()
{
    LOG_ERROR("Must implement on child class");
    return NULL;
}






/*
 * ========================== 以下为业务处理代码 ==========================
 */


// 处理请求
int Submit::Deal(Page *page)
{
    return ERR;
}







}// end of PAGE_SPACE

