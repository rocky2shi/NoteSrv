// Rocky 2010-03-16 09:57:12
#include "Module.h"
namespace MODULE_SPACE
{








Module::Factory Module::m_Factory;


Module::Module()
{
    FUNCTION_TRACK(); // �����켣����
}

Module::Module(const string &id): m_module(id)
{
    FUNCTION_TRACK(); // �����켣����

    // ��δ����������ǰ�ѵ��ã���ʱ��m_TmpFactory[ index ]ӦΪNULL
    assert(NULL == m_Factory[ id ]);

    Register(id, this);
}

Module::~Module()
{
    FUNCTION_TRACK(); // �����켣����
}


/*
 * ���ܣ�ע��������󵽹���
 * ��д��Rocky 2009-09-07 17:09:46
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 * ע�⣺�˺����ڽ���main()����֮ǰ���ɹ��캯�������ˣ���Щ����ʹ��MB_LOGGER_xxx()��
 */
int Module::Register(const string &id, Module *sub)
{
    FUNCTION_TRACK(); // �����켣����
    assert("" != id && NULL == m_Factory[ id ]);
    m_Factory[ id ] = sub;  // �������
    return OK;
}

/*
 * ���ܣ��ر�ģ��
 * ��д��Rocky 2010-03-16 14:52:25
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 * ע�⣺�˺����ڽ���main()����֮ǰ���ɹ��캯��������
 */
int Module::UnRegister(const string &id)
{
    if(NULL == m_Factory[ id ])
    {
        return ERR;
    }
    m_Factory.erase(id); // ɾ��
    return OK;
}


/*
 * ���ܣ���̬��ʼ������
 * ��д��Rocky 2009-09-23 14:14:55
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
int Module::init()
{
    FUNCTION_TRACK(); // �����켣����


    Factory::iterator it;

    for (it = m_Factory.begin(); it != m_Factory.end(); ++it)
    {
        const string &id = it->first;
        Module * const &db = it->second;

        // ���������ʼ������
        if( db->DoInit() < 0 )
        {
            LOG_ERROR("��ʼ��Module������[%s]����", id.c_str() );
            return ERR;
        }
        LOG_DEBUG("��ʼ��Module������[%s]����", id.c_str());
    }

    return OK;
}

/*
 * ���ܣ������ֵĳ�ʼ�����������ƣ����Գ鵽���ࣨ����������������ʽ���ã�
 * ��д��Rocky 2009-09-15 18:30:21
 * ���أ���ȷ����>=0ֵ������<0ֵ��
 */
int Module::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

//    // ��ģ��ͳһʹ�õĴ���
//    const string close = GetCfgValue(m_module, "close", "");
//    if("1" == close)
//    {
//        LOG_ERROR("ģ��[%s]����Ϊ�ر�", m_module.c_str() );
//        UnRegister(m_module);
//        return OK;
//    }

    return OK;
}





















/******************** �����������²�����ִ�о���ҵ�� ********************/
/*
 *  Run(): ��ѭ��������Deal()
 */

// ȡģ����
const string Module::GetName() const
{
    return m_module;
}


// ѭ�����ô�����
int Module::Run()
{
    FUNCTION_TRACK(); // �����켣����

    // �Ȳ鿴��ǰ�������ã��粻�裬�ٲ鿴���õ����ã��ٲ�����ʹ��Ĭ��ֵ��10�룩
    const int second = 3; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MyBox::S2I( GetCfgValue("Other", "main_loop_interval", "10") );

    LOG_INFO("main_loop_interval=[%d]", second);

    // ����ÿ������
    while( !SignalDeal::exit() )
    {
        // ���ô�����
        Deal();

        // ������ɣ���ͣsecond�롣
        sleep(second);
    }// end of while( ...

    return OK;
}

// ������
int Module::Deal()
{
    FUNCTION_TRACK(); // �����켣����
    LOG_ERROR("�ݲ�֧�ִ�ģ��[%s]", m_module.c_str() );
    return ERR;
}




}// end of MODULE_SPACE
