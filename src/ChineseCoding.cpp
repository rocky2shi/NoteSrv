// Rocky 2010-05-14 22:56:44
#include "Malloc.h"
#include "ChineseCoding.h"
#include <iconv.h>
namespace CHINESECODING_SPACE
{


ChineseCoding::ChineseCoding(const string &from, const string &to)
                 : m_from(from),
                   m_to(to)
{
    //����һ��ת��������
    m_cd = iconv_open(m_to.c_str(), m_from.c_str()); // ע�⣬����Ĳ�����to��ǰ!
}

ChineseCoding::~ChineseCoding()
{
    if(m_cd != 0)
    {
        // �ر�
        iconv_close(m_cd);
    }
}


// ��str��from����תΪto���루from��to��Ϊͬһ�ַ�����
int ChineseCoding::Converter(const string &from, string &to)
{
    if(m_cd == 0)
    {
        LOG_ERROR("iconv_open fail");
        // ����ʱ��ԭֵ���䣻
        return ERR;
    }

    const int nToLen = from.length() * 2; // ��������Ӧ���ܴ�Ż������
    char fixbuf[512*1024] = "";    // ���ת����Ľ��
    Malloc buf(fixbuf, sizeof(fixbuf), nToLen + 1);
    char *pBuf = buf.Get();

    if(NULL == pBuf)
    {
        LOG_ERROR("new char[ %u ] error", nToLen + 1);
        return ERR;
    }

    const char *pfrom = from.c_str();
    const char *pto = pBuf;
    char **ppfrom = (char **)&pfrom;
    char **ppto = (char **)&pto;

    size_t tolen = nToLen;
    size_t fromlen = from.length();

    // ת��
    if(iconv(m_cd, ppfrom, &fromlen, ppto, &tolen) == (size_t)(-1))
    {
        LOG_DEBUG("from=[%s] to=[%s]", m_from.c_str(), m_to.c_str());
        LOG_ERROR("iconv fail: [%s]", strerror(errno));
        return ERR;
    }

    to = pBuf;

    return OK;
}









}// end of CHINESECODING_SPACE

