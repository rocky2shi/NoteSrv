/**********************************************************************
*                                                                     *
*   ���� : ��ǩ����                                                   *
*   ��д : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   ���� : 2009-08-10                                                 *
*   �汾 : 0.9.2                                                      *
*   �޸ġ�ʹ�÷��������ļ���˵��                                      *
*                                                                     *
**********************************************************************/



/*
 * form_data_id: �����ύ���ݵ�form
 * type: ��ǩ���ͣ�1. "anchor" -- ����message.cgi��
 *                 2. "coord" -- ����browse.cgi��
 */
function Bookmark(form_data_id, type)
{
    /*
     * ���ݳ�Ա
     */
    var m_mark;                         // ������ǩ����
    var m_data = GetObj(form_data_id);  // ��ǩ�ύ�����õ��ı��ؼ�
    var m_ary = [];                     // ��ǩ��������
    var m_cur_index = 0;                // ��ǰ��ǩ����
    var m_style_def = "color:#737373; cursor:pointer;";
    var m_type = type || "anchor";


    /*
     * [2009-08-18 00:26:12]
     * Ϊ����Ӧ������ǩ��������message.cgi��browse.cgi�У�
     *      message.cgi: ��aê������¼��ת
     *      browse.cgi: ���ĵ�λ����������¼��ת
     *   ������m_fun��������������"anchor"��"coord"�������е�һ
     *   ��function������װ���ݣ��ڶ�������ִ����ת��
     */
    var m_fun = {"anchor" : [
                    // ��װ���ݶ���
                    function(key, title)
                    {
                        return {"key":key, "title":title, "create":GetKeyStr()};
                    },
                    // ִ�ж�Ӧ����ת����
                    function(index)
                    {
                    }
                 ],
                 "coord" : [
                    function(key)
                    {
                        // ȡ�ĵ���ѡ���ı���Ϊ��ǩ˵��
                        var str = GetSelectText() || "[��]";
                        var LEN = 280;
                        
                        if(str.length > LEN)
                        {
                            var n = str.length - LEN;
                            // Rocky 2010-01-14 13:50:37
                            str = str.substr(0, LEN) 
                                + "<span title='" + str.substr(LEN) + "'>......"
                                + "  [<span class='hand blue' onclick={this.parentNode.innerHTML=this.parentNode.title;}>+</span>]"
                                + "</span>";
                        }

                        str = str.replace(/\s+/g, " ");
                        

                        // ��װΪ����
                        // �ظ�key��y����Ϊ�˼�����ǰ���ݣ�
                        var data = {"key":key, "title":str, "create":GetKeyStr()};
                        return data;
                    },
                    function(index)
                    {
                    }
                 ]
    }



    /*
     * �ⲿ�ӿ�
     */
    Bookmark.Set = function(key, title)
    {
        var data = GetDataObj(key, title);

        if( exist(data) )
        {
            window.status = "���Ѵ��ڣ�" + data.key;
            return;
        }

        // ������ǩ
        Insert(data);

        // �ղ�����ǩ�Ƶ���λ
        MoveTo(m_ary.length - 1, 0);

        // ����Ϊ��ǰ
        SetCur(0);

        // ˢ��
        Syn();

        // �ύ��������
        if('coord' != m_type)
        {
            Submit(); // [Rocky 2009-12-09 21:42:17]
        }
    }

    // ������ǩ��ǰ��ť
    Bookmark.Flash = function()
    {
        // �����ⲿ��ʱ������
        // SetTimeout(Flash, 1000);

        var button = $(m_cur_index);
        if(!button || button.over)
        {
            return;
        }
        button.flash = !button.flash;
        if(button.flash)
        {
            SetStyleFlash(button);
        }
        else
        {
            SetStyleDef(button);
        }
    }

    // ������ǩ[2009-08-20 00:55:43]
    Bookmark.Find = function(key)
    {
        var i;
        for(i=0; i<m_ary.length; i++)
        {
            if(key == m_ary[i].key)
            {
                SetCur(i);
                return;
            }
        }
    }


    /*
     * �ڲ�����
     */
    // ȡ������Ӧ�ĵĶ���ǩ
    function $(index)
    {
        var span = m_mark.getElementsByTagName("span");
        var li = span[0].getElementsByTagName("li");

        return li[index];
    }

    // ��ʼ��
    function Init()
    {
        // ������ǩ����
        CreateUI();

        // ��ʾ����
        Tips();

        var ary = ToArray(m_data.value);
        for(var i=0; i<ary.length; i++)
        {
            Insert(ary[i]);
        }

        // ��ʼλ��
        AdjustPos();

        // ���ù������ڵĶ���
        InsertFun(window, "scroll", AdjustPos);

        // �ı䴰�ڴ�Сʱ
        InsertFun(window, "resize", AdjustPos);

        var li = m_mark.getElementsByTagName('li');

        // �ϼ�ͷ��<li>�е�һ��Ԫ�أ�
        var up = li[0];
        SetStyleDef(up);
        up.onclick = function(event)
        {
            event = event || window.event;
            if(event.ctrlKey)
            {
                MoveToPrev();
            }
            else
            {
                // ����ǰһ��
                Prev();
            }
        }
        up.onmouseover = function()
        {
            SetStyleMouseOver(this);
        }
        up.onmouseout = function()
        {
            SetStyleMouseOut(this);
            Tips.Close();
        }

        // �¼�ͷ��<li>�����һ��Ԫ�أ�
        var down = li[li.length - 1];
        SetStyleDef(down);
        down.onclick = function(event)
        {
            event = event || window.event;
            if(event.ctrlKey)
            {
                // �ƶ�ǰ��ת
                MoveToNext();
            }
            else
            {
                // ������һ����ǩ��
                Next();
            }
        }
        down.onmouseover = up.onmouseover;
        down.onmouseout = up.onmouseout;
    }

    // ������ǩ
    function Insert(ary)
    {
        // ������ǩ��ť
        var obj = Create();

        // ���÷��
        SetStyleDef(obj);

        // ���ö���
        SetEvent(obj);

        // �������
        m_ary[ m_ary.length ] = ary;

        return true;
    }

    // ������ǩ����
    function CreateUI()
    {
        var body = document.getElementsByTagName('body')[0];
        var ul = document.createElement("ul");

        // ��ǩ��ť
        ul.innerHTML = "<li>��</li><span></span><li>��</li>";
        ul.style.cssText = "position:absolute; visibility:hidden; "
                           + "top:0px; left:0px; "
                           + "margin:0px; padding:0px; color:#737373; "
                           + "cursor:pointer;";
        body.appendChild(ul);
        m_mark = ul;
    }

    // �ڽ����ϴ�����ǩ�������ض�������
    function Create()
    {
        var span = m_mark.getElementsByTagName("span")[0];
        var li = span.getElementsByTagName("li");
        var button = document.createElement("li");

        button.innerHTML = "��";
        button.index = li.length;   // ��ǰ��ť���
        button.flash = false;       // �������
        // button.title = '����ת�����һ�ɾ����ǩ��';

        // ����
        span.appendChild(button);

        return button;
    }

    // ��ʾ����
    function Tips()
    {
        var win;
        var timer;

        // ����
        function Create()
        {
            var body = document.getElementsByTagName('body')[0];
            var div = document.createElement("div");

            div.innerHTML = "";
            div.style.cssText = "position:absolute; visibility:hidden; display:block;"
                                + "top:0px; left:0px; width:238px; padding:4px;"
                                + "background-color:#C3C3C3; color:#075F40; "
                                + "filter:progid:DXImageTransform.Microsoft.Alpha(style=0,opacity=80,finishOpacity=100); "
                                + "opacity: 0.80; ";
            div.onmouseover = function()
            {
                ClearTimeout(timer);
            }
            div.onmouseout = function()
            {
                Tips.Close();
            }
            body.appendChild(div);
            win = div;
        }

        function DoClose()
        {
            // win.style.visibility = "hidden";
            CloseWin(win);
        }

        // ��
        Tips.Open = function(obj, key, title, time)
        {
            ClearTimeout(timer);
            var pos = GetPos( obj );
            var html = "<B>"
                     + "[" + key + "]"
                     + (time ? ("[" + time + "]") : "")
                     + "</B><br>"
                     + "&nbsp;&nbsp;" + title;
            win.innerHTML = html;
            win.style.left = pos.x + pos.w + 10 + 'px';
            win.style.top = pos.y + 6 + 'px';
            // win.style.visibility = "visible";
            OpenWin(win);
        }

        // �ر�
        Tips.Close = function()
        {
            // ��ʱ�ر�
            timer = SetTimeout(DoClose, 100);
        }

        // ��ʼ��
        Create();
    }

    // ������ǰ��ǩ��Ӧ������
    function Indication(obj)
    {
        if(null == obj || obj.lock)
        {
            return;
        }
        var bgcolor = obj.style.background;     // ԭ��ɫ
        var cssText = obj.style.cssText;        // ԭ���
        var count = 0;

        obj.lock = true;    // ������������������
        Do();

        function Do()
        {
            if(count > 2)   // �����κ󷵻�ԭ
            {
                obj.lock = false;   // ����
                obj.style.cssText = cssText;    // ��ԭ
                return;
            }
            // ����
            obj.style.background = (count%2 == 0) ? '#939393' : bgcolor;
            count++;
            SetTimeout(Do, 500);
        }
    }

    // ��ǩ�Ѵ��ڣ�����true�����򷵻�false;
    function exist(data)
    {
        var i;
        for(i=0; i<m_ary.length; i++)
        {
            if(m_ary[i].key == data.key)
            {
                return true;
            }
        }
        return false;
    }

    // ɾ�������ϵ���ǩ
    function Remove(index)
    {
        var span = m_mark.getElementsByTagName("span")[0];
        var li = span.getElementsByTagName("li");
        // ɾ��
        span.removeChild( $(index) );
        // ��������
        for(var i=0; i<li.length; i++)
        {
            li[i].index = i;
        }
    }

    // Ĭ�Ϸ��
    function SetStyleDef(obj)
    {
        obj.style.cssText = m_style_def;
    }

    // ��ǰ��ǩ���
    function SetStyleCur(obj)
    {
        obj.style.cssText = "#FA8072";
    }

    // ����Ƴ�
    function SetStyleMouseOut(obj)
    {
        obj.style.color = "#737373";
    }

    // ����ƽ�
    function SetStyleMouseOver(obj)
    {
        obj.style.color = "#FA8072";
    }

    // ����
    function SetStyleFlash(obj)
    {
        obj.style.color = "#8C8A8E";
    }

    // �����¼�
    function SetEvent(obj)
    {
        obj.onclick = function(event)
        {
            event = event || window.event;
            // �޸�
            if(event.ctrlKey)
            {
                Modify(this.index);
                return;
            }
            // �������ǩ��Ϊ��ǰ��ǩ
            SetCur(this.index);
            Tips.Close();
        }
        obj.onmouseover = function()
        {
            this.over = true;
            SetStyleMouseOver(this);
            Tips.Open( this,
                       (this.index + 1) + "/" + m_ary.length,
                       m_ary[this.index].title,
                       TimeTo(m_ary[this.index].create, 2) );
        }
        obj.onmouseout = function()
        {
            this.over = false;
            SetStyleMouseOut(this);
            Tips.Close();
        }
        obj.oncontextmenu = function(event)
        {
            event = event || window.event;
            EventStop(event);   // ��ֹ�¼�����
            // ɾ����ǰ��ǩ
            Del(this.index);
            return false;
        }
    }

    // ����Ϊ��ǰ��ǩ
    function SetCur(index)
    {
        if(index < 0 || index >= m_ary.length)
        {
            return;
        }
        SetStyleDef( $(m_cur_index) );
        m_cur_index = index;
        JumpTo(index);
        Indication( GetObj(m_ary[index].key) ); // ����
    }

    // ����ǰһ��ǩ
    function Prev()
    {
        SetCur(m_cur_index - 1);
    }

    // ������һ��ǩ
    function Next()
    {
        SetCur(m_cur_index + 1);
    }

    // ��ǰ��ǩǰ��һ��λ��
    function MoveToPrev()
    {
        // �ƶ��ڲ�����
        MoveTo(m_cur_index - 1, m_cur_index);
        // �ƶ���������ǩ��ť
        Prev();
        // ˢ��
        Syn();
        // �ύ
        Submit();
    }

    // ��ǰ��ǩ����һ��λ��
    function MoveToNext()
    {
        // �ƶ��ڲ�����
        MoveTo(m_cur_index, m_cur_index + 1);
        // �ƶ���������ǩ��ť
        Next();
        // ˢ��
        Syn();
        // �ύ
        Submit();
    }

    // ��ǰ��ǩ�Ƶ���n�������������ĵ������ƶ���
    function MoveTo(src, dest)
    {
        /*
         * ����src��dest��ǰ���ϵ��ѭ���ƶ�����src����Ԫ���Ƶ�
         * dest���������������Ƶ�һ��λ��
         */

        if(src < 0 || src >= m_ary.length || dest < 0 || dest >= m_ary.length)
        {
            return;
        }
        if(src < dest)
        {
            var i;
            var tmp = m_ary[src];
            for(i=src; i<dest; i++)
            {
                m_ary[i] = m_ary[i+1];
            }
            m_ary[i] = tmp;
        }
        else if(src > dest)
        {
            var i;
            var tmp = m_ary[src];
            for(i=src; i>dest; i--)
            {
                m_ary[i] = m_ary[i-1];
            }
            m_ary[i] = tmp;
        }
    }

    // ��ת
    function JumpTo(index)
    {
        /*
         * 2009-08-20 00:36:44
         */

        // ������ǰ������Ϊ��ǩ��ǵ�����
        if(m_ary[index].y)
        {
            window.scrollTo(0, m_ary[index].y);
            return;
        }

        // �������в����<span>Ϊ��ǣ�ֱ�ӵ�������Ĵ��룩
        var e = GetObj(m_ary[index].key);
        if(null == e)
        {
            return;
        }
        var a = GetPos(m_mark);     // ��ǩ����
        var b = GetPos(e);          // ��ǩ��Ӧ���ļ�¼����
        var middle = a.y + a.h / 2; // �൱����Ļ�м�
        var center = b.y + b.h / 2; // ���ı�ǵ��м�
        window.scrollBy(0, center - middle);
    }

    // ȡ���ݶ���
    function GetDataObj(key, title)
    {
        return m_fun[m_type][0](key, title);
    }

    // �Ӵ�������
    function ToArray(str)
    {
        var ary = [];
        if("" == str)
        {
            str = "[]";
        }
        try     // ȷ������������ʱjsҲ�ǿ��õġ� [2008-11-06 22:13:10]
        {
            eval("ary=" + str);
        }
        catch(e)
        {
            ary = [];
            alert('��ǩ���ݳ��������ǩ��');
        }
        return ary;
    }

    // �����鵽��
    function ToStr(ary)
    {
        var i, e;
        var str="[";
        var s = '';
        for(i in ary)
        {
            e=ary[i];
            str += '{';
            for(var o in e)
            {
                // ת��\        [2008-11-06 22:07:49]
                s = (e[o] + "").replace(/\\/g, "\\\\");
                // ת��'
                s = s.replace(/'/g, "\\'");
                str += "'" + o + "':'" + s + "',";
            }
            str = str.replace(/,$/, '');    // ȥ�����ġ�,��
            str += "},";
        }

        str = str.replace(/,$/, '');    // ȥ�����ġ�,��
        str += "]";

        return str;
    }

    // ͬ����ǩ���ύform����
    function Syn()
    {
        m_data.value = ToStr(m_ary);
        m_ary = ToArray(m_data.value);
    }

    // ����λ�ã���λ����ࣩ
    function AdjustPos()
    {
        var nWinHeight = document.body.clientHeight;
        var nObjHeight = m_mark.offsetHeight;
        var nTop = (nWinHeight - nObjHeight) /2 + document.body.scrollTop;
        m_mark.style.top = nTop + 'px';
        m_mark.style.visibility = "visible";
    }


    // �޸ĵ�ǰ��ǩ��ע
    function Modify(index)
    {
        if(!m_ary || !m_ary[index])
        {
            return;
        }
        var str = window.prompt("�޸ĵ�ǰ��ǩ[" + (index + 1) + "]��ע���������µ�˵����", m_ary[index].title);
        if(null == str)
        {
            return;
        }
        // �޸�
        m_ary[index].title = str;
        // ˢ��
        Syn();
        // �ύ
        Submit();
    }

    // �����ǰ��ǩ
    function Del(index)
    {
        if(!m_ary || !m_ary[index])
        {
            return;
        }
        if(!window.confirm("ɾ�����Ϊ[" + (index + 1) + "]����ǩ��ȷ����"))
        {
            return;
        }
        // ɾ��
        delete m_ary[index];
        // ɾ�������ϵ�
        Remove(index);
        // ˢ��
        Syn();
        // ����0Ϊ��ǰ��ǩ
        // SetCur(0);
        // �ύ
        Submit();
    }

    // �ύ��������
    function Submit()
    {
        m_data.form.submit();
    }

    // ��ʼ��
    Init();
}










/**********************************************************************
ʹ�÷�����

html:

<!-- ��ǩ���� -->
<ul id="bookmark" style="position:absolute; visibility:hidden; top:200px; left:0px; width:1px; margin:0px; padding:0px; color:#737373; cursor:pointer;">
<li name="up">��</li>
<span></span>
<li>��</li>
</ul>

<!-- ��ǩ�ύ -->
<form method="POST" target="tmp_iframe" style="display:none">
<input name="key" value="20090730222406">
<textarea id="bookmark_data" name="bookmark_data" rows=10 cols=200></textarea>
<input name="bookmark_save" value="�ύ">
<input type="submit">
</form>


js��
        Bookmark()          -- ��ʼ��
        Bookmark.Set()      -- ������ǩ


History:

 2009-08-10 : 0.9��
 2009-12-08 : 0.9.1�森�޸��ύ���������ݲ�ͬ���ܵ���
 2009-12-09 : 0.9.2�森�ٴ��޸��ύ������
 2010-01-14 : 0.9.3��. �޸���ǩ�������ֵ���ʾ������Ӻ�ʱ����
**********************************************************************/