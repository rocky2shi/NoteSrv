// Rocky 2010-04-09 14:19:58
#include "Page.h"
#include "Tag.h"
namespace PAGE_SPACE
{










Tag::Factory Tag::m_Factory;


Tag::Tag()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag::Tag(const string &page, const string &tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    Register(page, tag, this);
}

Tag::~Tag()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}


/*
 * 功能：注册子类对象到工厂
 * 编写：Rocky 2009-09-07 17:09:46
 * 返回：正确返回>=0值，出错返<0值；
 * 注意：此函数在进入main()函数之前就由构造函数调用了
 */
int Tag::Register(const string &page, const string &tag, Tag *sub)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    if(NULL != m_Factory[ page ][ tag ])
    {
        printf("Register error, page=[%s], tag=[%s]\n", page.c_str(), tag.c_str()); // 2010-05-13 17:12:06
        assert(NULL == m_Factory[ page ][ tag ]);   // 这时的位置应为空
        return ERR;
    }
    m_Factory[ page ][ tag ]  = sub;  // 加入队列
    m_id = tag;
    return OK;
}

/*
 * 功能：静态初始化设置
 * 编写：Rocky 2009-09-23 14:14:55
 * 返回：正确返回>=0值，出错返<0值；
 */
int Tag::init()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return OK;
}

/*
 * 功能：跟据传入的id从工厂中产生一个处理对象（调用具体的子类对象创建函数来实现）
 * 编写：Rocky 2009-09-23 14:17:54
 * 返回：正确返回>=0值，出错返<0值；
 */
Tag *Tag::New(const string &page/*=""*/, const string &tag/*=""*/)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    /* 取id对应的对象，然再使用此对象的DoNew()来创建这种类的对象
     */
    Tag * const obj = m_Factory[ page ][ tag ];
    if(NULL == obj)
    {
        LOG_DEBUG("m_Factory[%s][%s]=NULL", page.c_str(), tag.c_str());
        return NULL;
    }
    // 创建子类对象
    Tag *sub = obj->DoNew();
    if(NULL == sub)
    {
        return NULL;
    }
    sub->m_id = obj->m_id;
    return sub;
}


/*
 * 功能：各子类负责实现自已初始化操作（子类应重实现此接口）
 * 编写：Rocky 2010-04-09 15:15:58
 * 返回：正确返回>=0值，出错返<0值；
 */
int Tag::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("功能未实现");
    return ERR;
}

/*
 * 功能：各子类负责实现自已的对象创建器（子类应重实现此接口）
 * 编写：Rocky 2010-04-09 15:16:12
 * 返回：正确返回>=0值，出错返<0值；
 */
Tag *Tag::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("功能未实现");
    return NULL;
}






/*
 * ========================== 以下为业务处理代码 ==========================
 */




// 取标记对象的内容
string Tag::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    LOG_ERROR("功能未实现，应由子类实现。");
    return "";
}

// 设置文本（及类型）
void Tag::Set(const char *tag, Type type/*=TAG*/)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_tag = tag;
    m_type = type;
}










}// end of TAG_SPACE
