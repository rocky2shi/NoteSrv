// Rocky 2010-05-11 21:42:15
#include "Page.h"
#include "Submit.h"
namespace PAGE_SPACE
{



// ע�ᣨ������������m_queue��
int Submit::Manage::Register(const string &page, const string &element, Submit *sub)
{
    FUNCTION_TRACK(); // �����켣����
    assert(NULL == m_queue[ page ][ element ]);   // ��ʱ��λ��ӦΪ��
    m_queue[ page ][ element ] = sub;  // �������
    sub->m_page = page;
    sub->m_element = element;
    // ��ʱ��־ģ�黹������ [XXX]
    //printf("Register: page=[%s], element=[%s], obj=[%p]... [OK]\n",
    //                                page.c_str(), element.c_str(), sub);
    return OK;
}


// ȡ����Ԫ��
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



// ���ʼ�����ĳ�������ʱִ��һ�Σ�
int Submit::init()
{
    static Manage factory;

    // // ע�ᵽȫ�ֻ�����¼   // [XXX:8982516]
    // Manage *obj = instance( &factory );
    // if(NULL == obj)
    // {
    //     LOG_ERROR("Submit::instance() error.");
    //     return ERR;
    // }

    return OK;
}

// ���ݴ����id[page,tag]�ӹ����в���һ���������
Submit *Submit::New(const string &page, const string &element)
{
    // �ڹ������Ҵ��������������������
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
    // ����ֵ���Ӷ���
    sub->m_page = parent->m_page;
    sub->m_element = parent->m_element;
    return sub;
}


// �����ฺ��ʵ�����ѳ�ʼ������������Ӧ��ʵ�ִ˽ӿڣ�
int Submit::DoInit()
{
    LOG_ERROR("Must implement on child class");
    return ERR;
}

// �����ฺ��ʵ�����ѵĶ��󴴽���������Ӧ��ʵ�ִ˽ӿڣ�
Submit *Submit::DoNew()
{
    LOG_ERROR("Must implement on child class");
    return NULL;
}






/*
 * ========================== ����Ϊҵ������� ==========================
 */


// ��������
int Submit::Deal(Page *page)
{
    return ERR;
}







}// end of PAGE_SPACE

