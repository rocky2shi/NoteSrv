#ifndef __CHINESECODING_H_20100514__
#define __CHINESECODING_H_20100514__
#include "Common.h"
#include <iconv.h>
namespace CHINESECODING_SPACE
{






// 中文编码转换处理
class ChineseCoding
{
public:
    ChineseCoding(const string &from, const string &to);
    ~ChineseCoding();

    // 把str从from编码转为to编码（成功返回OK）
    int Converter(const string &from, string &to);

    // str是code编码时返true
    static bool TestCoding(const char *str, const string code);

    // 较安全的编码转换（可以转换时，直接修改str）
    static int GB18030ToUTF8(string &str);

private:
    const string m_from;    // 源编码
    const string m_to;      // 目标编码
    iconv_t m_cd;           // 转换描述符
};










}// end of CHINESECODING_SPACE
using namespace CHINESECODING_SPACE;
#endif // end of __CHINESECODING_H_20100514__

