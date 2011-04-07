/**********************************************************************
*                                                                     *
*   功能 : 登录窗口显示等处理                                         *
*   编写 : Rocky 2010-05-29 17:09:37                                  *
*   mail : mypyramid@126.com                                          *
*   修改、使用方法见本文件底说明                                      *
*                                                                     *
**********************************************************************/






function LoginWin()
{
    var This = (window == this) ? LoginWin : this;
    var win;
    var timer;
    var interval = 10000; // 每10秒做一次检测 [XXX]
    var err_msg_id = "LOGIN_WIN375359086234"; 
    var bOpen = false;


    /****************** 外部接口 ******************/

    // 打开
    This.Open = function()
    {
        // 已打开，则直接返回；
        if(bOpen)
        {
            return;
        }
        ShortKey.LockAll();
        OverLayer.Open();
        ShowToCenter.MoveToCenter( win.id ); // 居中
        OpenWin(win);
        SetFocus();
        //ClearInterval(timer);   // 关闭定时器（见[XXX:34438285]处说明）
        bOpen = true;
    }

    // 关闭
    This.Close = function()
    {
        // 已关闭，则直接返回；
        if(! bOpen)
        {
            return;
        }
        ShortKey.Unlock();
        OverLayer.Close();
        CloseWin(win);
        //timer = SetInterval(Ping, interval); // 重启定时器（见[XXX:34438285]处说明）
        bOpen = false;
    }

    // 发送登录请求（实际上本接口只在内部使用）
    This.Submit = function(forms)
    {
        var url = "login";

        /* 组装Post参数
         */
        var post = "";
        post += "username=" + encodeURIComponent( $('user').value );
        post += "&password=" + encodeURIComponent( forms["password"].value );
        post += "&login=1";

        // 发送请求到服务端
        SubmitToServer(url, post, Response); 
        // 处理回应
        function Response(str)
        {
            if("OK" == str || "" == str)
            {
                /* 验证通过，关闭登录窗口；
                 * 当用户打开了多个窗口，只要在一个窗口中输入验证码正确，则其它
                 * 窗口的验证码输入窗口则要自动关闭；[XXX:34438285]
                 */
                This.Close();
            }
            else
            {
                // 错误提示
                $(err_msg_id).innerHTML = str;
            }
        }
    }


    /****************** 内部处理函数 ******************/
    function Init()
    {
        if( !Create() )
        {
            alert("创建窗口出错");
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
        tDiv.id = "LOGIN_WIN48583945394294"; // 使用一随机id，因为有些操作需要用到id；
        tDiv.innerHTML = ""
            + "<div>"
            + "<fieldset style='width:291px;'><legend>重新登录</legend>"
            + "<center>"
            + "<form>"
            + "<br>"
            + "密码：<input name=password id=" + pasd_id + " type=password style='width:154px;'"
            + "         onkeydown=\"$('" + err_msg_id + "').innerHTML='';\">"
            + "      <input type=submit class='TABTO_" + pasd_id + "' value='确定' onclick='LoginWin.Submit(this.form); return false;'>"
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

    // 设置焦点到输入框
    function SetFocus()
    {
        // 密码输入框
        $(win.id).getElementsByTagName('input')[0].focus();
    }

    // 检测当前登录是否乃有效  [Rocky 2010-05-29 18:27:54]
    function Ping()
    {
        var url = "ping";
        var user = $('user');

        if(null == user)
        {
            return;
        }
        
        /* 组装Post参数
         */
        var post = "username=" + user.value;

        // 发送请求到服务端
        SubmitToServer(url, post, Response); 
        // 处理回应
        function Response(str)
        {
            if("OK" == str)
            {
                This.Close();
            }
            else
            {
                // 显示提示
                $(err_msg_id).innerHTML = str;
                // 打开重新登录窗口 
                This.Open();
            }
        }
    }

    // 执行初始化
    if( ! Init() )
    {
        return false;
    }

    // 非登录页，则启动登录有效检测；
    if( location.href.indexOf('/login') == -1
        && location.href.indexOf('/register') == -1
      )
    {
        // 启动定时器
        timer = SetInterval(Ping, interval);   
    }

    return true;
}








/**********************************************************************
使用方法：

html:


History:

 2010-05-29: 0.1版

**********************************************************************/
