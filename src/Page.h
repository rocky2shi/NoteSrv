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






// cgiҳ�洦�����
class Page
{
/*
 *      ======================= ҵ��ӿڴ��� =======================
 */
public:
    // ����ҳ������
    virtual int Deal();

    // ��������
    void SetRequest(Request * request);

    // ȡ��ҳ�浱ǰ���õ��������ӣ�
    Request *GetRequest();

    // ��ģʽ���õ�ȡʵ���ӿ�
    inline static Page *instance(Page *page=NULL)
    {
        static Page *obj = (Page *)(Environment::instance()->ClassInit("Page", page));
        return obj;
    }

    // ���á���ȡtag�䴦��������
    const string GetResult(const string &key);
    void SetResult(const string &key, const string &value);

    // ȡ��ǰ��KEY
    const string GetCurrentKey();

    // ȡ��ǰKEY��Ӧ�����ݣ�����
    Conf *GetCurrentPack();

    // ȡ��ǰҳ������
    PageCfg *GetCurrentPageCfg();

    // ͳһ�����棨������ͬ����������£�
    int Save(const Ini &pack);

protected:
    // ���httpЭ��ͷ��
    virtual int OutHead();

    // ���������
    virtual int OutBody();

    // ҳ����ת
    void ChangeTo(const string &url);


private:

    // ����ҳ��ģ��
    int Parse();


protected:
    string m_id;                    // ģ��id
    string m_html;                  // ҳ���ļ���������ָ����
    Request * m_request;            // �ͻ���������Ϣ���ⲿ�Ѿ�������
    vector< Tag * > *m_TagList;     // ҳ�����б���б�ע�⣬m_list��ָ�룩��ͬ�������
    Counter *m_VisitCounter;        // ҳ����ʼ�����
    bool m_bParent;                 // �������ǣ������Ӷ����ɸ�����new������
    map<string, string> m_result;   // ҳ�������䴫��Ϣ
    string m_key;                   // ÿ�����ݶ���Ӧ��һ��KEY
    Conf *m_pack;                   // ��ǰKEY��Ӧ�����ݣ�����Ϊ�����ζ��ļ��ã�
    PageCfg *m_PageCfg;             // ��ǰҳ������





/*
 *      ========== ���´�����Ҫ���ڹ����������ã�ģ����룩=========
 */
public:
    Page();
    virtual ~Page();

    // ��ʼ��
    static int init();


    // ���ݴ����id[page,tag]�ӹ����в���һ���������
    static Page *New(const string &page);

    // ע�ᣨ������������m_Factory��
    int Register(const string &page, Page *sub);

    // ע��
    int UnRegister(const string &page);

protected:

    // �����ฺ��ʵ�����ѳ�ʼ������������Ӧ��ʵ�ִ˽ӿڣ�
    virtual int DoInit();

    // �����ฺ��ʵ�����ѵĶ��󴴽���������Ӧ��ʵ�ִ˽ӿڣ�
    virtual Page *DoNew();

private:
    typedef map< string, Page * > Factory;

    // ������󹤳�
    Factory m_Factory;



public:
    // ��������ָ������
    typedef AutoPtr<Page> Ptr;
};









/*
 * ����Page_*ģ����Զ����أ�����ģ��رպ��Զ�ע����
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
 * ���ﶨ�����ִ���ʽ��
 *   1. ��̬�⴦��ʽ
 *   2. ֱ��Ƕ�����ʽ��Ĭ�ϣ�������ʹ�ö�̬�����������Ĭ�ϴ�����룻
 */

#if 0
// ��ʱȡ���˴���ʽ��ע���Makefile���Ӧ��[Rocky 2010-06-04 17:13:44] [XXX]
// ���嶯̬�����
#define DefinitinoDllEnter(Type, id)                        \
extern "C" void DllEnter(void *param)                       \
{                                                           \
    Environment::instance( (Environment*)param );           \
                                                            \
    /* ����һ��̬�����Ա��ڹ���ʱע�ᣬ����ʱע����*/     \
    static AutoLoadPageDll< Type > tmp( id );               \
}
#endif

/* �ɳ�ʼ����ClassInit���õĽӿ� */
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
