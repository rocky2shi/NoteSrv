#ifndef __TAG_H_20100409__
#define __TAG_H_20100409__
#include "Common.h"
namespace PAGE_SPACE
{


class Page;





// 标记处理基类
class Tag
{
/*
 *      ======================= 业务接口代码 =======================
 */
public:
    // tag类型定义：1. 当是TXT，直接输出m_tag； 2. 当是TAG时，则输出m_tag对应的数据；
    typedef enum{TXT, TAG} Type;

    // 取标记对象的内容
    virtual string Get(Page *page) = 0;

    // 设置文本（及类型）
    virtual void Set(const char *tag, Type type=TAG);


protected:
    string m_id;    // 本模块id
    string m_tag;   // 对应的处理标记
    Type m_type;    // 从属于哪个页（或全局有效）







/*
 *      ========== 以下代码主要用于工厂处理设置（模板代码）=========
 */
public:
    virtual ~Tag();

    // 初始化
    static int init();

    // 跟据传入的id[page,tag]从工厂中产生一个处理对象
    static Tag *New(const string &page="", const string &tag="");

protected:
    Tag();
    Tag(const string &page, const string &tag); // 子类调用来进行初次注册

    // 注册（把子类对象加入m_Factory）
    int Register(const string &page, const string &tag, Tag *sub);

    // 各子类负责实现自已初始化操作（子类应重实现此接口）
    virtual int DoInit() = 0;

    // 各子类负责实现自已的对象创建器（子类应重实现此接口）
    virtual Tag *DoNew() = 0;

private:
    // ［页面名，页内元素标记] => 处理对象
    typedef map< string, map<string, Tag *> > Factory;

    /*
     * 在进入main()函数之前，各对象已把自已插入到m_TmpFactory，而后再
     * 在main()中调用init()时，根据彩种id，把对象插入到m_Factory，实际
     * 的操作用到的将是m_Factory；
     */
    static Factory m_Factory;   // 初始化后填充的产生子对象工厂
};








}// end of PAGE_SPACE
using namespace PAGE_SPACE;
#endif // end of __TAG_H_20100409__

