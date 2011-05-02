#ifndef __DATAPACK_H_20100518__
#define __DATAPACK_H_20100518__
#include "UserData.h"
#include "Common.h"
namespace DATAPACK_SPACE
{





// ���ݴ�����û����ݡ������ȣ��Զ���ṹ��
class DataPack
{
    // У����
    static const unsigned int MAGIC = 0xCC57EE9D;
    // ��ǰ�汾�ţ����ڼ�¼��ţ�����ʹ���ĸ��汾������룩
    static const string CURRENT_VERSION; // = "20100519100025";

    // ���ڣ������ļ���������
    typedef enum{
        FT_DATA,            // �����ļ�
        FT_ATTACH,          // �����ļ�
        FT_MENU_CFG,        // �����������˵�����
        FT_PAGE_CFG,        // ��ҳ�棨���ã�����
    } FileType;

    // ��ǰ����������
    typedef enum{
        OPR_UNKNOWN,        // δȷ������������
        OPR_MAKEPACK,       // ���
        OPR_UNPACK,         // ���
        OPR_FINISH,         // ������������������
    } OprType;

    // ����ÿ���ļ���ͷ��
    struct Head
    {
        int type;               // �������ͣ������ļ��������ȣ������ϱ�FileType���壩
        char basename[256];     // �ļ���
        unsigned int filesize;  // ���ݲ��ֳ��ȣ��ֽ�����
        long mtime;             // ����޸�ʱ��
        char reserve[16];       // �����������Ժ����ڴ���ļ����Եȣ�
        unsigned int magic;

        Head()
        {
            memset(this, 0, sizeof(*this)); // ���ֶ�����
            magic = MAGIC;      // ����У��ֵ
        }
        // ͷ����Ч����true�����ʱ�ã�
        bool isValid()
        {
            return MAGIC == magic;
        }
    };

    // �汾��Ϣ
    struct Version
    {
        char key[32];
        Version()
        {
            // �Ե�ǰʱ�䴮Ϊ�汾���
            strcpy(key, CURRENT_VERSION.c_str());
        }
        // �汾ƥ�䷵��true
        bool isValid()
        {
            return CURRENT_VERSION == key;
        }
    };

public:
    // �������
    typedef enum
    {
        A_EXCLUDE_ATTACH,   // �ų��������򸽼��ϴ���ʱ��������
        //
        // �¼����Ե������
        //
        A_MAX
    } Attrib;


public:
    DataPack(const string &username);
    ~DataPack();

    /*
     * ========== ������� ==========
     */
    // ���ô����Ϣ
    int MakePack();

    // ������Ҫ������û����ݣ���������
    int Add(const Ini &pack);

    // ��������û�����
    int AddAll();

    // ���������˵�����ҳ�����õȣ�
    int OtherPack();

    // ��������������ذ��ļ�����
    const string PackFinish();

    // ���ã���������������
    void SetAttrib(Attrib type, bool valid=true);

    /*
     * ========== ������� ==========
     */
    // ���ý����Ϣ��pack: ����İ��ļ�����
    int UnPack(const string &pack);

    /*
     * ========== �������� ==========
     */
    // ȡ������
    const string GetResult(const string &field) const;

private:
    // ���һ���ļ�����filename�ļ�������У�
    int FilePack(const string &filelname, FileType type);

    // �ļ������
    int FileUnPack();

    // ����
    void Clear();


private:
    FileObj m_pack;          // ����ļ�
    FileObj m_ini;           // ��ʱ�û������ļ�
    UserData m_UserData;     // ��ǰ�������û�����
    const string m_username; // ��ǰ�������û���
    OprType m_opr;           // ��ǰ����ִ�еĲ�����1:��� 2:���
    bool m_attribs[A_MAX];   // ������Ա�־��
    Conf m_result;           // ��¼�������
};







}// end of DATAPACK_SPACE
using namespace DATAPACK_SPACE;
#endif // end of __DATAPACK_H_20100518__
