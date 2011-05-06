


/*
 *  功能：归类选择设置
 *  编写：Rocky 2008-10-29 00:36:34
 *  说明：
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

    // 设置默认值
    head.show = head.show || 'title';
    value.show = value.show || 'value';

    // 记录第一个菜单项
    menu.obj.first_item = null;
    // 原值
    head.obj.old_value = head.obj.value;
    value.obj.old_value = value.obj.value;

    // 对外接口
    //this.Open = Open;
    //this.Close = Close;
    //this.Show = Show;


    // 执行初始化
    Init();

    /*
     *  功能：显示控件；
     *        注意，控件必须是 .style.visibility 设置的。
     */
    function Open()
    {
        // 在每次打开菜单时，都要移到正确位置（因为目的位置可能会变动）。
        MoveTo();
        // 显示
        menu.obj.style.visibility = "visible";
    }

    /*
     *  功能：隐藏控件；
     *        注意，控件必须是 .style.visibility 设置的。
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
     *  功能：检测菜单是否已打开
     *  返回：true-已打开，false-未打开；
     */
    function IsOpen()
    {
        return menu.obj.style.visibility != "hidden";
    }


    /*
     *  功能：一次调用显示、再次调用隐藏控件；
     *        注意，控件必须是 .style.visibility 设置的。
     */
    function Show()
    {
        IsOpen() ? Close() : Open();
    }

    /*
     *  功能：把控件移到适当位置（用于显示）
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
     *  功能：把控件中已选的checkbox复到目的的value。
     */
    function CopyTo()
    {
        var chks = menu.obj.getElementsByTagName("input");
        var chk = null;
        var menu_chk = {};  // 记录菜单中选中的项

        menu_chk.value = '';    // 值[value]
        menu_chk.title = '';    // 标题[title]
        menu_chk.index = '';    // 序号

        for(var i=0; (chk=chks[i]); i++)
        {
            if(chk.checked)
            {
                menu_chk.value += chk.value ? (chk.value + ',') : '';
                menu_chk.title += chk.title ? (chk.title + ',') : '';
                menu_chk.index += (i+1) + ',';
            }
        }
        // 去掉最后的“,”
        menu_chk.value = menu_chk.value.slice(0, -1);
        menu_chk.title = menu_chk.title.slice(0, -1);
        menu_chk.index = menu_chk.index ? (head.obj.old_value + "[" + menu_chk.index.slice(0, -1) + "]") : "";

        // 显示
        head.obj.value = menu_chk[(head.show)] || head.obj.old_value;
        // 要提交的值
        value.obj.value = menu_chk[(value.show)] || value.obj.old_value;
    }

    // 关闭时清掉所有菜单选项上的反白显示
    function ClearBgColor()
    {
        var items = menu.obj.getElementsByTagName('label');
        for(var o='',i=0; (o = items[i]); i++)
        {
            o.className = '';
        }
    }

    // 动作定义
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
        var x = pos.x + N;  // 菜单坐标
        var y = pos.y + N;
        var w = pos.w - N;
        var h = pos.h - N;
        var X = document.body.scrollLeft + event.clientX;   // 鼠标在屏幕上的坐标
        var Y = document.body.scrollTop + event.clientY;

        if(Y < y - 30       // 在上方
           || Y > y + h     // 在下方
           || X < pos.x     // 在左边
           || X > x + w)    // 在右边
        {
            Close(); // 超出范围，隐藏；
        }
    }

    /*
     *  功能：初始化，设置点击事件，当在菜单外点击时，隐藏菜单；
     *        注意，控件必须是 .style.visibility 设置的。
     *  日期：2008-10-03 15:56:18
     */
    function Init()
    {
        // 设置在菜单体上的单击、移动鼠标等
        InsertFun(menu.obj, "click", Click);
        InsertFun(document, "mouseover", MouseOver);

        // 在头上单击时
        head.obj.onclick = function()
        {
            Show();
        }
        // 按下回车键时弹出菜单
        head.obj.onkeydown = function(event)
        {
            event = event ? event :  window.event;

            if(13 == event.keyCode) // 回车：打开/关闭菜单。
            {
                Show();
                return false; // 使用按下回车时也不要触发提交
            }
            else if(40 == event.keyCode) // 下箭头
            {
                // 如菜单未打开，则打开（显示）它。否则把焦点移到第一项。
                if(IsOpen())
                {
                    // 焦点移到第一项
                    if(null != menu.obj.first_item)
                    {
                        menu.obj.first_item.focus();
                        return false;   // 注意，在radio中返true会跳过第一项
                    }
                }
                else
                {
                    Open();
                }
            }
            return true;
        }

        // 在选择项上单击时（在文字上单击时也做标记）
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
                case 40:             // 当按下箭头时，焦点移到下一项。
                    this.blur();
                    this.next.focus();
                    break;
                case 37:
                case 38:             // 当按上箭头时，焦点移到上一项。
                    this.blur();
                    this.prev.focus();
                    break;
                case 27:
                case 13:             // 回车，关闭菜单。
                    this.blur();
                    Close();
                    head.obj.focus();
                    return false;    // 必须返回false，否则将触发提交。
                    break;
                case 32:
                    return true;     // 开放空格键，用于执行选择。
                    break;
                }
                return false;        // 注意，必须有这句，否则当用上、下箭头选择时，将跳过两个选项!
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
            // 记录前、后元素（双向循环）
            o.prev = (i==0) ? items[items.length - 1] : items[i-1];
            o.next = (i==items.length - 1) ? items[0] : items[i+1];
        }

        // 设置下拉滚动条
        if(i > 16)
        {
            menu.obj.style.height = '418px';
            menu.obj.style.overflowY = 'scroll';
        }

        //
        CopyTo();
    }
}
