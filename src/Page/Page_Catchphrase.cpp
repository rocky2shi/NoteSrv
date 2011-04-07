// Rocky 2010-04-28 00:40:08
#include "Page_Catchphrase.h"
namespace PAGE_CATCHPHRASE_SPACE
{





// 标明模块
static const string THIS_MODULE = "catchphrase";


vector<string> Page_Catchphrase::m_sentence;






Page_Catchphrase::Page_Catchphrase()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Page_Catchphrase::~Page_Catchphrase()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}


int Page_Catchphrase::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    const string filename = GlobalConfig::instance()->SystemDataDir() + "catchphrase.txt";

    FileObj list(filename);
    char buf[512];

    while( list.GetLine(buf, sizeof(buf)) > 0 )
    {
        m_sentence.push_back( buf );
    }

    LOG_INFO("get catchphrase number: %d", m_sentence.size());

    return Page::DoInit();
}

// 子类对象创建器
Page *Page_Catchphrase::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Page_Catchphrase;
}








/******************************** 业务代码 ********************************/




int Page_Catchphrase::OutBody()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    assert(NULL != m_request);

    const int num = GetRandomInt(0, m_sentence.size() - 1); // 取一随机数
    const string &sentence = m_sentence[ num ];

    m_request->GetConnect()->Send(sentence.c_str(), sentence.length());

    return OK;
}











// 定义动态库入口
DefinitinoDllEnter(Page_Catchphrase, THIS_MODULE)

}// end of PAGE_CATCHPHRASE_SPACE

