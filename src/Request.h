#ifndef __REQUEST_H_20100418__
#define __REQUEST_H_20100418__
#include "Common.h"
#include "Connect.h"
#include "FileObj.h"
namespace REQUEST_SPACE
{




// �ͻ��˷���������������
class Request
{
public:
    Request(Connect *connect);
    ~Request();

    // ȡ����ͷ���ֶζ�Ӧֵ�������ִ�Сд��
    const string GetHead(const string &field) const;

    // һ���ֶζ�Ӧֵ�������ִ�Сд��
    const string GetField(const string &field) const;

    // ȡ�ֶζ�Ӧ�ļ�����
    FileObj *GetFileObj(const string &field);

    // ��������ҳ����
    const string GetPageName();

    // ȡ����
    Connect *GetConnect();

    // �Ƿ�ΪͼƬ��jpg, jpeg, png, gif, ...
    bool isImage();

    // �Ƿ�Ϊ�ı��ļ���txt, js, css, html, htm, ...
    bool isText();

    // �Ƿ�Ϊcgi����ҳ��
    bool isCgi();

    // �������Ƿ���ȷ����
    bool ok();

    // ȡ��ǰ��¼�û���
    const string GetCurrentUser() const;

    // ȡCookie�е�ֵ
    const string GetCookie(const string &key) const;


private:
    void Parse();       // ��������
    void ParseHead();   // ��������ͷ��
    void ParseAttach(); // �����������
    void ParsePost();   // ����POST�����
    void ParseGet(char *str);        // ����GET�����
    void ParseUrlParam(char *param); // ����url������
    void ParseCookie();              // ����Cookie��

    int GetLine(char *buf, unsigned int max);   // ȡһ��

private:
    string m_page;      // ҳ����
    string m_boundary;  // post����ʱ���õķָ���
    string m_type;      // ��������ͣ�Ŀǰ�������������
    Connect * const m_connect;          // �ͻ�������
    string m_PageType;                  // ҳ������׺
    map<string, string> m_HeadFieldAry; // ����ͷ�ֶ��б�
    map<string, string> m_FieldAry;     // �ֶ��б�
    map<string, FileObj*> m_FileObjAry; // �������ļ��б�
    map<string, string> m_Cookie;       // Cookie�е�[��/ֵ]��
    string m_ErrMsg;                    // ��Ŵ�����Ϣ������ȫ��ȷʱ����ֵӦΪ�գ�
};







}// end of REQUEST_SPACE
using namespace REQUEST_SPACE;
#endif // end of __REQUEST_H_20100418__
