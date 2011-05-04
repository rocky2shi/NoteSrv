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
    //申请一个转换描述符
    m_cd = iconv_open(m_to.c_str(), m_from.c_str()); // 注意，这里的参数是to在前!
}

ChineseCoding::~ChineseCoding()
{
    if(m_cd != 0)
    {
        // 关闭
        iconv_close(m_cd);
    }
}


// 把str从from编码转为to编码（from和to可为同一字符串）
int ChineseCoding::Converter(const string &from, string &to)
{
    if(m_cd == 0)
    {
        LOG_ERROR("iconv_open fail");
        // 出错时，原值不变；
        return ERR;
    }

    const int nToLen = from.length() * 2; // 两部长度应该能存放换结果不
    char fixbuf[512*1024] = "";    // 存放转换后的结果
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

    // 转换
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

