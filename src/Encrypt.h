#ifndef __ENCRYPT_H_20100513__
#define __ENCRYPT_H_20100513__
#include "Common.h"
namespace ENCRYPT_SPACE
{




/* 功能：加解密
 * 编写：Rocky 2010-05-13 12:26:31
 * 版本：v0.1
 */
class Encrypt
{
public:
    Encrypt(const string &password);

    // 加密
    const string encrypt(const string &data);

    // 解密
    const string decrypt(const string &data);

private:
    const string m_password;
    const int m_seed;
};







}// end of ENCRYPT_SPACE
using namespace ENCRYPT_SPACE;
#endif // end of __ENCRYPT_H_20100513__
