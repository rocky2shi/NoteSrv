// Rocky 2010-04-07 18:07:41


#include "Common.h"
#include "Connect.h"
#include "ConnectQueue.h"
#include "FileObj.h"
#include "GetFileList.h"
#include "Ini.h"
#include "UserData.h"
#include "GlobalConfig.h"
#include "ThreadPool.h"
#include "User.h"
#include "Menu.h"
#include "Encrypt.h"
#include "ChineseCoding.h"
#include "Submit.h"
#include "Compress.h"
#include "Session.h"
#include "User.h"
#include "Menu.h"
#include "Module.h"
#include "Page.h"
#include "ThreadPool.h"
#include "Counter.h"
#include "PageCfg.h"
#include "DataPack.h"
#include "MultiStringMatch.h"
#include "Timer.h"
#include "Other.h"
//#include "StringFilter.h"




#undef Debug
#define Debug(format, args...)   printf("%s:%d : " format "\n", __FILE__, __LINE__, ##args)



int main(int argc, char *argv[])
{
    FUNCTION_TRACK(); // �����켣����


    {/****************************** ��ʼ�� ��ʼ ******************************/

    int ret;

    // ��־�Ե���ǰ̨
    setenv("LOG_TERMINAL", "1", 1);

    // ��ʼ��ȫ��������
    ret = GlobalConfig::init(argc, argv);
    if(ret < 0)
    {
        LOG_ERROR("��ʼ��ȫ����������� [%d] \n", ret);
        return ret;
    }

//    // ��ʼ��ϵͳ����
//    ret = SystemInit();
//    if(ret < 0)
//    {
//        printf("��ʼ����ʼ��ϵͳ���ó��� [%d] \n", ret);
//        return ret;
//    }

    // ��ʼ����־������
    ret = Log::init();
    if(ret < 0)
    {
        LOG_ERROR("��ʼ����־��������� [%d] \n", ret);
        return ret;
    }

//    // ��ʼ����������
//    ret = Counter::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ����ʼ������������� [%d]", ret);
//        return ret;
//    }
//
//    // ��ʼ����ʱ��������
//    ret = Timer::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ����ʱ����������� [%d] ", ret);
//        return ret;
//    }
//
//    // ��ʼ���̳߳���
//    ret = ThreadPool::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ���̳߳������ [%d] ", ret);
//        return ret;
//    }
//
//    // // ��ʼ��ϵͳ�ź���
//    // ret = SignalDeal::init();
//    // if(ret < 0)
//    // {
//    //     LOG_ERROR("��ʼ��ϵͳ�ź������ [%d] ", ret);
//    //     return ret;
//    // }
//
//    // ��ʼ��ģ�������
//    ret = Module::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ��ģ���������� [%d] ", ret);
//        return ret;
//    }
//
//    // ��ʼ��ҳ�洦����
//    ret = Page::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ��ҳ�洦������� [%d] ", ret);
//        return ret;
//    }
//
//    // ��ʼ���Ự��
//    ret = Session::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ���Ự����� [%d] ", ret);
//        return ret;
//    }
//
//    // ��ʼ���û���Ϣ��
//    ret = User::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ���û���Ϣ����� [%d] ", ret);
//        return ret;
//    }
//
//    // ��ʼ���˵�������
//    ret = Menu::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ���˵���������� [%d] ", ret);
//        return ret;
//    }
//
//    // ��ʼ��ѹ��������
//    ret = Compress::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ��ѹ����������� [%d] ", ret);
//        return ret;
//    }
//
//    // ��ʼ���ύ�¼�������
//    ret = Submit::init();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ���ύ�¼���������� [%d] ", ret);
//        return ret;
//    }
//
//    // ��ʼ�������������
//    ret = OtherInit();
//    if(ret < 0)
//    {
//        LOG_ERROR("��ʼ����������������� [%d] ", ret);
//        return ret;
//    }






    }/****************************** ��ʼ�� ���� ******************************/





    {// ���� �������ӷ����(TCP)
    #if 0

        int ret;
        Connect::auto_ptr server;

        ret = server->InitAsServer(17890);
        if(ret < 0)
        {
            LOG_ERROR("��ʼ������");
            return -1;
        }

        while(1)
        {
            Connect * const pConnect = server->Accept();
            if(NULL == pConnect)
            {
                LOG_ERROR("���ӳ���");
                continue;
            }


            int i = 0;
            char buf[128] = "";
            while(1)
            {
                int ret = pConnect->Recv(buf, sizeof(buf) - 1);
                if(ret <= 0)
                {
                    break;
                }
                buf[ret] = '\0';
                LOG_DEBUG("%d. [%s]", i, buf);
                i++;
            }

            pConnect->Close();

            LOG_DEBUG("�ѶϿ�");


            if( strstr(buf, "exit") != NULL )
            {
                break;
            }
        }
    #endif
    }

    {// ���� ConnectQueue
    #if 0

        ConnectQueue::init();

        Connect *p =Connect::New();

        LOG_DEBUG("p=[%p]", p);
        ConnectQueue::instance()->push(p);
        ConnectQueue::instance()->push(p);
        ConnectQueue::instance()->push(p);

        Connect *p2;

        p2 = ConnectQueue::instance()->pop();
        LOG_DEBUG("p2=[%p]", p2);

        p2 = ConnectQueue::instance()->pop();
        LOG_DEBUG("p2=[%p]", p2);

        p2 = ConnectQueue::instance()->pop();
        LOG_DEBUG("p2=[%p]", p2);

        p2 = ConnectQueue::instance()->pop();
        LOG_DEBUG("p2=[%p]", p2);

    #endif
    }

    {// ���� FileObj
    #if 0

        FileObj file;

        if( !file.Open("../tmp/a.bin") )
        {
            LOG_ERROR("Open() error");
            return -1;
        }

        char buf[9] = "";
        int ret;

        while( (ret = file.GetLine(buf, 8)) > 0 )
        {
            LOG_DEBUG("[%d] [%s]", ret, buf);
        }




    #endif
    }

    {// ���� DirList
    #if 0

        GetFileList list("../tmp/Data");

        list.SortFilename();

        GetFileList::file_iterator file(list);

        while( file.next() )
        {
            printf("[%s]\n", file.name());
        }

        LOG_DEBUG("-----------------------------------------");

//        GetFileList::dir_iterator dir(list);
//
//        while( dir.next() )
//        {
//            LOG_DEBUG("[%s]", dir.name());
//        }

    #endif
    }

    {// ���� Ini
    #if 0

        Ini ini("../tmp/a.ini");

//        ini.Del("4");
//        ini.Del("6");
//
//        ini.Dump();
//
//        LOG_DEBUG("-------------------------------------------");
//
//        ini.Del("8", "port");
//
//        ini.Dump();
//
//        LOG_DEBUG("-------------------------------------------");
//
//        ini.Set("a", "name", "rocky");
//        ini.Set("a", "name2", "rocky2");

        ini.Dump();

        ini.Write("../tmp/a2.ini");
        ini.Set("ok", "name", "rocky");
        ini.Write();


    #endif
    }

    {// ���� UserData::iterator
    #if 0

        UserData::iterator it( "../tmp/data2" );

        // �г���������
        while( it.next() )
        {
            string key;
            Conf *pack = NULL;

            // ȡ����ǰ��ָ��key��Pack
            it.Get(key, pack);

            printf("\n[%s]\n", key.c_str());

            Conf::iterator itPack(pack);
            while( itPack.next() )
            {
                printf("%s=%s\n", itPack.GetName(), itPack.GetValue());
            }

            LOG_DEBUG("*****************************************");
        }


    #endif
    }

    {// ���� Escape
    #if 0

        string s = "1234567890-------{&}---{<}----{>}--";

        string str = Escape(s);

        Debug("[%s]", s.c_str());

        Debug("[%s]", str.c_str() );



    #endif
    }

    {// ���� GlobalConfig
    #if 0

        int ret = GlobalConfig::init();
        if(ret < 0)
        {
            Debug("err");
            return -1;
        }

        string str;

        str = GlobalConfig::instance()->UserRootDir();
        Debug("UserRootDir=[%s]", str.c_str());

        str = GlobalConfig::instance()->CgiDir();
        Debug("CgiDir=[%s]", str.c_str());

        str = GlobalConfig::instance()->HtmlDir();
        Debug("HtmlDir=[%s]", str.c_str());

        str = GlobalConfig::instance()->TemplateDir();
        Debug("TemplateDir=[%s]", str.c_str());



    #endif
    }


    {// ���� TextToWeb
    #if 0

        string str = "-&-<->-1234567890-=\\";

        string s;

        s = TextToWeb( str );

        Debug("[%s]", str.c_str());
        Debug("[%s]", s.c_str());


    #endif
    }

    {// ���� WebToText
    #if 0

        string str = "--[&lt;]--[&lt]--[&lt;&lt;]--[&lt&lt]--[&lt;&lt]--[&lt&lt;]";

        string s;

        s = WebToText( str );

        Debug("[%s]", str.c_str());
        Debug("[%s]", s.c_str());


    #endif
    }

    {// ���� UrlDecode
    #if 0

        char str[512] = "�й���1234567890%5B%24v+%3D%7E+s%2F%5C%2B%2F+%2Fg%3B%5D%0D%0A";

        Debug("[%s]", str);

        Debug("[%s]", UrlDecode(str));

    #endif
    }


    {// ���� ThreadPool
    #if 0

        struct BackCall
        {
            static void *exe(void *)
            {
                LOG_INFO("BackCall::exe()........");
                return NULL;
            }
        };

        ThreadPool pool( BackCall::exe );

        sleep(2);

    #endif
    }


    {// ���� KeyToSecond
    #if 0

        Debug("[%lu]", KeyToSecond(argv[1]?argv[1]:"20011112183101"));

    #endif
    }


    {// ���� User       [2010-05-06 14:52:12]
    #if 0

        {
            User *user = User::Get("rocky");

            Debug("[%s]", user->DataDir().c_str());
            Debug("[%s]", user->GetInfo("password").c_str());
            Debug("[%s]", user->isValid()?"��Ч":"��Ч�û�");
        }


        {
            User *user = User::Get("rocky2");

            Debug("[%s]", user->DataDir().c_str());
            Debug("[%s]", user->GetInfo("password").c_str());
            Debug("[%s]", user->isValid()?"��Ч":"��Ч�û�");
        }

    #endif
    }


    {// ���� Menu   [2010-05-06 18:45:11]
    #if 0

        {
            Menu *menu = Menu::Get("rocky", "TypeDropBox.cfg");

            Menu::iterator it(menu);

            while( it.next() )
            {
                Debug("[%s]", it->Get("title").c_str());
            }
        }
        Debug("-------------------------------------------------------");
        {
            Menu *menu = Menu::Get("rocky2", "TypeDropBox.cfg");

            Menu::iterator it(menu);

            while( it.next() )
            {
                Debug("[%s]", it->Get("title").c_str());
            }
        }
        Debug("-------------------------------------------------------");
        {
            Menu *menu = Menu::Get("rocky", "TypeDropBox.cfg");

            Menu::iterator it(menu);

            while( it.next() )
            {
                Debug("[%s]", it->Get("title").c_str());
            }
        }
    #endif
    }


    {// ���� XXX
    #if 0

        Debug("[%s]", Crypt(argv[1]?:"123").c_str());

    #endif
    }


    {// ���� UrlCode
    #if 0
//        char from[] = "Invalid user: ";
//        char to[256];
//
//        UrlCode(from, to, sizeof(to));
//
//        Debug("[%s]", to);
//
//        string str = "Invalid user: ";
//        string s = UrlCode(str);
//
//        Debug("[%s]", s.c_str());

        {
            const string from = "ͼ3 HTTPSͨ��ʱ��ͼ.png";
            string to = UrlCode(from);

            Debug("[%s]", to.c_str());
        }
        {
            const string from = "ͼ1 �ԳƼ���.png";
            string to = UrlCode(from);

            Debug("[%s]", to.c_str());
        }
        {
            const string from = "ͼ2 �ǶԳƼ���.png";
            string to = UrlCode(from);

            Debug("[%s]", to.c_str());
        }

    #endif
    }

    {// ���� Conf �໥��ֵ������    [2010-05-10 16:20:50]
    #if 0


        Conf pack4;
        {
            Conf pack1;

            pack1.Set("a1", "1111");
            pack1.Set("a2", "2222222");
            pack1.Set("a3", "333");
            pack1.Set("a4", "44444444444444");

            {
                Conf::iterator it(&pack1);
                while( it.next() )
                {
                    Debug("%s=%s", it.GetName(), it.GetValue());
                }
            }

            Debug("------------------------------");

            Conf pack2;

            pack2.Set("b1", "777");
            pack2.Set("a2", "88");  // ԭkey��Ӧֵ����ͬ����������
            pack2.Set("b3", "9999999");

            {
                Conf::iterator it(&pack2);
                while( it.next() )
                {
                    Debug("%s=%s", it.GetName(), it.GetValue());
                }
            }

            Debug("------------------------------");

            Conf pack3;

            pack3.Set("ip", "1.2.3.4");
            pack3.Set(pack1);
            pack3.Set(pack2);// ԭ"a2"��Ӧֵ��������


            {
                Conf::iterator it(&pack3);
                while( it.next() )
                {
                    Debug("%s=%s", it.GetName(), it.GetValue());
                }
            }


            pack4.Set(pack1);
            pack4.Set(pack2);
            pack4.Set(pack3);
        }

        {Debug("------------------------------");
            Conf::iterator it(&pack4);
            while( it.next() )
            {
                Debug("%s=%s", it.GetName(), it.GetValue());
            }
        }

    #endif
    }

    {// ���� Encrypt
    #if 0

//        Encrypt data("123");
//
//        string s = "abcdefghijklmnopqrstuvwxyz�й�0123456789[!@#$%^&*()_+|~<>?;:'\\/><()[]{}\"]";
//
//        string s1 = data.encrypt(s);
//
//        string s2 = data.decrypt(s1);
//
//        Debug("%s", s.c_str());
//        Debug("%s", s1.c_str());
//        Debug("%s", s2.c_str());

        {
            string text = "9D888CA9AC95EBD4EABEDCB2E0A49284CDCA81F49695D8CFC2909DF0F694DA8085EB9987C8D5";
            string s2 = Encrypt("123").decrypt( text );

            Debug("%s", s2.c_str());
        }



    #endif
    }

    {// ���� Encrypt 2
    #if 0

        Encrypt data("123");

        string s = "�˵�һ������ȼ��Ҳ���ܸ��࣬�Ҳ��ܸ��࣬��Ը��ȼ�������� ���� ��˹�����˹��";

        string s1 = Encrypt("123").encrypt(s); // data.encrypt(s);

        string s2 = Encrypt("123").decrypt(s1);

        Debug("%s", s.c_str());
        Debug("%s", s1.c_str());
        Debug("%s", s2.c_str());





    #endif
    }

    {// ���� Join
    #if 0
        vector<string> v;

        v.push_back("111");
        v.push_back("222");
        v.push_back("333");

        Debug("[%s]", Join(v).c_str());
        Debug("[%s]", Join(v, ":").c_str());
        Debug("[%s]", Join(v, "] [").c_str());


    #endif
    }

    {// ���� XXX
    #if 0

        string s = ChineseCoding::Converter(argv[1], argv[2], argv[3]);

        Debug("[%s]", s.c_str());

        //string s2 = ChineseCoding::Converter(argv[1], "gbk", "utf-8");

        //Debug("[%s]", s2.c_str());





    #endif
    }

    {// ���� XXX
    #if 0
        sleep(1);

        Ini pack;
        pack.Set("20100515020544", "title", "");
        pack.Set("20100515020544", "text", "");
        pack.Set("20100515020544", "password", "");

        UserData user("rocky");

        user.Get( pack ); // ��ʱ�����title��text��

        pack.Dump();

        LOG_DEBUG("------------------------");

        Ini all;
        all.Set("20100515020544", Conf());
        user.Get( all ); // ������������
        all.Dump();

        LOG_DEBUG("------------------------");

        {
        Ini all;
        all.Set("20100515020511", Conf());
        user.Get( all ); // ������������
        all.Dump();
        }

        LOG_DEBUG("------------------------");

    #endif
    }

    {// ���� XXX
    #if 0

        FileObj file(argv[1]);
        ChineseCoding code(argv[2], argv[3]);

        char *p = new char[ file.Size() + 1 ];

        int ret = file.Read(p, file.Size());

        p[ret] = '\0';

        const string from = p;
        string to;

        code.Converter(from, to);

        LOG_DEBUG("---------------------------");
        LOG_DEBUG("[%s]", from.c_str());
        LOG_DEBUG("---------------------------");
        LOG_DEBUG("[%s]", to.c_str());
        LOG_DEBUG("---------------------------");



    #endif
    }

    {// ���� PageCfg
    #if 0

        PageCfg cfg("rocky2", "browse.txt");

        string s1 = cfg.Get("filter", argv[1]?:"query_keyword");
        Debug("[%s]", s1.c_str());

        cfg.Save();

    #endif
    }

    {// ���� UserData::GetAttachList        [Rocky 2010-05-18 17:52:24]
    #if 0

        UserData user("rocky");

        // �г���������
        vector<string> list;
        vector<string>::iterator it;
        user.GetAttachList("20100129130823", list);
        Debug("[%d]", list.size());
        for(it = list.begin(); list.end() != it; it++)
        {
            Debug("[%s]", (*it).c_str());
        }


    #endif
    }

    {// ���� DataPack
    #if 0
        if(1 == argc)   // ���
        {
            DataPack pack("rocky");

            pack.MakePack();

            Ini data;

            data.Set("20100408133333", "title", "�л����񹲺͹�");

            //pack.Add(data);
            pack.AddAll();

            FileObj *file = pack.GetPack();
            Debug("[%s]", file->Fullname().c_str());
        }
        else    // ���
        {
            const string filename = argv[1];

            Debug("[%s]", filename.c_str());

            DataPack unpack("rocky");

            unpack.UnPack(filename);
        }

    #endif
    }

    {// ���� SettFileModifyTime
    #if 0

        long sec = atol(argv[2]?:"1274079728");
        string filename = argv[1];
        int ret;

        ret = SettFileModifyTime(argv[1], sec);

        Debug("[%d] [%ld]", ret, sec);


//        {
//            struct utimbuf t;
//            t.modtime = sec;
//            t.actime  = sec + 1;
//
//            ret = utime(filename.c_str(), &t);
//
//            Debug("[%d] [%ld]", ret, sec);
//        }


    #endif
    }

    {// ���� ZipCompress() ZipDecompress()
    #if 0
        int ret;

        if('1' == argv[1][0])
        {
            ret = ZipCompress(argv[2], argv[3]);
            Debug("%d", ret);
        }
        else if('2' == argv[1][0])
        {
            ret = ZipDecompress(argv[2], argv[3]);
            Debug("%d", ret);
        }



    #endif
    }

    {// ���� MultiStringMatch
    #if 0

        vector<string> from;
        vector<string> to;

        from.push_back("<");
        from.push_back(">");
        from.push_back("\n");
        from.push_back("��");

        to.push_back("[");
        to.push_back("]");
        to.push_back("\\n");
        to.push_back("WW");

        MultiStringMatch DelHtml(from ,to);

        string s = "abcde�й���--<-->--\n--";
        string s1;

        s1 = DelHtml.Replace(s);

        Debug("[%s]", s.c_str());
        Debug("[%s]", s1.c_str());


    #endif
    }

    {// ���� get password
    #if 0

        {
            Debug("-----------------------------------");
            const string answer = "ddd";
            string password = "123";

//            if("" != password)
//            {
//                password = Crypt(password, CRYPT_VERSION);
//            }

            string s1 = Encrypt(answer).encrypt( password );

            Debug("[%s]", answer.c_str());
            Debug("[%s]", password.c_str());
            Debug("[%s]", s1.c_str());
        }

        {
            Debug("-----------------------------------");
            const string &cfg_answer = "0265A3";
            const string &cli_answer = "ddd";
            string password = Encrypt(cli_answer).decrypt( cfg_answer );

            Debug("cfg_answer=[%s]", cfg_answer.c_str());
            Debug("cli_answer=[%s]", cli_answer.c_str());
            Debug("password=[%s]", password.c_str());

//            Debug("[%s]", password.c_str());
//            Debug("[%s]", Crypt(password, CRYPT_VERSION).c_str());

        }


    #endif
    }

    {// ���� Timer  ��Rocky 2010-05-25 14:43:44��
    #if 0

        struct doit
        {
            static void exe1(void *p)
            {
                Debug("--------[%s]------------", (char *)p);
            }
            static void exe2(void *p)
            {
                Debug("--------[%s]------------", (char *)p);
                Timer::UnRegister(exe2);
            }
            static void exe3(void *p)
            {
                Debug("--------[%s]------------", (char *)p);
            }

        };

        sleep(3);

        LOG_DEBUG("****************** begin *******************************");

        char s[] = "aaaaaa";

        Timer::Register(doit::exe1, s, "exe1");
        Timer::Register(doit::exe2, s, "exe2");
        Timer::Register(doit::exe3, s, "exe3");


        sleep(35);

        LOG_DEBUG("****************** end *******************************");

    #endif
    }

    {// ���� StringFilter
    #if 0

//        {
//            int ret;
//            StringFilter filter;
//
//            const string from = "(.*)<div><img src=\"/cgi-bin/image.cgi\\?key=([0-9]+)&amp;operator=out&amp;img=([^\"]+)\" title=\"([^\"]+)\" id=\"([^\"]+)\"></div>(.*)";
//            const string to = "$1<div><img src=\"/image?key=$2&amp;file=$3\" title=\"$4\" id=\"$5\"></div>$6";
//            // const string to = "<div><img src=\"/image?key=$1&amp;file=$2\" title=\"$3\" id=\"$4\"></div>";
//            // const string regTo = "$1|$2|$3|$4";
//
//            ret = filter.Regcomp(from.c_str(), to.c_str());
//            Debug("[%d]", ret);
//
//            const string src = "123<div><img src=\"/cgi-bin/image.cgi?key=20100525164311&amp;operator=out&amp;img=logo.gif.png\" title=\"logo.gif.png\" id=\"img_v3_20100525164356\"></div>345";
//            char buf[1024] = "";
//
//            ret = filter.ChangTo(src.c_str(), buf);
//
//            Debug("[%d]", ret);
//            Debug("%s", src.c_str());
//            Debug("%s", buf);
//        }

        {
            int ret;
            StringFilter filter;

            const string from = "([0-9]*)";
            const string to = "ABC";


            ret = filter.Regcomp(from.c_str(), to.c_str());
            Debug("[%d]", ret);

            const string src = "------123----45----7890-";
            char buf[1024] = "";

            ret = filter.ChangTo(src.c_str(), buf);

            Debug("[%d]", ret);
            Debug("%s", src.c_str());
            Debug("%s", buf);
        }

    #endif
    }

    {// ���� MergeBookmark.pl   [Rocky 2010-05-26 15:42:37]
    #if 0

        if(argc < 3)
        {
            Debug("argv error");
            return 1;
        }

        struct TMP
        {
            static const string MergeBookmark(const string &bookmark1, const string &bookmark2)
            {
                struct AutoDelete
                {
                    AutoDelete(const string &file1, const string &file2, const string &file3)
                        : m_file1(file1), m_file2(file2), m_file3(file3)
                    {
                    }
                    ~AutoDelete()
                    {
                        Debug("Delete file: [%s] [%s] [%s]",
                                m_file1.c_str(), m_file2.c_str(), m_file3.c_str());
                        DeleteFile(m_file1);
                        DeleteFile(m_file2);
                        DeleteFile(m_file3);
                    }
                private:
                    const string &m_file1;
                    const string &m_file2;
                    const string &m_file3;
                };
                const string tmpfile1 = StringToTempFile(bookmark1);
                const string tmpfile2 = StringToTempFile(bookmark2);
                const string tmpfile3 = StringToTempFile("");
                AutoDelete tmp(tmpfile1, tmpfile2, tmpfile3);

                Debug("[%s]", tmpfile1.c_str());
                Debug("[%s]", tmpfile2.c_str());
                Debug("[%s]", tmpfile3.c_str());

                // ���ⲿ�������ļ�
                const string cmd = "../tool/MergeBookmark.pl "
                                   "'" + tmpfile1 + "' "
                                   "'" + tmpfile2 + "' "
                                   "'" + tmpfile3 + "' ";
                int ret = system(cmd.c_str());
                LOG_DEBUG("ret=[%d] cmd=[%s]", ret, cmd.c_str());
                if(0 != ret)
                {
                    LOG_ERROR("Command error: [%s]", cmd.c_str());
                    return "";
                }

                return FileToStr(tmpfile3);
            }
        };

        const string &bookmark1 = argv[1];
        const string &bookmark2 = argv[2];
        const string bookmark3 = TMP::MergeBookmark(bookmark1, bookmark2);

        Debug("---------------------------------------------");
        Debug("[%s]", bookmark1.c_str());
        Debug("---------------------------------------------");
        Debug("[%s]", bookmark2.c_str());
        Debug("---------------------------------------------");
        Debug("[%s]", bookmark3.c_str());



    #endif
    }

    {// ���� XXX
    #if 0

        const string query_keyword = "����";

        map<string, MultiStringMatch *> matchs;
        vector<string> query_keyword_set;
        query_keyword_set.push_back(query_keyword);
        Debug("[%s]", Join(query_keyword_set, ",").c_str());
        matchs["query_keyword"] = new MultiStringMatch( query_keyword_set );

        // ȡ��Ӧƥ���
        MultiStringMatch *match = &abc;// matchs["query_keyword"];
        const string &text = "�л����񹲺͹������л����񹲺͹�";
        if("" != text && NULL != match)
        {
            // �鿴text���Ƿ�����Ҫ�Ĺؼ��֣�����ķ���false����ʾҪ���˴����ݣ�
            if( !match->MatchOneKey( text ) )
            {
                Debug("Filter ...");
                return 4;
            }
        }

        sleep(1);

    #endif
    }

    {// ���� MultiStringMatch   [Rocky 2010-06-01 12:18:02]
    #if 0

    {
//        vector<string> from;
//        vector<string> to;
//        from.push_back("<");
//        from.push_back(">");
//        from.push_back("\n");
//        from.push_back("����");
//        to.push_back("[");
//        to.push_back("]");
//        to.push_back("\\n");
//        to.push_back("����");
//        MultiStringMatch DelHtml(from ,to);
//
//        string s = "�л����񹲺͹������л����񹲺͹�";
//        string s1;
//
//        s1 = DelHtml.Replace(s);
//
//        Debug("[%s]", s.c_str());
//        Debug("[%s]", s1.c_str());
    }
    Debug("----------------------------------------");
    {
        vector<string> from;
        vector<string> to;
        from.push_back("�л�");
        from.push_back("����");
        MultiStringMatch DelHtml(from ,to);

        string s = "�л����񹲺͹��� ���л����񹲺͹�";

        Debug("one=[%d]", DelHtml.MatchOneKey(s));
        Debug("all=[%d]", DelHtml.MatchAllKey(s));
        Debug("-------");
        Debug("one=[%d]", DelHtml.MatchOneKey(""));
        Debug("all=[%d]", DelHtml.MatchAllKey(""));
    }

    #endif
    }

    {// ���� ���ߣ��ļ���תΪurl���뷽ʽ��[Rocky 2010-06-04 16:09:49]
    #if 0


        bool bToUrl;

        if(argc>1 && '1' == argv[1][0])
        {
            bToUrl = true;
        }
        else if(argc>1 && '2' == argv[1][0])
        {
            bToUrl = false;
        }
        else
        {
            printf("Usage: filename2url {1|2}\n"
                   " 1: filename to url\n"
                   " 2: url to filename\n"
                  );
            return 1;
        }

        GetFileList list("./");

        GetFileList::file_iterator file(list);

        if(bToUrl)
        {
            while( file.next() )
            {
                char key[1024] = "";
                char dot = 'X';
                const string old = GetBaseName( file.name() );
                string now;
                int ret = sscanf(old.c_str(), "%[^.]%c", key, &dot);

                // �����ϸ�ʽ�Ĳ���ת��
                if(2 == ret && strlen(key) == 14 && '.' == dot)
                {
                    now = key + string(".") + UrlCode(old.substr(15));
                    printf("[%s] => [%s]\n", old.c_str(), now.c_str());
                    rename(old.c_str(), now.c_str());
                }
            }
        }
        else
        {
            while( file.next() )
            {
                const string old = GetBaseName( file.name() );
                const string now = UrlDecode(old);
                printf("[%s] => [%s]\n", old.c_str(), now.c_str());
                rename(old.c_str(), now.c_str());
            }
        }

    #endif
    }

    {// ���� XXX
    #if 0

        string s = argv[1]?:"abc�й���123";

        string s1 = FilenameEncode(s);
        string s2 = FilenameDecode(s1);

        Debug("[%s]", s.c_str());
        Debug("[%s]", s1.c_str());
        Debug("[%s]", s2.c_str());

    #endif
    }

    {// ���� XXX
    #if 0



    #endif
    }

    {// ���� WebHtmlToText
    #if 1

        // string s = "s<span id=\"rk_v3_20100611191030\">sss&lt;papas-lt--mspss[pa]ssssAs</span>d";
        string s = argc>1?argv[1]:"---abc---def---d--e--f---ghi";
        string s1;



            vector<string> from;
            vector<string> to;

//            // -----------------
//            from.push_back("&amp;");
//            from.push_back("&amp");
//
//            to.push_back("&");
//            to.push_back("&");

            // -----------------
            from.push_back("&lt");
            from.push_back("&lt;");
            from.push_back("&gt");
            from.push_back("&gt;");
            from.push_back("&amp");
            from.push_back("&amp;");
            from.push_back("\n");
            from.push_back("\r");
            from.push_back("<");
            from.push_back(">");

            to.push_back("<");
            to.push_back("<");
            to.push_back(">");
            to.push_back(">");
            to.push_back("&");
            to.push_back("&");
            to.push_back("\x1");
            to.push_back("");
            to.push_back("\x2");
            to.push_back("\x3");

//            // -----------------
//            from.push_back("abc");
//            to.push_back("A");
//
//            from.push_back("def");
//            to.push_back("B");
//
//            from.push_back("gh");
//            to.push_back("C");

            MultiStringMatch match(from ,to);

        //s1 =WebHtmlToText(s);
        s1 = match.Replace(s);

        Debug("[%s]", s.c_str());
        Debug("[%s]", s1.c_str());

    #endif
    }


    //sleep(2);
    return 0;
}





