#ifndef __SUBMIT_H_20100511__
#define __SUBMIT_H_20100511__
#include "Common.h"
namespace PAGE_SPACE
{


class Page;




// 处理提交事件基类
class Submit
{
/*
 *      ======================= 业务接口代码 =======================
 */
public:
    // 处理请求
    virtual int Deal(Page *page);


protected:
    string m_page;                  // 所属页面名
    string m_element;               // 元素标记





/*
 *      ========== 以下代码主要用于工厂处理设置（模板代码）=========
 */
private:
    class Manage
    {
    public:
        // 注册（把子类对象加入m_Factory）
        int Register(const string &page, const string &element, Submit *sub);

        // 取处理元素
        Submit *Get(const string &page, const string &element);

    private:
        // ［页面名，提交元素字段名] => 处理对象
        typedef map< string, map<string, Submit *> > Factory;

        // 子类对象工厂
        Factory m_queue;
    };

public:
    virtual ~Submit();

    // 类初始化（的程序启动时执行一次）
    static int init();

    // 跟据传入的id[page,element]从工厂中产生一个处理对象
    static Submit *New(const string &page, const string &element);

protected:
    Submit();
    Submit(const string &page, const string &element);

    // 单模式中用的取实例接口
    inline static Manage *instance(Manage *executor=NULL)
    {
        // static Manage *obj = (Manage *)(Environment::instance()->ClassInit("Submit::Manage", executor));
        // return obj;
        static Manage factory;  // [XXX:8982516]
        return &factory;
    }

    // 各子类负责实现自已初始化操作（子类应重实现此接口）
    virtual int DoInit();

    // 各子类负责实现自已的对象创建器（子类应重实现此接口）
    virtual Submit *DoNew();

public:
    // 定义智能指针类型
    typedef AutoPtr<Submit> Ptr;
};







}// end of PAGE_SPACE
using namespace PAGE_SPACE;
#endif // end of __SUBMIT_H_20100511__

