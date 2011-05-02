#ifndef __TAG_H_20100409__
#define __TAG_H_20100409__
#include "Common.h"
namespace PAGE_SPACE
{


class Page;





// ��Ǵ������
class Tag
{
/*
 *      ======================= ҵ��ӿڴ��� =======================
 */
public:
    // tag���Ͷ��壺1. ����TXT��ֱ�����m_tag�� 2. ����TAGʱ�������m_tag��Ӧ�����ݣ�
    typedef enum{TXT, TAG} Type;

    // ȡ��Ƕ��������
    virtual string Get(Page *page) = 0;

    // �����ı��������ͣ�
    virtual void Set(const char *tag, Type type=TAG);


protected:
    string m_id;    // ��ģ��id
    string m_tag;   // ��Ӧ�Ĵ�����
    Type m_type;    // �������ĸ�ҳ����ȫ����Ч��







/*
 *      ========== ���´�����Ҫ���ڹ����������ã�ģ����룩=========
 */
public:
    virtual ~Tag();

    // ��ʼ��
    static int init();

    // ���ݴ����id[page,tag]�ӹ����в���һ���������
    static Tag *New(const string &page="", const string &tag="");

protected:
    Tag();
    Tag(const string &page, const string &tag); // ������������г���ע��

    // ע�ᣨ������������m_Factory��
    int Register(const string &page, const string &tag, Tag *sub);

    // �����ฺ��ʵ�����ѳ�ʼ������������Ӧ��ʵ�ִ˽ӿڣ�
    virtual int DoInit() = 0;

    // �����ฺ��ʵ�����ѵĶ��󴴽���������Ӧ��ʵ�ִ˽ӿڣ�
    virtual Tag *DoNew() = 0;

private:
    // ��ҳ������ҳ��Ԫ�ر��] => �������
    typedef map< string, map<string, Tag *> > Factory;

    /*
     * �ڽ���main()����֮ǰ���������Ѱ����Ѳ��뵽m_TmpFactory��������
     * ��main()�е���init()ʱ�����ݲ���id���Ѷ�����뵽m_Factory��ʵ��
     * �Ĳ����õ��Ľ���m_Factory��
     */
    static Factory m_Factory;   // ��ʼ�������Ĳ����Ӷ��󹤳�
};








}// end of PAGE_SPACE
using namespace PAGE_SPACE;
#endif // end of __TAG_H_20100409__

