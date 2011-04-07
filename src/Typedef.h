#ifndef __TYPEDEF_H_20100409__
#define __TYPEDEF_H_20100409__
#include <string>
namespace TYPEDEF_SPACE
{

using namespace std;


const int BUF_MAX_LEN = 512;


/*
 *  [Rocky 2010-04-09 15:17:47]
 *  模块错误码
 */
enum
{
    OK             = 0,
    ERR            = -1,
};


/*
 * 页面标记作用域；
 */
const string TAG_GLOBAL = "GLOBAL"; // 全局作用域



/*
 * 页面提交处理标记作用域；
 */
const string SUBMIT_GLOBAL = "GLOBAL"; // 全局作用域






/*
 *  [Rocky 2010-04-22 11:36:19]
 *  页面内部标记定义
 */
const char TAG_LEFT[] = "<!--$"; // 左括号
const char TAG_RIGHT[] = "$-->"; // 右括号



/*
 * [Rocky 2010-05-13 16:58:23]
 * 加密算法版本编号（以后会根据此号调对应版本加解密处理接口）
 */
const string CRYPT_VERSION = "S1";








}// end of TYPEDEF_SPACE
using namespace TYPEDEF_SPACE;
#endif // end of __TYPEDEF_H_20100409__
