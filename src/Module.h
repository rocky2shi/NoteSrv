#ifndef __MODULE_H_20100316__
#define __MODULE_H_20100316__
#include "SignalDeal.h"
#include "Common.h"
namespace MODULE_SPACE
{






// ���ݱ��������
class Module
{
public:
    // �������������г�����ģ�顣ע�⣬������ָ�����Module::m_Factory����
    class iterator
    {
    public:
        iterator(): first(true)
        {
            it = Module::m_Factory.begin();
        }

        // ָ����һ��Ԫ��
        bool next()
        {
            if(first)   // ���ǵ�һ�ε���next()�������Ӽӣ�
            {
                first = false;
            }
            else
            {
                it++;
            }
            return Module::m_Factory.end() != it;
        }

        // ����()������������ģ������ַ��
        Module * operator()()
        {
            return it->second;
        }

    private:
        map<string, Module*>::iterator it;
        bool first;
    };


public:
    // ѭ������Deal()
    virtual int Run();

    // ҵ������ڣ��ɸ�����ʵ�֣�
    virtual int Deal();

    // ȡģ����
    const string GetName() const;


protected:
    string m_module;  // ��ģ�����ƣ�id��



/*
 * ���´�����Ҫ���ڹ�����������
 */
public:
    virtual ~Module();

    // ��ʼ��
    static int init();


protected:
    Module();
    Module(const string &id); // ������������г���ע��

    // ע�ᣨ������������m_Factory����ע���ӿ���Ϊstatic�Է�ֹRegister()�ڲ�
    // �����޸������Ա��
    static int Register(const string &id, Module *sub);
    // �ر�ģ��
    static int UnRegister(const string &id);

    // �����ฺ��ʵ�����ѳ�ʼ������
    virtual int DoInit() = 0 ;

private:
    typedef map<string, Module *> Factory;

    /*
     * �ڽ���main()����֮ǰ���������Ѱ����Ѳ��뵽m_Factory��������
     * ��main()�е���init()ʱ���ٴε���m_Factory���Ӷ���ĳ�ʼ����
     * ����
     */
    static Factory m_Factory;   // ��ʼ�������Ĳ����Ӷ��󹤳�
};







}// end of MODULE_SPACE
using namespace MODULE_SPACE;
#endif // end of __MODULE_H_20100316__
