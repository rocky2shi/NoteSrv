// Rocky 2010-05-02 01:12:07
#include "Page.h"
#include "Tag_ClientIp.h"
namespace TAG_CLIENTIP_SPACE
{




// ����ģ��
static const string THIS_MODULE = "ClientIp";





Tag_ClientIp::Tag_ClientIp()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_ClientIp::Tag_ClientIp(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_ClientIp::~Tag_ClientIp()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_ClientIp::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_ClientIp::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_ClientIp;
}








/******************************** ҵ����� ********************************/





// ȡ��Ƕ��������
string Tag_ClientIp::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    Connect * const connect = page->GetRequest()->GetConnect();
    return connect->GetPeerAddr(); // ȡ���ͻ�������ip
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_ClientIp tmp(TAG_GLOBAL, THIS_MODULE);

}// end of TAG_CLIENTIP_SPACE

