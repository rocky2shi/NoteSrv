#ifndef __GLOBALCONFIG_H_20100414__
#define __GLOBALCONFIG_H_20100414__
#include "Common.h"
#include "Environment.h"
namespace GLOBALCONFIG_SPACE
{





// ȫ�����ù���
class GlobalConfig
{
public:
    // ���ʼ��
    static int init(int argc, char *argv[]);

    // ���û��ȡ����
    inline static GlobalConfig *instance(GlobalConfig *config=NULL)
    {
        static GlobalConfig *obj = (GlobalConfig *)(Environment::instance()->ClassInit("GlobalConfig", config));
        return obj;
    }

    // ϵͳ��Ŀ¼
    const string Root() const;

    // ȫ������Ŀ¼
    const string SystemDataDir() const;

    // �û�Ŀ¼��ŵ�
    const string UserRootDir() const;

    // ҳ�洦��̬�⣨ģ�飩Ŀ¼
    const string CgiDir() const;

    // �ı�ҳ��Ŀ¼�����html��js��txt��css����ֱ����ҳ������ʾ���ļ���
    const string HtmlDir() const;

    // ģ���ļ�Ŀ¼
    const string TemplateDir() const;

    // ϵͳ��־Ŀ¼
    const string SystemLogDir() const;

    // ϵͳ��ʱĿ¼
    const string SystemTmpDir() const;

    // ��Ź��ߡ��ⲿ�������ȵ�Ŀ¼
    const string ToolDir() const;

    // ȫ�ּ�����Ŀ¼
    const string CounterDir() const;

    // ��ȡ�����в���������ѡ���ȡfield�ֶζ�Ӧ�Ĳ������������򷵻�Ĭ�ϵ�defֵ��
    const string GetOption(const string &field, const string def="") const;

private:
    GlobalConfig(int argc, char *argv[]);


private:
    map<string, string> m_option;   // ����ѡ��
};









}// end of GLOBALCONFIG_SPACE
using namespace GLOBALCONFIG_SPACE;
#endif // end of __GLOBALCONFIG_H_20100414__
