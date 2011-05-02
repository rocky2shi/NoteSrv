// Rocky 2010-04-14 19:18:50
#include "GlobalConfig.h"
namespace GLOBALCONFIG_SPACE
{






GlobalConfig::GlobalConfig(int argc, char *argv[])
{
    /*
     * �鿴��������
     */
    m_option["::RootDir"] = getenv("RootDir") ? : ".";
    m_option["::ServicePort"] = getenv("ServicePort") ? : "80";

    /*
     * �鿴�����У��Ȼ����������ȣ�
     */
    // ��һ������Ϊ����˿�
    if(argc > 1)
    {
        m_option["::ServicePort"] = argv[1];
    }

    // �ڶ�������Ϊ�����Ŀ¼
    if(argc > 2)
    {
        m_option["::RootDir"] = argv[2];
    }
}

// ���ʼ��
int GlobalConfig::init(int argc, char *argv[])
{
    int ret;
    static GlobalConfig config(argc, argv);

    if(NULL == GlobalConfig::instance( &config ))
    {
        printf("GlobalConfig::instance() error");   // [XXX]
        return ERR;
    }

    return OK;
}

// ϵͳ��Ŀ¼
const string GlobalConfig::Root() const
{
    // ���û�δ���ã���Ĭ��Ϊ��ǰĿ¼��
    return GetOption("::RootDir", ".") + "/";
}

// �û�Ŀ¼��ŵ�
const string GlobalConfig::UserRootDir() const
{
    return Root() + "data/user/";
}

// ҳ�洦��̬�⣨ģ�飩Ŀ¼
const string GlobalConfig::CgiDir() const
{
    return Root() + "cgi/";
}

// �ı�ҳ��Ŀ¼�����html��js��txt��css����ֱ����ҳ������ʾ���ļ���
const string GlobalConfig::HtmlDir() const
{
    return Root() + "html/";
}

// ģ���ļ�Ŀ¼
const string GlobalConfig::TemplateDir() const
{
    return Root() + "template/";
}


// ϵͳ��־Ŀ¼
const string GlobalConfig::SystemLogDir() const
{
    return Root() + "log/";
}

// ϵͳ��ʱĿ¼
const string GlobalConfig::SystemTmpDir() const
{
    return Root() + "tmp/";
}

// ȫ������Ŀ¼
const string GlobalConfig::SystemDataDir() const
{
    return Root() + "data/";
}

// ��Ź��ߡ��ⲿ�������ȵ�Ŀ¼
const string GlobalConfig::ToolDir() const
{
    return Root() + "tool/";
}

// ȫ�ּ�����Ŀ¼
const string GlobalConfig::CounterDir() const
{
    return Root() + "data/counter/";
}

// ��ȡ�����в���������ѡ���ȡfield�ֶζ�Ӧ�Ĳ������������򷵻�Ĭ�ϵ�defֵ��
const string GlobalConfig::GetOption(const string &field, const string def/*=""*/) const
{
    return GetMapValue(m_option, field, def);
}



















}// end of GLOBALCONFIG_SPACE
