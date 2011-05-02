


/*
 *  ���ܣ�����ѡ������
 *  ��д��Rocky 2008-10-29 00:36:34
 *  ˵����
 *          TypeSelectMenu( {id:"input_type_select", show:'value'|'title'|'index'},
 *                          {id:"span_type_select", click:menu_click},
 *                          {id:"query_type"} );
 */
function TypeSelectMenu(head, menu, value)
{
    head.obj = $(head.id);
    menu.obj = $(menu.id);
    value.obj = $(value.id);
    var THIS = this;
    var timeout = "";

    // ����Ĭ��ֵ
    head.show = head.show || 'title';
    value.show = value.show || 'value';

    // ��¼��һ���˵���
    menu.obj.first_item = null;
    // ԭֵ
    head.obj.old_value = head.obj.value;
    value.obj.old_value = value.obj.value;

    // ����ӿ�
    //this.Open = Open;
    //this.Close = Close;
    //this.Show = Show;


    // ִ�г�ʼ��
    Init();

    /*
     *  ���ܣ���ʾ�ؼ���
     *        ע�⣬�ؼ������� .style.visibility ���õġ�
     */
    function Open()
    {
        // ��ÿ�δ򿪲˵�ʱ����Ҫ�Ƶ���ȷλ�ã���ΪĿ��λ�ÿ��ܻ�䶯����
        MoveTo();
        // ��ʾ
        menu.obj.style.visibility = "visible";
    }

    /*
     *  ���ܣ����ؿؼ���
     *        ע�⣬�ؼ������� .style.visibility ���õġ�
     */
    function Close()
    {
        if(IsOpen())
        {
            ClearBgColor();
            menu.obj.style.visibility = "hidden";
        }
    }

    /*
     *  ���ܣ����˵��Ƿ��Ѵ�
     *  ���أ�true-�Ѵ򿪣�false-δ�򿪣�
     */
    function IsOpen()
    {
        return menu.obj.style.visibility != "hidden";
    }


    /*
     *  ���ܣ�һ�ε�����ʾ���ٴε������ؿؼ���
     *        ע�⣬�ؼ������� .style.visibility ���õġ�
     */
    function Show()
    {
        IsOpen() ? Close() : Open();
    }

    /*
     *  ���ܣ��ѿؼ��Ƶ��ʵ�λ�ã�������ʾ��
     */
    function MoveTo()
    {
        var dest = GetPosition(head.id);
        if(null == dest)
        {
            return;
        }
        menu.obj.style.left = dest.x + 1 + 'px';
        menu.obj.style.top = dest.y + dest.h + 1 + 'px';
    }

    /*
     *  ���ܣ��ѿؼ�����ѡ��checkbox����Ŀ�ĵ�value��
     */
    function CopyTo()
    {
        var chks = menu.obj.getElementsByTagName("input");
        var chk = null;
        var menu_chk = {};  // ��¼�˵���ѡ�е���

        menu_chk.value = '';    // ֵ[value]
        menu_chk.title = '';    // ����[title]
        menu_chk.index = '';    // ���

        for(var i=0; (chk=chks[i]); i++)
        {
            if(chk.checked)
            {
                menu_chk.value += chk.value ? (chk.value + ',') : '';
                menu_chk.title += chk.title ? (chk.title + ',') : '';
                menu_chk.index += (i+1) + ',';
            }
        }
        // ȥ�����ġ�,��
        menu_chk.value = menu_chk.value.slice(0, -1);
        menu_chk.title = menu_chk.title.slice(0, -1);
        menu_chk.index = menu_chk.index ? (head.obj.old_value + "[" + menu_chk.index.slice(0, -1) + "]") : "";

        // ��ʾ
        head.obj.value = menu_chk[(head.show)] || head.obj.old_value;
        // Ҫ�ύ��ֵ
        value.obj.value = menu_chk[(value.show)] || value.obj.old_value;
    }

    // �ر�ʱ������в˵�ѡ���ϵķ�����ʾ
    function ClearBgColor()
    {
        var items = menu.obj.getElementsByTagName('label');
        for(var o='',i=0; (o = items[i]); i++)
        {
            o.className = '';
        }
    }

    // ��������
    function Click(event)
    {
        CopyTo();
        if(typeof menu.click == 'function')
        {
            menu.click(value.obj.value);
        }
    }
    function MouseOver()
    {
        var event = this.event;

        var N = 0;
        var pos = GetPosition(menu.id);
        var x = pos.x + N;  // �˵�����
        var y = pos.y + N;
        var w = pos.w - N;
        var h = pos.h - N;
        var X = document.body.scrollLeft + event.clientX;   // �������Ļ�ϵ�����
        var Y = document.body.scrollTop + event.clientY;

        if(Y < y - 30       // ���Ϸ�
           || Y > y + h     // ���·�
           || X < pos.x     // �����
           || X > x + w)    // ���ұ�
        {
            Close(); // ������Χ�����أ�
        }
    }

    /*
     *  ���ܣ���ʼ�������õ���¼������ڲ˵�����ʱ�����ز˵���
     *        ע�⣬�ؼ������� .style.visibility ���õġ�
     *  ���ڣ�2008-10-03 15:56:18
     */
    function Init()
    {
        // �����ڲ˵����ϵĵ������ƶ�����
        InsertFun(menu.obj, "click", Click);
        InsertFun(document, "mouseover", MouseOver);

        // ��ͷ�ϵ���ʱ
        head.obj.onclick = function()
        {
            Show();
        }
        // ���»س���ʱ�����˵�
        head.obj.onkeydown = function(event)
        {
            event = event ? event :  window.event;

            if(13 == event.keyCode) // �س�����/�رղ˵���
            {
                Show();
                return false; // ʹ�ð��»س�ʱҲ��Ҫ�����ύ
            }
            else if(40 == event.keyCode) // �¼�ͷ
            {
                // ��˵�δ�򿪣���򿪣���ʾ����������ѽ����Ƶ���һ�
                if(IsOpen())
                {
                    // �����Ƶ���һ��
                    if(null != menu.obj.first_item)
                    {
                        menu.obj.first_item.focus();
                        return false;   // ע�⣬��radio�з�true��������һ��
                    }
                }
                else
                {
                    Open();
                }
            }
            return true;
        }

        // ��ѡ�����ϵ���ʱ���������ϵ���ʱҲ����ǣ�
        var items = menu.obj.getElementsByTagName('label');
        var o = '';
        var i = 0;
        menu.obj.first_item = items[0];
        for(i=0; (o = items[i]); i++)
        {
            o.onmouseover = function()
            {
                this.focus();
            }
            o.onmouseout = function()
            {
                this.blur();
            }
            o.onkeydown = function(event)
            {
                event = event ? event :  window.event;
                switch(event.keyCode)
                {
                case 39:
                case 40:             // �����¼�ͷʱ�������Ƶ���һ�
                    this.blur();
                    this.next.focus();
                    break;
                case 37:
                case 38:             // �����ϼ�ͷʱ�������Ƶ���һ�
                    this.blur();
                    this.prev.focus();
                    break;
                case 27:
                case 13:             // �س����رղ˵���
                    this.blur();
                    Close();
                    head.obj.focus();
                    return false;    // ���뷵��false�����򽫴����ύ��
                    break;
                case 32:
                    return true;     // ���ſո��������ִ��ѡ��
                    break;
                }
                return false;        // ע�⣬��������䣬�������ϡ��¼�ͷѡ��ʱ������������ѡ��!
            }
            o.focus = function()
            {
                this.className = 'in';
                this.getElementsByTagName('input')[0].focus();
            }
            o.blur = function()
            {
                this.className = '';
            }
            // ��¼ǰ����Ԫ�أ�˫��ѭ����
            o.prev = (i==0) ? items[items.length - 1] : items[i-1];
            o.next = (i==items.length - 1) ? items[0] : items[i+1];
        }

        // ��������������
        if(i > 16)
        {
            menu.obj.style.height = '418px';
            menu.obj.style.overflowY = 'scroll';
        }

        //
        CopyTo();
    }
}
