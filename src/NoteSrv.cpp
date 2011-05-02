// Rocky 2010-04-07 18:07:41
#include "Help.h"
#include "Other.h"
#include "Timer.h"
#include "Submit.h"
#include "Compress.h"
#include "Session.h"
#include "User.h"
#include "Menu.h"
#include "Module.h"
#include "Page.h"
#include "ThreadPool.h"
#include "Counter.h"
#include "Common.h"









static void ModuleThread(Module *module)
{
    assert(NULL != module);

    LOG_INFO("Start service: [%s]", module->GetName().c_str());

    // ִ��ҵ��ѭ��
    module->Run();

    LOG_INFO("Service quit: [%s]", module->GetName().c_str());
}



int main(int argc, char *argv[])
{
    FUNCTION_TRACK(); // �����켣����


    if( (2 == argc) && ((strncmp(argv[1], "-h", 2) == 0) || (strncmp(argv[1], "--help", 6) == 0)) )
    {
        Help();
        return 0;
    }
    if( (2 == argc) && (strncmp(argv[1], "--changes", 2) == 0) )
    {
        Changes();
        return 0;
    }

    {/****************************** ��ʼ�� ��ʼ ******************************/

    int ret;



    // ��ʼ��ȫ��������
    ret = GlobalConfig::init(argc, argv);
    if(ret < 0)
    {
        printf("��ʼ��ȫ����������� [%d] \n", ret);
        return ret;
    }

    // ��ʼ��ϵͳ����
    ret = SystemInit();
    if(ret < 0)
    {
        printf("��ʼ��ϵͳ���ó��� [%d] \n", ret);
        return ret;
    }

    // ��ʼ����־������
    ret = Log::init();
    if(ret < 0)
    {
        printf("��ʼ����־��������� [%d] \n", ret);
        return ret;
    }

    // ��ʼ��ȫ��������
    ret = ClassInit::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ��ȫ�������ó��� [%d] \n", ret);
        return ret;
    }

    // ��ʼ����������
    ret = Counter::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ����ʼ������������� [%d]", ret);
        return ret;
    }

    // ��ʼ����ʱ��������
    ret = Timer::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ����ʱ����������� [%d] ", ret);
        return ret;
    }

    // ��ʼ���̳߳���
    ret = ThreadPool::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ���̳߳������ [%d] ", ret);
        return ret;
    }

    // // ��ʼ��ϵͳ�ź���
    // ret = SignalDeal::init();
    // if(ret < 0)
    // {
    //     LOG_ERROR("��ʼ��ϵͳ�ź������ [%d] ", ret);
    //     return ret;
    // }

    // ��ʼ��ģ�������
    ret = Module::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ��ģ���������� [%d] ", ret);
        return ret;
    }

    // ��ʼ��ҳ�洦����
    ret = Page::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ��ҳ�洦������� [%d] ", ret);
        return ret;
    }

    // ��ʼ���Ự��
    ret = Session::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ���Ự����� [%d] ", ret);
        return ret;
    }

    // ��ʼ���û���Ϣ��
    ret = User::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ���û���Ϣ����� [%d] ", ret);
        return ret;
    }

    // ��ʼ���˵�������
    ret = Menu::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ���˵���������� [%d] ", ret);
        return ret;
    }

    // ��ʼ��ѹ��������
    ret = Compress::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ��ѹ����������� [%d] ", ret);
        return ret;
    }

    // ��ʼ���ύ�¼�������
    ret = Submit::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ���ύ�¼���������� [%d] ", ret);
        return ret;
    }

    // ��ʼ�������������
    ret = OtherInit();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ����������������� [%d] ", ret);
        return ret;
    }






    }/****************************** ��ʼ�� ���� ******************************/




    Module::iterator module;    // ������module�ڲ�ָ����Module��ĳ�Ա��
    boost::thread_group grp;

    // ��ȡ���д򿪵�ģ�飬ÿ��ģ���Ӧһ�������̣߳�
    while( module.next() )
    {
        /*
         * module()�ǵ��������˵�()�������ȡ��ģ��ָ�룬��Ϊ���������̺߳�
         * ��ModuleThread()�����̺߳�������ִ�д���ѭ����
         */
        grp.create_thread( boost::bind( &ModuleThread, module() ) );
    }

    grp.join_all();


    LOG_INFO("���߳��˳�������");
    pthread_exit(NULL);
    // return 0;
}





