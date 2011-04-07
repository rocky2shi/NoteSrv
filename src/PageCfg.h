#ifndef __PAGECFG_H_20100518__
#define __PAGECFG_H_20100518__
#include "Ini.h"
#include "Common.h"
namespace PAGECFG_SPACE
{





// 当前页面配制（数据）处理类
class PageCfg
{
public:
    // 指定用户名、配置文件
    PageCfg(const string &username, const string &cfg);

    // 取某项配制值
    const string Get(const string &key, const string &item) const;

    // 设置某项配制值
    const int Set(const string &key, const string &item, const string &value);

    // 保存（写入文件）
    int Save();

public:
    Ini m_data; // 当前页配置数据
    string m_fullname; // 配置文件全路径
};








}// end of PAGECFG_SPACE
using namespace PAGECFG_SPACE;
#endif // end of __PAGECFG_H_20100518__
