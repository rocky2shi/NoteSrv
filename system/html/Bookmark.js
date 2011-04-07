/**********************************************************************
*                                                                     *
*   功能 : 书签设置                                                   *
*   编写 : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   日期 : 2009-08-10                                                 *
*   版本 : 0.9.2                                                      *
*   修改、使用方法见本文件底说明                                      *
*                                                                     *
**********************************************************************/



/*
 * form_data_id: 用于提交数据的form
 * type: 书签类型：1. "anchor" -- 用于message.cgi中
 *                 2. "coord" -- 用于browse.cgi中
 */
function Bookmark(form_data_id, type)
{
    /*
     * 数据成员
     */
    var m_mark;                         // 界面书签对象
    var m_data = GetObj(form_data_id);  // 书签提交数据用的文本控件
    var m_ary = [];                     // 书签对象数组
    var m_cur_index = 0;                // 当前书签索引
    var m_style_def = "color:#737373; cursor:pointer;";
    var m_type = type || "anchor";


    /*
     * [2009-08-18 00:26:12]
     * 为了适应两种书签操作：在message.cgi和browse.cgi中，
     *      message.cgi: 以a锚点来记录跳转
     *      browse.cgi: 以文档位置坐标来记录跳转
     *   所以在m_fun中设置两个对象"anchor"和"coord"，它们中第一
     *   个function用来组装数据，第二个用来执行跳转；
     */
    var m_fun = {"anchor" : [
                    // 组装数据对象
                    function(key, title)
                    {
                        return {"key":key, "title":title, "create":GetKeyStr()};
                    },
                    // 执行对应的跳转动作
                    function(index)
                    {
                    }
                 ],
                 "coord" : [
                    function(key)
                    {
                        // 取文档中选中文本作为书签说明
                        var str = GetSelectText() || "[空]";
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
                        

                        // 组装为对象
                        // 重复key和y，是为了兼容以前数据；
                        var data = {"key":key, "title":str, "create":GetKeyStr()};
                        return data;
                    },
                    function(index)
                    {
                    }
                 ]
    }



    /*
     * 外部接口
     */
    Bookmark.Set = function(key, title)
    {
        var data = GetDataObj(key, title);

        if( exist(data) )
        {
            window.status = "键已存在：" + data.key;
            return;
        }

        // 加新书签
        Insert(data);

        // 刚插入书签移到首位
        MoveTo(m_ary.length - 1, 0);

        // 设置为当前
        SetCur(0);

        // 刷新
        Syn();

        // 提交到服务器
        if('coord' != m_type)
        {
            Submit(); // [Rocky 2009-12-09 21:42:17]
        }
    }

    // 闪动书签当前按钮
    Bookmark.Flash = function()
    {
        // 改由外部定时器调用
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

    // 反查书签[2009-08-20 00:55:43]
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
     * 内部函数
     */
    // 取索引对应的的对书签
    function $(index)
    {
        var span = m_mark.getElementsByTagName("span");
        var li = span[0].getElementsByTagName("li");

        return li[index];
    }

    // 初始化
    function Init()
    {
        // 创建书签界面
        CreateUI();

        // 提示窗口
        Tips();

        var ary = ToArray(m_data.value);
        for(var i=0; i<ary.length; i++)
        {
            Insert(ary[i]);
        }

        // 初始位置
        AdjustPos();

        // 设置滚动窗口的动作
        InsertFun(window, "scroll", AdjustPos);

        // 改变窗口大小时
        InsertFun(window, "resize", AdjustPos);

        var li = m_mark.getElementsByTagName('li');

        // 上箭头（<li>中第一个元素）
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
                // 跳到前一个
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

        // 下箭头（<li>中最后一个元素）
        var down = li[li.length - 1];
        SetStyleDef(down);
        down.onclick = function(event)
        {
            event = event || window.event;
            if(event.ctrlKey)
            {
                // 移动前跳转
                MoveToNext();
            }
            else
            {
                // 跳到下一个书签处
                Next();
            }
        }
        down.onmouseover = up.onmouseover;
        down.onmouseout = up.onmouseout;
    }

    // 加入书签
    function Insert(ary)
    {
        // 创建书签按钮
        var obj = Create();

        // 设置风格
        SetStyleDef(obj);

        // 设置动作
        SetEvent(obj);

        // 填充数组
        m_ary[ m_ary.length ] = ary;

        return true;
    }

    // 创建书签界面
    function CreateUI()
    {
        var body = document.getElementsByTagName('body')[0];
        var ul = document.createElement("ul");

        // 书签按钮
        ul.innerHTML = "<li>▲</li><span></span><li>▼</li>";
        ul.style.cssText = "position:absolute; visibility:hidden; "
                           + "top:0px; left:0px; "
                           + "margin:0px; padding:0px; color:#737373; "
                           + "cursor:pointer;";
        body.appendChild(ul);
        m_mark = ul;
    }

    // 在界面上创建书签，并返回对象句柄；
    function Create()
    {
        var span = m_mark.getElementsByTagName("span")[0];
        var li = span.getElementsByTagName("li");
        var button = document.createElement("li");

        button.innerHTML = "●";
        button.index = li.length;   // 当前按钮序号
        button.flash = false;       // 闪动标记
        // button.title = '单击转跳，右击删除书签；';

        // 插入
        span.appendChild(button);

        return button;
    }

    // 提示窗口
    function Tips()
    {
        var win;
        var timer;

        // 创建
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

        // 打开
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

        // 关闭
        Tips.Close = function()
        {
            // 延时关闭
            timer = SetTimeout(DoClose, 100);
        }

        // 初始化
        Create();
    }

    // 闪动当前书签对应的数据
    function Indication(obj)
    {
        if(null == obj || obj.lock)
        {
            return;
        }
        var bgcolor = obj.style.background;     // 原颜色
        var cssText = obj.style.cssText;        // 原风格
        var count = 0;

        obj.lock = true;    // 加锁，以免多次闪动；
        Do();

        function Do()
        {
            if(count > 2)   // 闪两次后返还原
            {
                obj.lock = false;   // 解锁
                obj.style.cssText = cssText;    // 还原
                return;
            }
            // 闪动
            obj.style.background = (count%2 == 0) ? '#939393' : bgcolor;
            count++;
            SetTimeout(Do, 500);
        }
    }

    // 书签已存在，返回true，否则返回false;
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

    // 删除界面上的书签
    function Remove(index)
    {
        var span = m_mark.getElementsByTagName("span")[0];
        var li = span.getElementsByTagName("li");
        // 删除
        span.removeChild( $(index) );
        // 重设索引
        for(var i=0; i<li.length; i++)
        {
            li[i].index = i;
        }
    }

    // 默认风格
    function SetStyleDef(obj)
    {
        obj.style.cssText = m_style_def;
    }

    // 当前书签风格
    function SetStyleCur(obj)
    {
        obj.style.cssText = "#FA8072";
    }

    // 鼠标移出
    function SetStyleMouseOut(obj)
    {
        obj.style.color = "#737373";
    }

    // 鼠标移进
    function SetStyleMouseOver(obj)
    {
        obj.style.color = "#FA8072";
    }

    // 闪动
    function SetStyleFlash(obj)
    {
        obj.style.color = "#8C8A8E";
    }

    // 设置事件
    function SetEvent(obj)
    {
        obj.onclick = function(event)
        {
            event = event || window.event;
            // 修改
            if(event.ctrlKey)
            {
                Modify(this.index);
                return;
            }
            // 点击处书签置为当前书签
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
            EventStop(event);   // 中止事件传递
            // 删除当前书签
            Del(this.index);
            return false;
        }
    }

    // 设置为当前书签
    function SetCur(index)
    {
        if(index < 0 || index >= m_ary.length)
        {
            return;
        }
        SetStyleDef( $(m_cur_index) );
        m_cur_index = index;
        JumpTo(index);
        Indication( GetObj(m_ary[index].key) ); // 闪动
    }

    // 跳到前一书签
    function Prev()
    {
        SetCur(m_cur_index - 1);
    }

    // 跳到后一书签
    function Next()
    {
        SetCur(m_cur_index + 1);
    }

    // 当前书签前移一个位置
    function MoveToPrev()
    {
        // 移动内部数组
        MoveTo(m_cur_index - 1, m_cur_index);
        // 移动界面上书签按钮
        Prev();
        // 刷新
        Syn();
        // 提交
        Submit();
    }

    // 当前书签后移一个位置
    function MoveToNext()
    {
        // 移动内部数组
        MoveTo(m_cur_index, m_cur_index + 1);
        // 移动界面上书签按钮
        Next();
        // 刷新
        Syn();
        // 提交
        Submit();
    }

    // 当前书签移到第n个索引，其它的的依次移动．
    function MoveTo(src, dest)
    {
        /*
         * 根据src和dest的前后关系来循环移动，把src处的元素移到
         * dest处，其它的依次移到一个位置
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

    // 跳转
    function JumpTo(index)
    {
        /*
         * 2009-08-20 00:36:44
         */

        // 兼容以前用坐标为书签标记的数据
        if(m_ary[index].y)
        {
            window.scrollTo(0, m_ary[index].y);
            return;
        }

        // 以正文中插入的<span>为标记（直接调用上面的代码）
        var e = GetObj(m_ary[index].key);
        if(null == e)
        {
            return;
        }
        var a = GetPos(m_mark);     // 书签坐标
        var b = GetPos(e);          // 书签对应正文记录坐标
        var middle = a.y + a.h / 2; // 相当于屏幕中间
        var center = b.y + b.h / 2; // 正文标记的中间
        window.scrollBy(0, center - middle);
    }

    // 取数据对象
    function GetDataObj(key, title)
    {
        return m_fun[m_type][0](key, title);
    }

    // 从串到数组
    function ToArray(str)
    {
        var ary = [];
        if("" == str)
        {
            str = "[]";
        }
        try     // 确保当数据有误时js也是可用的。 [2008-11-06 22:13:10]
        {
            eval("ary=" + str);
        }
        catch(e)
        {
            ary = [];
            alert('书签数据出错，清除书签。');
        }
        return ary;
    }

    // 从数组到串
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
                // 转义\        [2008-11-06 22:07:49]
                s = (e[o] + "").replace(/\\/g, "\\\\");
                // 转义'
                s = s.replace(/'/g, "\\'");
                str += "'" + o + "':'" + s + "',";
            }
            str = str.replace(/,$/, '');    // 去掉最后的‘,’
            str += "},";
        }

        str = str.replace(/,$/, '');    // 去掉最后的‘,’
        str += "]";

        return str;
    }

    // 同步书签和提交form数据
    function Syn()
    {
        m_data.value = ToStr(m_ary);
        m_ary = ToArray(m_data.value);
    }

    // 调整位置（定位于左侧）
    function AdjustPos()
    {
        var nWinHeight = document.body.clientHeight;
        var nObjHeight = m_mark.offsetHeight;
        var nTop = (nWinHeight - nObjHeight) /2 + document.body.scrollTop;
        m_mark.style.top = nTop + 'px';
        m_mark.style.visibility = "visible";
    }


    // 修改当前书签标注
    function Modify(index)
    {
        if(!m_ary || !m_ary[index])
        {
            return;
        }
        var str = window.prompt("修改当前书签[" + (index + 1) + "]标注，请输入新的说明：", m_ary[index].title);
        if(null == str)
        {
            return;
        }
        // 修改
        m_ary[index].title = str;
        // 刷新
        Syn();
        // 提交
        Submit();
    }

    // 册除当前书签
    function Del(index)
    {
        if(!m_ary || !m_ary[index])
        {
            return;
        }
        if(!window.confirm("删除编号为[" + (index + 1) + "]的书签，确定吗？"))
        {
            return;
        }
        // 删除
        delete m_ary[index];
        // 删除界面上的
        Remove(index);
        // 刷新
        Syn();
        // 设置0为当前书签
        // SetCur(0);
        // 提交
        Submit();
    }

    // 提交到服务器
    function Submit()
    {
        m_data.form.submit();
    }

    // 初始化
    Init();
}










/**********************************************************************
使用方法：

html:

<!-- 书签界面 -->
<ul id="bookmark" style="position:absolute; visibility:hidden; top:200px; left:0px; width:1px; margin:0px; padding:0px; color:#737373; cursor:pointer;">
<li name="up">▲</li>
<span></span>
<li>▼</li>
</ul>

<!-- 书签提交 -->
<form method="POST" target="tmp_iframe" style="display:none">
<input name="key" value="20090730222406">
<textarea id="bookmark_data" name="bookmark_data" rows=10 cols=200></textarea>
<input name="bookmark_save" value="提交">
<input type="submit">
</form>


js：
        Bookmark()          -- 初始化
        Bookmark.Set()      -- 设置书签


History:

 2009-08-10 : 0.9版
 2009-12-08 : 0.9.1版．修改提交函数，跟据不同功能调；
 2009-12-09 : 0.9.2版．再次修改提交函数；
 2010-01-14 : 0.9.3版. 修改书签过长部分的显示（点击加号时）；
**********************************************************************/