/**********************************************************************
*                                                                     *
*   功能 : 提示小窗口                                                 *
*   编写 : Rocky 2009-12-06 00:13:58                                  *
*   mail : mypyramid@126.com                                          *
*   修改、使用方法见本文件底说明                                      *
*                                                                     *
**********************************************************************/



// 提示窗口
function Tips()
{
    var win;
    var timer;
    var This = (window == this) ? Tips : this;

    /****************** 内部函数 ******************/

    // 创建
    function Create()
    {
        var body = document.getElementsByTagName('body')[0];
        var div = document.createElement("div");

        div.innerHTML = "";
        div.style.cssText = "position:absolute; visibility:hidden; display:block;"
                            + "paddings:4px; "
                            + "background-color:#C3C3C3; color:#075F40; "
                            + "filter:progid:DXImageTransform.Microsoft.Alpha(style=0,opacity=80,finishOpacity=100); "
                            + "opacity: 0.80; ";
        div.onmouseover = function()
        {
            ClearTimeout(timer);
        }
        div.onmouseout = function()
        {
            This.Close();
        }
        body.appendChild(div);
        win = div;
    }

    function DoClose()
    {
        var depth = 100;
        function SetOpacity()
        {
            Opacity(win, depth);
            depth -= 5;

            if(depth <= 0)
            {
                Hide();
                Opacity(win, 100);
                return;
            }
            SetTimeout(SetOpacity, 50);
        }
        function Hide()
        {
            win.style.visibility = "hidden";
        }

        // 如包部函数Opacity()未定义，就直接隐藏，否则渐隐；
        typeof Opacity == 'undefined' ? Hide() : SetOpacity();
    }

    /****************** 外部接口 ******************/

    // 打开
    This.Open = function(content)
    {
        ClearTimeout(timer);
        win.innerHTML = content;
        win.style.visibility = "visible";
        return This;
    }

    // 关闭（默认100毫秒）
    This.Close = function(msec)
    {
        // 延时关闭
        timer = SetTimeout(DoClose, msec||100);
    }

    // 取窗口句柄
    This.GetWin = function()
    {
        return win;
    }

    // 初始化
    Create();
}
















/**********************************************************************
使用方法：

html:


History:

 2009-12-06: 0.1版
 2010-01-13: 0.2版，1.简化类成没（修改取This的方法） 2.去掉width；
**********************************************************************/
