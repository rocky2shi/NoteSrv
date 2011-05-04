// Rocky 2010-05-13 12:25:03
#include "Encrypt.h"
namespace ENCRYPT_SPACE
{






/******************************************************************************/

/*
 * 功能：打乱及还原字符串
 * 编写：Rocky 2010-05-14 10:42:45
 */
class Swap
{
public:
    // 把str顺序打乱
    static const string doit(int seed, const string &str)
    {
        string result = str;
        const int len = str.length();
        seed += len; // 再加串的长度做相关性
        int i;

        for(i=0; i<len; i++)
        {
            int range = len - i - 1;
            int m = Rand(seed, i, 0, range);

            char tmp = result[ range ];
            result[ range ] = result[ m ];
            result[ m ] = tmp;
        }
        result[ len ] = '\0';
        return result;
    }

    // 对应于Swap0()，即还原str串；
    static const string undo(int seed, const string &str)
    {
        string result = str;
        const int len = str.length();
        seed += len; // 再加串的长度做相关性
        int i;

        for(i=len-1; i>=0; i--)
        {
            int range = len - i - 1;
            int m = Rand(seed, i, 0, range);

            char tmp = result[ m ];
            result[ m ] = result[ range ];
            result[ range ] = tmp;
        }
        result[ len ] = '\0';
        return result;
    }

private:
    // 取随机数（伪）
    static unsigned int Rand(int seed, int n, int min, int max)
    {
        unsigned int m = ~(seed * 262147 * n);
        return (min + m) % ((max + 1 - min)?:1);
    }
};

static const string encrypt0(string password, const char *data)
{
    password = Crypt(password, CRYPT_VERSION);
    const int nPasswdLen = password.length();   // 密码编码后的长度
    const int nDataLen = strlen(data);          // 数据长度
    int iData = 0;
    int iPaswd = 0;
    const int BUF_MAX = 256;
    char buf[BUF_MAX];  // 长度应该足够比nPasswdLen大了
    int iBuf = 0;   // buf的下标
    string result;

    if(nPasswdLen > BUF_MAX / 2)
    {
        Debug("Buf not enough.");
        return "";
    }

    /*
     * 明文和密码做异或，一段一段地把结果放到result中；
     */
    for(iData=0; iData<nDataLen; iData++)
    {
        int num = (unsigned char )data[iData] ^ (unsigned char )password[iPaswd];

        AsciiToString(num, buf + iBuf); // 编码为可见字符，即：num 字符转为 ASCII
        iBuf += 2;

        iPaswd++;
        if(iPaswd >= nPasswdLen)
        {
            buf[iBuf] = '\0';
            result += buf;
            iPaswd = 0;
            iBuf = 0;
        }
    }
    buf[iBuf] = '\0';
    result += buf;

    return result;
}
static const string encrypt0(string password, const string &data)
{
    return encrypt0(password, data.c_str());
}


static const string decrypt0(string password, const char *data)
{
    password = Crypt(password, CRYPT_VERSION);
    const int nPasswdLen = password.length();   // 密码编码后的长度
    const int nDataLen = strlen(data);          // 数据长度
    int iData = 0;
    int iPaswd = 0;
    const int BUF_MAX = 256;
    char buf[BUF_MAX];  // 长度应该足够比nPasswdLen大了
    int iBuf = 0;   // buf的下标
    string result;

    if(nDataLen % 2 != 0)
    {
        Debug("Data format error");
        return "";
    }
    if(nPasswdLen > BUF_MAX / 2)
    {
        Debug("Buf not enough.");
        return "";
    }

    Debug("data=[%s] [%d]", data, nDataLen);

    /*
     * 明文和密码做异或，一段一段地把结果放到result中；
     */
    for(iData=0; iData<nDataLen; iData+=2)
    {
        const int num = toInt(data[iData], data[iData + 1]);
        buf[iBuf] = num ^ (unsigned char )password[iPaswd];
        iBuf++;
        iPaswd++;
        if(iPaswd >= nPasswdLen)
        {
            buf[iBuf] = '\0';
            result += buf;
            iPaswd = 0;
            iBuf = 0;
        }
    }
    buf[iBuf] = '\0';
    result += buf;

    return result;
}
static const string decrypt0(string password, const string &data)
{
    return decrypt0(password, data.c_str());
}

/******************************************************************************/















Encrypt::Encrypt(const string &password)
            : m_password(password),
              m_seed( SDBMHash(m_password.c_str()) )
{
}

// 加密
const string Encrypt::encrypt(const string &data)
{
    const string &s1 = encrypt0(m_password, data);
    return Swap::doit(m_seed, s1);
}

// 解密
const string Encrypt::decrypt(const string &data)
{
    const string &s1 = Swap::undo(m_seed, data);
    return decrypt0( m_password, s1);
}







}// end of ENCRYPT_SPACE
