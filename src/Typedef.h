#ifndef __TYPEDEF_H_20100409__
#define __TYPEDEF_H_20100409__
#include <string>
namespace TYPEDEF_SPACE
{

using namespace std;


const int BUF_MAX_LEN = 512;


/*
 *  [Rocky 2010-04-09 15:17:47]
 *  ģ�������
 */
enum
{
    OK             = 0,
    ERR            = -1,
};


/*
 * ҳ����������
 */
const string TAG_GLOBAL = "GLOBAL"; // ȫ��������



/*
 * ҳ���ύ������������
 */
const string SUBMIT_GLOBAL = "GLOBAL"; // ȫ��������






/*
 *  [Rocky 2010-04-22 11:36:19]
 *  ҳ���ڲ���Ƕ���
 */
const char TAG_LEFT[] = "<!--$"; // ������
const char TAG_RIGHT[] = "$-->"; // ������



/*
 * [Rocky 2010-05-13 16:58:23]
 * �����㷨�汾��ţ��Ժ����ݴ˺ŵ���Ӧ�汾�ӽ��ܴ���ӿڣ�
 */
const string CRYPT_VERSION = "S1";








}// end of TYPEDEF_SPACE
using namespace TYPEDEF_SPACE;
#endif // end of __TYPEDEF_H_20100409__
