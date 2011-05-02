#ifndef __CHINESECODING_H_20100514__
#define __CHINESECODING_H_20100514__
#include "Common.h"
#include <iconv.h>
namespace CHINESECODING_SPACE
{






// ���ı���ת������
class ChineseCoding
{
public:
    ChineseCoding(const string &from, const string &to);
    ~ChineseCoding();

    // ��str��from����תΪto���루�ɹ�����OK��
    int Converter(const string &from, string &to);

private:
    const string m_from;    // Դ����
    const string m_to;      // Ŀ�����
    iconv_t m_cd;           // ת��������
};










}// end of CHINESECODING_SPACE
using namespace CHINESECODING_SPACE;
#endif // end of __CHINESECODING_H_20100514__

