#ifndef __SUBMIT_H_20100511__
#define __SUBMIT_H_20100511__
#include "Common.h"
namespace PAGE_SPACE
{


class Page;




// �����ύ�¼�����
class Submit
{
/*
 *      ======================= ҵ��ӿڴ��� =======================
 */
public:
    // ��������
    virtual int Deal(Page *page);


protected:
    string m_page;                  // ����ҳ����
    string m_element;               // Ԫ�ر��





/*
 *      ========== ���´�����Ҫ���ڹ����������ã�ģ����룩=========
 */
private:
    class Manage
    {
    public:
        // ע�ᣨ������������m_Factory��
        int Register(const string &page, const string &element, Submit *sub);

        // ȡ����Ԫ��
        Submit *Get(const string &page, const string &element);

    private:
        // ��ҳ�������ύԪ���ֶ���] => �������
        typedef map< string, map<string, Submit *> > Factory;

        // ������󹤳�
        Factory m_queue;
    };

public:
    virtual ~Submit();

    // ���ʼ�����ĳ�������ʱִ��һ�Σ�
    static int init();

    // ���ݴ����id[page,element]�ӹ����в���һ���������
    static Submit *New(const string &page, const string &element);

protected:
    Submit();
    Submit(const string &page, const string &element);

    // ��ģʽ���õ�ȡʵ���ӿ�
    inline static Manage *instance(Manage *executor=NULL)
    {
        // static Manage *obj = (Manage *)(Environment::instance()->ClassInit("Submit::Manage", executor));
        // return obj;
        static Manage factory;  // [XXX:8982516]
        return &factory;
    }

    // �����ฺ��ʵ�����ѳ�ʼ������������Ӧ��ʵ�ִ˽ӿڣ�
    virtual int DoInit();

    // �����ฺ��ʵ�����ѵĶ��󴴽���������Ӧ��ʵ�ִ˽ӿڣ�
    virtual Submit *DoNew();

public:
    // ��������ָ������
    typedef AutoPtr<Submit> Ptr;
};







}// end of PAGE_SPACE
using namespace PAGE_SPACE;
#endif // end of __SUBMIT_H_20100511__

