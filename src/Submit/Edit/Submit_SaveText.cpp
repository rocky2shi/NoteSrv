// Rocky 2010-05-15 16:34:16
#include "Page.h"
#include "UserData.h"
#include "Encrypt.h"
#include "Menu.h"
#include "MultiStringMatch.h"
#include "Submit_SaveText.h"
namespace SUBMIT_SAVETEXT_SPACE
{




// ����ģ��
static const string THIS_MODULE = "SaveText";





Submit_SaveText::Submit_SaveText()
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_SaveText::Submit_SaveText(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // �����켣����
}

Submit_SaveText::~Submit_SaveText()
{
    FUNCTION_TRACK(); // �����켣����
}




int Submit_SaveText::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Submit::DoInit();
}

// ������󴴽���
Submit *Submit_SaveText::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Submit_SaveText;
}





/******************************** ҵ����� ********************************/


// ��������
int Submit_SaveText::Save(Page *page)
{
    /*
     * ȡ������
     */
    Request * const &request = page->GetRequest();
    const string &key = page->GetCurrentKey();
    const string &title = WebEditToText( request->GetField("title") );

    if("" == title)
    {
        LOG_ERROR("title can't empty, key=[%s]", key.c_str());
        return ERR;
    }

    const Conf *pack = page->GetCurrentPack();      // ��ǰkey��Ӧ���ݼ�
    const string &text = WebEditToText( request->GetField("text") );  // ע�⣬ֻ����س����з���
    const string &modify = NowTime("%Y%m%d%H%M%S"); // �޸�ʱ��Ϊ��ǰʱ��
    const string &msg_type = request->GetField("msg_type");
    const string &password = request->GetField("password");

    /*
     * ���ݴ��
     */
    Ini data;
    // ���ܲ���
    if("" != password)
    {
        data.Set(key, "text", Encrypt(password).encrypt(text));
    }
    else
    {
        data.Set(key, "text", text);
    }
    data.Set(key, "title", title);
    data.Set(key, "modify", modify);
    data.Set(key, "msg_type", msg_type);

    // д��
    page->Save( data );

    // ���������ʾ˳��
    AdjustType(page, msg_type);

    return OK;
}

// �������ʹ���õĿ�ǰ��types:��ǰѡ�е����
int Submit_SaveText::AdjustType(Page *page, const string &types)
{
    const string &username = page->GetRequest()->GetCurrentUser();
    Menu *menu = Menu::Get(username, "TypeDropBox.cfg");    // [XXX:56425820]
    vector<string> items;
    vector<string>::iterator it;
    Split(types, ",", items); // �ָ20081004213824,20081004213826,20081004213827,20081004214214
    for(it = items.begin(); items.end() != it; it++)
    {
        const string &key = *it;
        // �����������õ�index�ֶ�Ϊ��ǰ�����ֵ��������һ˳��ֵ����ʹ֮��ǰ��
        menu->SetItem(key, "index", NowTime(""));
    }
    // ����
    return menu->Save();
}

// ��֤ͨ�������ص�["decryption"]Ϊ�մ�����ͨ���򷵻س�����Ϣ��
int Submit_SaveText::Deal(Page *page)
{
    int ret = Save(page);
    const string &key = page->GetCurrentKey();
    const string &operate = page->GetRequest()->GetField("operate");
    string err;
    string html;

    err = (OK == ret) ? "�ύ�ɹ���[" + key + "]"
                      : "����ʧ�ܣ���������Ҫ���µ�¼��";

    // ����ɹ��������½�ʱ������ҳ�棻
    if(OK == ret && "new" == operate)
    {
        // ���棬��ˢ��ҳ�棨�½���
        html = "<script>\n"
               "if(parent)\n"
               "{\n"
               "    parent.document.location.href = 'edit';\n"
               "}\n"
               "</script>\n"
               "\n";
    }
    else
    // ��ʾ��ǰ��Ϣ
    {
        // ���棬���ڵ�ǰҳ�棨��ˢ�£�
        html = "<script>\n"
               "var tmp = parent.document.getElementById('error');\n"
               "if(null != tmp)\n"
               "{\n"
               "    tmp.innerHTML = '" + err + "';\n"
               "    parent.ClearStatus();\n"
               "}\n"
               "</script>\n"
               "\n";
    }

    LOG_INFO("%s", err.c_str());

    // ����ҳ�����
    page->GetRequest()->GetConnect()->Send(html);
    return OK;
}













// ����Ϊȫ�ֱ�ǣ�
static Submit_SaveText tmp("edit", THIS_MODULE);

}// end of SUBMIT_SAVETEXT_SPACE

