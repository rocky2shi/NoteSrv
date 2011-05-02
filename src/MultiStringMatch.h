#ifndef __MULTISTRINGMATCH_H_20100521__
#define __MULTISTRINGMATCH_H_20100521__
#include <string>
#include <map>
#include <vector>
#include <algorithm>
namespace MULTISTRINGMATCH_SPACE
{

using namespace std;






/*
 * ���ܣ���ģ�ַ���ƥ�䣨���ҡ��滻��
 * ��д��Rocky 2010-05-21 10:46:59
 * �汾��v0.3
 */
class MultiStringMatch
{
    static const int INIT = 1;              // ��̬
    static const int STATE_MAX = 100000;    // �����ǣ���������״̬��С�ڴ�ֵ��
    struct Back
    {
        int state;
        int ch;
    };

public:
    // ���ڲ��ң�strings�Ӵ���
    MultiStringMatch(vector<string> &strings);

    // �����滻����from�е��Ӵ����ɶ�Ӧ��to�еĴ���
    MultiStringMatch(vector<string> &from, vector<string> &to);

    // �����滻����key��Ӧ����תΪvalue��Ӧ���ϣ�
    MultiStringMatch(map<string, string> &key2value);

    // �滻����str�е�ƥ���Ӵ���m_from���滻Ϊ��һ��Ӧ�Ӵ���m_to��
    const string Replace(const string &str);

    // ���ң�ֻҪƥ����һ���ؼ��֣�����true
    bool MatchOneKey(const string &str);

    // ���ң�����ƥ�����и����ؼ��ֲŷ���true
    bool MatchAllKey(const string &str);


private:
    // ��ʼ��
    int Init();

    // ɨ��m_from�и��Ӵ��������״̬ת����
    void MarkStateTable();

    // ȡ������״̬state��ƥ�䵱ǰ�ַ�����ʱ�Ĵ���
    const string GetBackStr(int state);

    // ���㲻ƥ��ʱ����ǰ״̬��Ӧ�Ĵ���
    void NotMatchTable();

    // ���״̬��
    void PrintStateTable();

    // �ؼ���ƥ��ִ�к���
    bool MatchKey(const string &str, bool once=true);

private:
    map<int, Back> m_BackTable;             // ���˸��ٱ�����¼��ǰ״������Դ��
    map< int, map<int, int> > m_StateTable; // ״̬��
    unsigned int m_CharTable[256];          // �ַ���
    vector<string> m_from;                  // Դ�Ӵ���
    vector<string> m_to;                    // Ŀ���Ӵ���
    map<int, string> m_NotMatchTable;       // ����ƥ��ʱ����ǰ״̬ӦתΪ�Ĵ���
};












}// end of MULTISTRINGMATCH_SPACE
using namespace MULTISTRINGMATCH_SPACE;
#endif // end of __MULTISTRINGMATCH_H_20100521__




/*******************************************************************************

history:
 2010-05-21: v0.1
 2010-05-21: v0.2������MatchOneKey()��MatchAllKey()�ӿڣ�
 2010-05-21: v0.3���޸�MultiStringMatch::MatchKey()�е�int ch��Ϊunsigned char ch��


***/
