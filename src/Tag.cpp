// Rocky 2010-04-09 14:19:58
#include "Page.h"
#include "Tag.h"
namespace PAGE_SPACE
{










Tag::Factory Tag::m_Factory;


Tag::Tag()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag::Tag(const string &page, const string &tag)
{
    FUNCTION_TRACK(); // �����켣����
    Register(page, tag, this);
}

Tag::~Tag()
{
    FUNCTION_TRACK(); // �����켣����
}


/*
 * ���ܣ�ע��������󵽹���
 * ��д��Rocky 2009-09-07 17:09:46
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 * ע�⣺�˺����ڽ���main()����֮ǰ���ɹ��캯��������
 */
int Tag::Register(const string &page, const string &tag, Tag *sub)
{
    FUNCTION_TRACK(); // �����켣����
    if(NULL != m_Factory[ page ][ tag ])
    {
        printf("Register error, page=[%s], tag=[%s]\n", page.c_str(), tag.c_str()); // 2010-05-13 17:12:06
        assert(NULL == m_Factory[ page ][ tag ]);   // ��ʱ��λ��ӦΪ��
        return ERR;
    }
    m_Factory[ page ][ tag ]  = sub;  // �������
    m_id = tag;
    return OK;
}

/*
 * ���ܣ���̬��ʼ������
 * ��д��Rocky 2009-09-23 14:14:55
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
int Tag::init()
{
    FUNCTION_TRACK(); // �����켣����
    return OK;
}

/*
 * ���ܣ����ݴ����id�ӹ����в���һ��������󣨵��þ����������󴴽�������ʵ�֣�
 * ��д��Rocky 2009-09-23 14:17:54
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
Tag *Tag::New(const string &page/*=""*/, const string &tag/*=""*/)
{
    FUNCTION_TRACK(); // �����켣����

    /* ȡid��Ӧ�Ķ���Ȼ��ʹ�ô˶����DoNew()������������Ķ���
     */
    Tag * const obj = m_Factory[ page ][ tag ];
    if(NULL == obj)
    {
        LOG_DEBUG("m_Factory[%s][%s]=NULL", page.c_str(), tag.c_str());
        return NULL;
    }
    // �����������
    Tag *sub = obj->DoNew();
    if(NULL == sub)
    {
        return NULL;
    }
    sub->m_id = obj->m_id;
    return sub;
}


/*
 * ���ܣ������ฺ��ʵ�����ѳ�ʼ������������Ӧ��ʵ�ִ˽ӿڣ�
 * ��д��Rocky 2010-04-09 15:15:58
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
int Tag::DoInit()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("����δʵ��");
    return ERR;
}

/*
 * ���ܣ������ฺ��ʵ�����ѵĶ��󴴽���������Ӧ��ʵ�ִ˽ӿڣ�
 * ��д��Rocky 2010-04-09 15:16:12
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
Tag *Tag::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("����δʵ��");
    return NULL;
}






/*
 * ========================== ����Ϊҵ������� ==========================
 */




// ȡ��Ƕ��������
string Tag::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("����δʵ�֣�Ӧ������ʵ�֡�");
    return "";
}

// �����ı��������ͣ�
void Tag::Set(const char *tag, Type type/*=TAG*/)
{
    FUNCTION_TRACK(); // �����켣����
    m_tag = tag;
    m_type = type;
}










}// end of TAG_SPACE
