/**********************************************************************
*                                                                     *
*   ���� : ��¼������ʾ�ȴ���                                         *
*   ��д : Rocky 2010-05-29 17:09:37                                  *
*   mail : mypyramid@126.com                                          *
*   �޸ġ�ʹ�÷��������ļ���˵��                                      *
*                                                                     *
**********************************************************************/






function LoginWin()
{
    var This = (window == this) ? LoginWin : this;
    var win;
    var timer;
    var interval = 10000; // ÿ10����һ�μ�� [XXX]
    var err_msg_id = "LOGIN_WIN375359086234"; 
    var bOpen = false;


    /****************** �ⲿ�ӿ� ******************/

    // ��
    This.Open = function()
    {
        // �Ѵ򿪣���ֱ�ӷ��أ�
        if(bOpen)
        {
            return;
        }
        ShortKey.LockAll();
        OverLayer.Open();
        ShowToCenter.MoveToCenter( win.id ); // ����
        OpenWin(win);
        SetFocus();
        //ClearInterval(timer);   // �رն�ʱ������[XXX:34438285]��˵����
        bOpen = true;
    }

    // �ر�
    This.Close = function()
    {
        // �ѹرգ���ֱ�ӷ��أ�
        if(! bOpen)
        {
            return;
        }
        ShortKey.Unlock();
        OverLayer.Close();
        CloseWin(win);
        //timer = SetInterval(Ping, interval); // ������ʱ������[XXX:34438285]��˵����
        bOpen = false;
    }

    // ���͵�¼����ʵ���ϱ��ӿ�ֻ���ڲ�ʹ�ã�
    This.Submit = function(forms)
    {
        var url = "login";

        /* ��װPost����
         */
        var post = "";
        post += "username=" + encodeURIComponent( $('user').value );
        post += "&password=" + encodeURIComponent( forms["password"].value );
        post += "&login=1";

        // �������󵽷����
        SubmitToServer(url, post, Response); 
        // �����Ӧ
        function Response(str)
        {
            if("OK" == str || "" == str)
            {
                /* ��֤ͨ�����رյ�¼���ڣ�
                 * ���û����˶�����ڣ�ֻҪ��һ��������������֤����ȷ��������
                 * ���ڵ���֤�����봰����Ҫ�Զ��رգ�[XXX:34438285]
                 */
                This.Close();
            }
            else
            {
                // ������ʾ
                $(err_msg_id).innerHTML = str;
            }
        }
    }


    /****************** �ڲ������� ******************/
    function Init()
    {
        if( !Create() )
        {
            alert("�������ڳ���");
            return false;
        }
        return true;
    };

    function Create()
    {
        var body = document.getElementsByTagName("body");
        tDiv = document.createElement("div");

        if(body[0] == null)
        {
            return false;
        }

        var pasd_id = "LOGIN_WIN29348204823572834";
        tDiv.id = "LOGIN_WIN48583945394294"; // ʹ��һ���id����Ϊ��Щ������Ҫ�õ�id��
        tDiv.innerHTML = ""
            + "<div>"
            + "<fieldset style='width:291px;'><legend>���µ�¼</legend>"
            + "<center>"
            + "<form>"
            + "<br>"
            + "���룺<input name=password id=" + pasd_id + " type=password style='width:154px;'"
            + "         onkeydown=\"$('" + err_msg_id + "').innerHTML='';\">"
            + "      <input type=submit class='TABTO_" + pasd_id + "' value='ȷ��' onclick='LoginWin.Submit(this.form); return false;'>"
            + "</form>"
            + "</center>"
            + "</fieldset>"
            + "</div>"
            + "<font color=red id='" + err_msg_id + "'></font>"
            + "";
        tDiv.style.cssText = "position:absolute; visibility:hidden; display:block; z-index:80;"
                            + "top:0px; left:0px; padding:4px;"
                            + "background-color:#FFFFFF;"
                            + "";

        body[0].appendChild(tDiv);
        win = tDiv;

        return true;
    }

    // ���ý��㵽�����
    function SetFocus()
    {
        // ���������
        $(win.id).getElementsByTagName('input')[0].focus();
    }

    // ��⵱ǰ��¼�Ƿ�����Ч  [Rocky 2010-05-29 18:27:54]
    function Ping()
    {
        var url = "ping";
        var user = $('user');

        if(null == user)
        {
            return;
        }
        
        /* ��װPost����
         */
        var post = "username=" + user.value;

        // �������󵽷����
        SubmitToServer(url, post, Response); 
        // �����Ӧ
        function Response(str)
        {
            if("OK" == str)
            {
                This.Close();
            }
            else
            {
                // ��ʾ��ʾ
                $(err_msg_id).innerHTML = str;
                // �����µ�¼���� 
                This.Open();
            }
        }
    }

    // ִ�г�ʼ��
    if( ! Init() )
    {
        return false;
    }

    // �ǵ�¼ҳ����������¼��Ч��⣻
    if( location.href.indexOf('/login') == -1
        && location.href.indexOf('/register') == -1
      )
    {
        // ������ʱ��
        timer = SetInterval(Ping, interval);   
    }

    return true;
}








/**********************************************************************
ʹ�÷�����

html:


History:

 2010-05-29: 0.1��

**********************************************************************/
