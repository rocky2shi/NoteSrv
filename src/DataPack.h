#ifndef __DATAPACK_H_20100518__
#define __DATAPACK_H_20100518__
#include "UserData.h"
#include "Common.h"
namespace DATAPACK_SPACE
{





// 数据打包（用户数据、附件等，自定义结构）
class DataPack
{
    // 校验数
    static const unsigned int MAGIC = 0xCC57EE9D;
    // 当前版本号（包内记录版号，决定使用哪个版本处理代码）
    static const string CURRENT_VERSION; // = "20100519100025";

    // 包内（各个文件）的类型
    typedef enum{
        FT_DATA,            // 数据文件
        FT_ATTACH,          // 附件文件
        FT_MENU_CFG,        // 各（下拉）菜单数据
        FT_PAGE_CFG,        // 各页面（配置）数据
    } FileType;

    // 当前操作的类型
    typedef enum{
        OPR_UNKNOWN,        // 未确定操作（出错）
        OPR_MAKEPACK,       // 打包
        OPR_UNPACK,         // 解包
        OPR_FINISH,         // 打包（或解包）操作完成
    } OprType;

    // 包里每个文件的头部
    struct Head
    {
        int type;               // 数据类型（数据文件、附件等）（见上边FileType定义）
        char basename[256];     // 文件名
        unsigned int filesize;  // 数据部分长度（字节数）
        long mtime;             // 最后修改时间
        char reserve[16];       // 保留（可能以后用于存放文件属性等）
        unsigned int magic;

        Head()
        {
            memset(this, 0, sizeof(*this)); // 各字段清零
            magic = MAGIC;      // 设置校验值
        }
        // 头部有效返回true（解包时用）
        bool isValid()
        {
            return MAGIC == magic;
        }
    };

    // 版本信息
    struct Version
    {
        char key[32];
        Version()
        {
            // 以当前时间串为版本标记
            strcpy(key, CURRENT_VERSION.c_str());
        }
        // 版本匹配返回true
        bool isValid()
        {
            return CURRENT_VERSION == key;
        }
    };

public:
    // 打包属性
    typedef enum
    {
        A_EXCLUDE_ATTACH,   // 排除附件（因附件较大，有时不需打包）
        //
        // 新加属性的这里加
        //
        A_MAX
    } Attrib;


public:
    DataPack(const string &username);
    ~DataPack();

    /*
     * ========== 打包处理 ==========
     */
    // 设置打包信息
    int MakePack();

    // 加入需要打包的用户数据（及附件）
    int Add(const Ini &pack);

    // 打包所有用户数据
    int AddAll();

    // 打包其它项：菜单、各页面配置等；
    int OtherPack();

    // 结束打包，并返回包文件名；
    const string PackFinish();

    // 设置（或清除）打包属性
    void SetAttrib(Attrib type, bool valid=true);

    /*
     * ========== 解包处理 ==========
     */
    // 设置解包信息（pack: 待解的包文件名）
    int UnPack(const string &pack);

    /*
     * ========== 其它操作 ==========
     */
    // 取处理结果
    const string GetResult(const string &field) const;

private:
    // 打包一个文件（把filename文件加入包中）
    int FilePack(const string &filelname, FileType type);

    // 文件解包；
    int FileUnPack();

    // 清理
    void Clear();


private:
    FileObj m_pack;          // 打包文件
    FileObj m_ini;           // 临时用户数据文件
    UserData m_UserData;     // 当前操作的用户数据
    const string m_username; // 当前操作的用户名
    OprType m_opr;           // 当前对象执行的操作：1:打包 2:解包
    bool m_attribs[A_MAX];   // 打包属性标志组
    Conf m_result;           // 记录操作结果
};







}// end of DATAPACK_SPACE
using namespace DATAPACK_SPACE;
#endif // end of __DATAPACK_H_20100518__
