#ifndef __USERDATA_H_20100430__
#define __USERDATA_H_20100430__
#include "Common.h"
#include "Ini.h"
#include "GetFileList.h"
namespace USERDATA_SPACE
{






// ȡ�������û������ݣ��ı���������ͼƬ�ȣ�
class UserData
{
public:
    // ����ȡ����������
    class iterator
    {
    public:
        // ��ȡpath�������ļ��ڵ�����
        iterator(const string &path);

        // ָ����һ������
        bool next();

        // ȡ��ǰ[key/pack]��
        int Get(string &key, Conf *&pack);


    private:
        Ini m_Curent;   // ��ǰ���ڶ�ȡ���ļ�
        Ini::reverse_iterator m_itCurrent;
        GetFileList m_FilsList;
        GetFileList::file_iterator m_itFile;
        bool m_first;
    };

public:
    UserData(const string &username);
    ~UserData();

    /*
     * ��ȡ��������λ������
     */
    const string Get(const string &key, const string &item);  // ��������
    int Get(Ini &pack);    // �����ļ��У�ȡ���ݣ����pack��key��Ӧ��ֵ

    /*
     * д���������
     * ��ע������ɾ��ĳ���ݣ�ֻ������״̬Ϊ"delete"������force==true��
     *   ����Set()���ɣ��������ڲ�����
     */
    int Set(const string &key, const string &item, const string &value); // ��������
    int Set(const Ini &pack, bool force=false);    // һ������

    /*
     * ȡkey��Ӧ���ݵĸ������б�attachs��
     */
    int GetAttachList(const string &key, vector<string> &attachs) const;

    // ȡ�������
    const string GetResult(const string &field) const;

private:
    const string m_username;
    Conf m_result;  // �¼���������ȼ�¼
//    Cache m_cache;
//    File m_file;
};





}// end of USERDATA_SPACE
using namespace USERDATA_SPACE;
#endif // end of __USERDATA_H_20100430__
