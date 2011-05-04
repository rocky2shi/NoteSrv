#ifndef __PAGE_H_20100417__
#define __PAGE_H_20100417__
#include "Counter.h"
#include "Request.h"
#include "Tag.h"
#include "Conf.h"
#include "Ini.h"
#include "Submit.h"
#include "PageCfg.h"
#include "Environment.h"
#include "Common.h"
namespace PAGE_SPACE
{






// cgi页面处理基类
class Page
{
/*
 *      ======================= 业务接口代码 =======================
 */
public:
    // 处理页求请求
    virtual int Deal();

    // 设置连接
    void SetRequest(Request * request);

    // 取出页面当前所用的请求（连接）
    Request *GetRequest();

    // 单模式中用的取实例接口
    inline static Page *instance(Page *page=NULL)
    {
        static Page *obj = (Page *)(Environment::instance()->ClassInit("Page", page));
        return obj;
    }

    // 设置、获取tag间处理结果传递
    const string GetResult(const string &key);
    void SetResult(const string &key, const string &value);

    // 取当前的KEY
    const string GetCurrentKey();

    // 取当前KEY对应的数据（包）
    Conf *GetCurrentPack();

    // 取当前页面设置
    PageCfg *GetCurrentPageCfg();

    // 统一处理保存（出便于同步处理缓存更新）
    int Save(const Ini &pack);

protected:
    // 输出http协议头部
    virtual int OutHead();

    // 输出数据体
    virtual int OutBody();

    // 页面跳转
    void ChangeTo(const string &url);


private:

    // 解析页面模板
    int Parse();


protected:
    string m_id;                    // 模块id
    string m_html;                  // 页面文件（由子类指定）
    Request * m_request;            // 客户端请求信息（外部已经解析）
    vector< Tag * > *m_TagList;     // 页面所有标记列表（注意，m_list是指针）（同类对象共享）
    Counter *m_VisitCounter;        // 页面访问计数器
    bool m_bParent;                 // 主对象标记（所有子对象都由父对象new出来）
    map<string, string> m_result;   // 页处理结果间传信息
    string m_key;                   // 每条数据都对应有一个KEY
    Conf *m_pack;                   // 当前KEY对应的数据（用于为避免多次读文件用）
    PageCfg *m_PageCfg;             // 当前页面设置





/*
 *      ========== 以下代码主要用于工厂处理设置（模板代码）=========
 */
public:
    Page();
    virtual ~Page();

    // 初始化
    static int init();


    // 跟据传入的id[page,tag]从工厂中产生一个处理对象
    static Page *New(const string &page);

    // 注册（把子类对象加入m_Factory）
    int Register(const string &page, Page *sub);

    // 注销
    int UnRegister(const string &page);

protected:

    // 各子类负责实现自已初始化操作（子类应重实现此接口）
    virtual int DoInit();

    // 各子类负责实现自已的对象创建器（子类应重实现此接口）
    virtual Page *DoNew();

private:
    typedef map< string, Page * > Factory;

    // 子类对象工厂
    Factory m_Factory;



public:
    // 定义智能指针类型
    typedef AutoPtr<Page> Ptr;
};









/*
 * 用于Page_*模块的自动加载，并在模块关闭后自动注销；
 * [Rocky 2010-04-27 09:55:40]
 */
template<typename Type>
class AutoLoadPageDll
{
public:
    AutoLoadPageDll(const string &id) : m_id(id)
    {
        static Type tmp;
        Page::instance()->Register(m_id, &tmp);
    }
    ~AutoLoadPageDll()
    {
        Page::instance()->UnRegister(m_id);
    }

private:
    const string &m_id;
};



/*
 * 这里定义两种处理方式：
 *   1. 动态库处理方式
 *   2. 直接嵌入程序方式（默认），可以使用动态库加载来覆盖默认处理代码；
 */

#if 0
// 暂时取消此处理方式（注意和Makefile相对应）[Rocky 2010-06-04 17:13:44] [XXX]
// 定义动态库入口
#define DefinitinoDllEnter(Type, id)                        \
extern "C" void DllEnter(void *param)                       \
{                                                           \
    Environment::instance( (Environment*)param );           \
                                                            \
    /* 定义一静态对象，以便在构造时注册，析构时注销；*/     \
    static AutoLoadPageDll< Type > tmp( id );               \
}
#endif

/* 由初始化类ClassInit调用的接口 */
#define DefinitinoDllEnter(Type, id)                        \
static int ModuleInit()                                     \
{                                                           \
    static Type tmp;                                        \
    Page::instance()->Register(id, &tmp);                   \
}                                                           \
static ClassInit tmp(ModuleInit);









}// end of PAGE_SPACE
using namespace PAGE_SPACE;
#endif // end of __PAGE_H_20100417__
