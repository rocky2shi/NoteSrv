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

    const int nToLen = from.length() * 2; // 两倍长度应该能存放换结果不
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

// str是code编码时返true
bool ChineseCoding::TestCoding(const char *str, const string code)
{
    //申请一个转换描述符，注意，这里的参数是to在前!
    iconv_t cd = iconv_open(code.c_str(), code.c_str());
    if(cd == 0)
    {
        LOG_ERROR("iconv_open fail");
        return false;
    }

    const int TO_MAX = 1024 * 1024;
    static char buf[TO_MAX];  // 不需要结果，只用于中间处理；
    const char *pfrom = str;
    const char *pto = buf;
    char **ppfrom = (char **)&pfrom;
    char **ppto = (char **)&pto;
    size_t tolen = TO_MAX;
    size_t fromlen = strlen(str);

    // 能功成转换则为已知编码
    int ret = iconv(cd, ppfrom, &fromlen, ppto, &tolen);

    iconv_close(cd);

    return (ret != (size_t)(-1)) ? true : false;
}

// 较安全的编码转换（可以转换时，直接修改str）
int ChineseCoding::GB18030ToUTF8(string &str)
{
    // [Rocky 2011-05-08 14:56:17]
    // 不是utf-8，且是gb18030的才转换；
    if( ! ChineseCoding::TestCoding(str.c_str(), "utf-8")
            && ChineseCoding::TestCoding(str.c_str(), "gb18030")
      )
    {
        int ret = ChineseCoding("gb18030",
                "utf-8").Converter(str, str);
        if(ret < 0)
        {
            LOG_ERROR("gb18030 to utf-8 error");
            return ret;
        }
    }
    return OK;
}






}// end of CHINESECODING_SPACE

