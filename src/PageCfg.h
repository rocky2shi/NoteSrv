#ifndef __PAGECFG_H_20100518__
#define __PAGECFG_H_20100518__
#include "Ini.h"
#include "Common.h"
namespace PAGECFG_SPACE
{





// ��ǰҳ�����ƣ����ݣ�������
class PageCfg
{
public:
    // ָ���û����������ļ�
    PageCfg(const string &username, const string &cfg);

    // ȡĳ������ֵ
    const string Get(const string &key, const string &item) const;

    // ����ĳ������ֵ
    const int Set(const string &key, const string &item, const string &value);

    // ���棨д���ļ���
    int Save();

public:
    Ini m_data; // ��ǰҳ��������
    string m_fullname; // �����ļ�ȫ·��
};








}// end of PAGECFG_SPACE
using namespace PAGECFG_SPACE;
#endif // end of __PAGECFG_H_20100518__
