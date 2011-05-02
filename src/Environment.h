#ifndef __ENVIRONMENT_H_20100429__
#define __ENVIRONMENT_H_20100429__
#include <string>
#include <map>
namespace ENVIRONMENT_SPACE
{


using namespace std;



// �������ݣ���ģ�顢��̬��֮�䣩
class Environment
{
public:
    inline static Environment *instance(Environment *exterior=NULL)
    {
        /*
         * ptrָ������������
         *
         *   (1). ����ģ�飨main()������ʱ����һ�ε�instance()���޴���������envʹ
         *        ��Ĭ�ϵ�NULLֵ����ʱinstance()���ý������ڲ��ľ�̬����������ã�
         *
         *   (2). ��������ģ����ʹ��ʱ����һ�ε�instance()����Ҫ���������������
         *        ��Ӧ����(1)�з��صĲ������Ա������������У�������ģ��ָ����ͬ��
         *        ִ�д��루�����ݣ���
         */
        static Environment interior;
        static Environment *ptr = exterior ? : &interior;
        return ptr;
    }

    int Insert(const string &id, void *obj);
    void *Get(const string &id);

    // ��ʼ��ȫ����
    void *ClassInit(const string &id, void *obj);

private:
    Environment();

private:
    map<string, void *> m_queue;
};




/*
�÷�ʾ��˵����

----------------------------------- �������� -----------------------------------

--------------Step 1: �����ȡ�����ã���ȫ��ʵ��
// ���û��ȡ����
inline static GlobalConfig *GlobalConfig::instance(GlobalConfig *config=NULL)
{
    static GlobalConfig *obj = (GlobalConfig *)(Environment::instance()->ClassInit("GlobalConfig", config));
    return obj;
}


--------------Step 2: �ڳ����ʼ��ʱ������ȫ��ʵ������¼�У��Ա�����ģ�飨��̬�⣩��ȡ����ʹ�ã�
// ���ʼ��
int GlobalConfig::init()
{
    int ret;
    static GlobalConfig config;

    if(NULL == GlobalConfig::instance( &config ))
    {
        LOG_ERROR("GlobalConfig::instance() error");
        return ERR;
    }

    return OK;
}


�����磺
    GlobalConfig::instance()->Get(...);

    ��instance()�е����Ϊ�գ�




----------------------------------- ��ģ�飨��̬�⣩�� -------------------------
�����ʼ����ֱ�ӵ��ã��磺

    GlobalConfig::instance()->Get(...);

    ��instance()�е����Ϊ�գ�

��ʱ�����ǵ�һ�ε��ã����ڲ�����ã�
    Environment::instance()->ClassInit()
����ʼ�����ڲ��ľ�̬��������ʹ����ģ���д���Ķ���ʵ��Ϊִ�в������Դﵽ������ģ
��ͳһ��Ŀ�ģ�






*/













}// end of ENVIRONMENT_SPACE
using namespace ENVIRONMENT_SPACE;
#endif // end of __ENVIRONMENT_H_20100429__
