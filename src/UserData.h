#ifndef __USERDATA_H_20100430__
#define __USERDATA_H_20100430__
#include "Common.h"
#include "Ini.h"
#include "GetFileList.h"
namespace USERDATA_SPACE
{






// 取出给定用户的数据（文本、附件、图片等）
class UserData
{
public:
    // 遍历取出所有数据
    class iterator
    {
    public:
        // 读取path下所有文件内的数据
        iterator(const string &path);

        // 指向下一组数据
        bool next();

        // 取当前[key/pack]对
        int Get(string &key, Conf *&pack);


    private:
        Ini m_Curent;   // 当前正在读取的文件
        Ini::reverse_iterator m_itCurrent;
        GetFileList m_FilsList;
        GetFileList::file_iterator m_itFile;
        bool m_first;
    };

public:
    UserData(const string &username);
    ~UserData();

    /*
     * 读取给定具体位置数据
     */
    const string Get(const string &key, const string &item);  // 单条数据
    int Get(Ini &pack);    // （从文件中）取数据，填充pack中key对应的值

    /*
     * 写数据入对象
     * （注：若是删除某数据，只需置其状态为"delete"，再以force==true来
     *   调用Set()即可，见函数内部）；
     */
    int Set(const string &key, const string &item, const string &value); // 单条数据
    int Set(const Ini &pack, bool force=false);    // 一组数据

    /*
     * 取key对应数据的附件名列表到attachs中
     */
    int GetAttachList(const string &key, vector<string> &attachs) const;

    // 取操作结果
    const string GetResult(const string &field) const;

private:
    const string m_username;
    Conf m_result;  // 事件操作结果等记录
//    Cache m_cache;
//    File m_file;
};





}// end of USERDATA_SPACE
using namespace USERDATA_SPACE;
#endif // end of __USERDATA_H_20100430__
