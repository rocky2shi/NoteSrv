/**********************************************************************
*                                                                     *
*   ���� : ��ʾС����                                                 *
*   ��д : Rocky 2009-12-06 00:13:58                                  *
*   mail : mypyramid@126.com                                          *
*   �޸ġ�ʹ�÷��������ļ���˵��                                      *
*                                                                     *
**********************************************************************/



// ��ʾ����
function Tips()
{
    var win;
    var timer;
    var This = (window == this) ? Tips : this;

    /****************** �ڲ����� ******************/

    // ����
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

        // ���������Opacity()δ���壬��ֱ�����أ���������
        typeof Opacity == 'undefined' ? Hide() : SetOpacity();
    }

    /****************** �ⲿ�ӿ� ******************/

    // ��
    This.Open = function(content)
    {
        ClearTimeout(timer);
        win.innerHTML = content;
        win.style.visibility = "visible";
        return This;
    }

    // �رգ�Ĭ��100���룩
    This.Close = function(msec)
    {
        // ��ʱ�ر�
        timer = SetTimeout(DoClose, msec||100);
    }

    // ȡ���ھ��
    This.GetWin = function()
    {
        return win;
    }

    // ��ʼ��
    Create();
}
















/**********************************************************************
ʹ�÷�����

html:


History:

 2009-12-06: 0.1��
 2010-01-13: 0.2�棬1.�����û���޸�ȡThis�ķ����� 2.ȥ��width��
**********************************************************************/
