// Rocky 2010-05-03 02:36:14
#include "UserData.h"
#include "Encrypt.h"
#include "Page_Reading.h"
namespace PAGE_READING_SPACE
{











// 标明模块
static const string THIS_MODULE = "reading";



Page_Reading::Page_Reading()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Reading::~Page_Reading()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Reading::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    m_html = GlobalConfig::instance()->TemplateDir() + "Reading.html";
    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Reading::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Reading;
}








/******************************** 业务代码 ********************************/

int Page_Reading::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);


    /*
     * 处理正文显示风格设置提交事件
     */
    const string &set_text_style = m_request->GetField("set_text_style");
    if("" != set_text_style)
    {
        Submit::Ptr submit( Submit::New("reading", "TextStyleSetting") );
        submit->Deal(this);
        return OK;
    }
    LOG_DEBUG("set_text_style=[%s]", set_text_style.c_str());

    /*
     * 处理正文中插入图片、书签等提交事件
     */
    const string &operate = m_request->GetField("operate");
    if("InsertObj" == operate)
    {
        Submit::Ptr submit( Submit::New("reading", "TextInsertImage") );
        if(submit->Deal(this) < 0)
        {
            m_request->GetConnect()->Send("[ERROR]");
            return OK;
        }
        m_request->GetConnect()->Send("[OK]");
        return OK;
    }
    LOG_DEBUG("operate=[%s]", operate.c_str());

    /*
     * 是否为解密提交事件
     */
    const string &decryption = m_request->GetField("decryption");
    if("" != decryption)
    {
        Submit::Ptr submit( Submit::New(SUBMIT_GLOBAL, "decryption") );
        /*
         * 认证未通过，则submit->Deal()处理内部设置encryption为"YES"，
         * 正文显示等代码根据此值决定是否显示内容。
         */
        submit->Deal(this);
    }

    /*
     * 处理数据转换（新、旧数据间）提交事件
     */
    const string &conversion = m_request->GetField("DataConversion");
    if("" != conversion)
    {
        Submit::Ptr submit( Submit::New("reading", "DataConversion") );
        if(submit->Deal(this) < 0)
        {
            m_request->GetConnect()->Send("ERROR");
        }
        else
        {
            m_request->GetConnect()->Send("OK");
        }
        return OK;
    }

    return Page::OutBody();
}







// 定义动态库入口
DefinitinoDllEnter(Page_Reading, THIS_MODULE)

}// end of PAGE_READING_SPACE

