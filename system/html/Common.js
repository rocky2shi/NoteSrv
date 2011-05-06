/**********************************************************************
*                                                                     *
*   功能 : 和具体应用无关的功能实现；                                 *
*   编写 : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   日期 : 2008-10-10                                                 *
*   版本 : 1.0                                                        *
*                                                                     *
**********************************************************************/


//调试
function DB(str)
{
    if(null == this.i)
        this.i = 0;
    if(null == str)
        str = '';
    document.title = str + '[' + (this.i++) + ']';
}

function DB2(obj)
{
    for(var i in obj)
    {
        document.write(i + '<br>');
    }
}

/*
 *  功能：重写alert()，方便调试
 *  编写：Rocky 2009-12-07
 */
function Alert(/* 可变参数 */)
{
    Alert.i = (Alert.i || 0) + 1;
    alert('----------[' + Alert.i + ']----------\r\n' + '******[' + [].slice.call(arguments).join(']\r\n******[') + ']');
}

/*
 *  功能：在标题栏显数据，用于调试；
 *  编写：Rocky 2008-10-29
 */
function Title(/* 可变参数 */)
{
    Title.i = (Title.i || 0) + 1;
    document.title = '[' + Title.i + ']' + '[' + [].slice.call(arguments) + ']';
}

/*
 *  功能：在状态栏显数据，用于调试；
 *  编写：Rocky 2008-10-29
 */
function Status(/* 可变参数 */)
{
    Title.i = (Title.i || 0) + 1;
    window.status = '[' + Title.i + ']' + '[' + [].slice.call(arguments) + ']';
}

function Print(str)
{
    document.write(str);
}

/*
 *  功能：根据控件id或name返回控件对象；
 */
function $(obj, p)
{
    if(typeof p != 'object')
    {
        p = document;
    }
    var tmp = p.getElementById(obj);
    if(null!=tmp && null!=tmp.id && ''!=tmp.id && tmp.id==obj)
        return tmp;
    tmp = p.getElementsByName(obj);
    if(null!=tmp && tmp.length>0)
        return tmp;
}

function GetObj(obj, p)
{
    return $(obj, p);
}

/*
 *  功能：向控件动作中插入自定义的动作（函数）
 *  日期：2008-10-27 02:01:05
 *  疑问：在IE中，(document == obj)和(obj == document)　
 *        及　(window == obj)和(obj == window)的比较结果不同
 */
function InsertFun(obj, fun_str, fun_name)
{
    if(typeof obj != 'object' || typeof fun_name != 'function')
    {
        alert('Common.js: 函数参数出错，调用都：' + InsertFun.caller);
        return false;
    }

    // 取出参数
    var args = [].slice.call(arguments, 3);

    if(document == obj || obj != window)
    {
        obj.attachEvent ?
            obj.attachEvent("on" + fun_str, function(){obj.event=window.event; return fun_name.apply(obj, args);})
            : obj.addEventListener(fun_str, function(event){obj.event=event; return fun_name.apply(obj, args);}, false);
    }
    else
    {
        window.attachEvent ?
            window.attachEvent("on" + fun_str, function(){return fun_name.apply(window, args);})
            : window.addEventListener(fun_str, function(event){window.event=event; return fun_name.apply(window, args);}, false);
    }
}

/*
 *  功能：取得控件的绝对位置(x, y), objID为对象ID;
 */
function GetPosition(objID)
{
    var e = GetObj(objID);
    if(null == e)
    {
        return null;
    }
    return GetPos(e);
}
function GetPos(obj)
{
    function POS()
    {
        this.x = 0;
        this.y = 0;
        this.w = 0;
        this.h = 0;
    }
    var e = obj;
    var pos = new POS();
    if(null == e)
    {
        return null;
    }
    pos.w = e.offsetWidth;
    pos.h = e.offsetHeight;
    pos.x = e.offsetLeft;
    pos.y = e.offsetTop;
    while( (e = e.offsetParent) )
    {
        pos.x += e.offsetLeft;
        pos.y += e.offsetTop;
    }
    return pos;
}

/*
 *  功能：列出对象所有的属性、方法，及对应的值；
 */
function ListOjb(obj)
{
    // 用来保存所有的属性名称和值
    var prop = "" ;
    var props = "";
    var ary = [];

    // 开始遍历
    for(var p in obj )
    {
        // 方法
        if(typeof(obj[p]) == "function")
        {
            prop = "<b>" + obj[p] + "</b>";
            ary.push(prop);
        }
        else
        {
            // p 为属性名称，obj[p]为对应属性的值
            prop = p + " = <font color=red>" + obj [p] + "</font>" ;
            ary.push(prop);
        }
    }

    // 排序
    ary.sort();

    // 最后显示所有的属性
    document.write(ary.join('<br>')) ;
}

/*
 *  功能：延时执行指定函数（类似于window.setTimeout）
 *  用法：
 *          SetTimeout(function, 2000, arg1, arg2, ...);
 *          SetInterval()类似;
 */
function SetTimeout(fRef, mDelay)
{
    if(typeof fRef == 'function')
    {
        var argu = [].slice.call(arguments, 2);
        function f()
        {
            fRef.apply(null, argu);
        }
        return window.setTimeout(f, mDelay);
    }
    return window.setTimeout(fRef, mDelay);
}
function ClearTimeout(iTimeoutID)
{
    window.clearTimeout(iTimeoutID);
}
function SetInterval(fRef, mDelay)
{
    if(typeof fRef == 'function')
    {
        var argu = [].slice.call(arguments, 2);
        function f()
        {
            fRef.apply(null, argu);
        }
        return window.setInterval(f, mDelay);
    }
    return window.setInterval(fRef, mDelay);
}
function ClearInterval(iTimeoutID)
{
    window.clearInterval(iTimeoutID);
}

/*
 *  功能：阻止事件传递
 */
function EventStop(event)
{
    event = event || window.event;
    if(event.preventDefault)
    {
        event.preventDefault();    // 阻止事件传递[Firefox]
    }
    event.returnValue = false;     // 阻止事件传递[IE]
}

/*
 * 功能：取对象透明度（对应于SetOpacity()）
 * 编写：Rocky 2009-12-23 21:31:57
 * 参数：obj--对象
 */
function GetOpacity(obj)
{
    var depth = 100;    // 默认返回无透明
    if(Version.ie())
    {
        var v = obj.style.filter.match(/Opacity=([\d.]+)/);
        depth = v && v[1] || 100;
    }
    else if(Version.firefox())
    {
        depth = (obj.style.MozOpacity * 100) || 100;
    }
    else if(Version.chrome())
    {
        depth = (obj.style.opacity * 100) || 100;
    }
    return depth;
}

/*
 * 功能：设置对象透明度
 * 编写：Rocky 2009-08-17 23:23:43
 * 参数：obj--对象
 *       depth--透明度，取值[0, 100]，值越小越透明，0时全透明．
 */
function SetOpacity(obj, depth)
{
    if(document.all)
    {
        // IE中
        obj.style.filter = "Alpha(Opacity=" + depth + ")";
    }
    else
    {
        // FF中
        obj.style.MozOpacity = depth/100;
        // Chrome
        obj.style.opacity = depth/100;
    }
}
function Opacity(obj, depth)
{
    return SetOpacity(obj, depth);
}

/*
 *  功能：取时间串（为KEY），注意，每调用一次装产生一个新KEY
 *  编写：Rocky 2008-11-10
 */
function GetKeyStr()
{
    var tt = new ServerDate();
    var y = tt.getFullYear() + '';
    var m = (tt.getMonth() + 1) + '';
    var d = tt.getDate() + '';
    var hh = tt.getHours() + '';
    var mm = tt.getMinutes() + '';
    var ss = tt.getSeconds() + '';

    m = (parseInt(m) < 10) ? ('0' + m) : m;
    d = (parseInt(d) < 10) ? ('0' + d) : d;
    hh = (parseInt(hh) < 10) ? ('0' + hh) : hh;
    mm = (parseInt(mm) < 10) ? ('0' + mm) : mm;
    ss = (parseInt(ss) < 10) ? ('0' + ss) : ss;

    return (y + m + d + hh + mm + ss);
}

/*
 *  功能：获取页面上选中的文本
 *  编写：2008-10-26 22:28:31
 *  说明：由网上整理而来
 */
function GetSelectText()
{
    if (window.getSelection)
    {
        return window.getSelection().toString();
    }
    else if (document.getSelection)
    {
        return document.getSelection();
    }
    else if (document.selection)
    {
        return document.selection.createRange().text;
    }
}

/*
 *  功能：获取页面上选中的html代码（文本及标记）
 *  编写：Rocky 2009-12-14 01:14:47
 */
function GetSelectHtml()
{
    // Firefox, Chrome等（bug: 选定不能跨标记 [XXX]）
    if(window.getSelection)
    {
        var id = Math.random();
        // 插入一标记
        var range = window.getSelection().getRangeAt(0);
        var span = document.createElement("span");
        span.id = id;
        range.surroundContents(span);

        // 取出选中的html文本
        var html = span.innerHTML;

        // 再去除刚插入的标记
        var mark_rex = "(<span [^>]*" + id + "[^>]*>)" + "([^<]*)" + "(</span>)";
        var re = new RegExp(mark_rex, "ig");
        var str = span.parentNode.innerHTML.replace(re, "$2");
        span.parentNode.innerHTML = str;

        // 返回html文本
        return html;
    }
    // IE
    return document.selection.createRange().htmlText;
}

/*
 * 功能：转换页面编码
 * 编写：2009-08-19 00:38:45
 */
function Escape(str)
{
    // str = str.replace(/&/, "&amp;");        // &
    str = str.replace(/</g, "&lt;");        // <
    str = str.replace(/>/g, "&gt;");        // >
    // str = str.replace(/\"/g, "&quot;");     // "
    // str = str.replace(/\'/g, "&#39;");      // '
    // str = str.replace(/=/g, "&#61;");       // =
    return str;
}

/*
 * 功能：时间串转换
 *          1. 20060324120254 => 2006年03月24日12点02分54秒
 *          2. 20060324120254 => 2006.03.24 12:02:54:
 * 调用：TimeTo("20060324120254", 1)
 */
function TimeTo(t, type)
{
    if(null == t)
    {
        return null;
    }

    switch(type)
    {
        case 1:
            t = t.replace(/(\d{4})(\d{2})(\d{2})(\d{2})(\d{2})(\d{2})/, "$1年$2月$3日$4点$5分$6秒");
            break;
        case 2:
            t = t.replace(/(\d{4})(\d{2})(\d{2})(\d{2})(\d{2})(\d{2})/, "$1.$2.$3 $4:$5:$6");
            break;
        default:
        {
            // 分离各段值
            var re = new RegExp("([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})", "g");
            t = re.exec(t);
            if(null != t)
            {
                t.shift(); // 去掉第一个
            }
            break;
        }
    }
    return t;
}

/*
 * 功能：创建处理对象
 * 编写：Rocky 2009-11-28
 */
function CreateXmlRequest()
{
    var xmlHttp;
    if (window.ActiveXObject)
    {
        xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    else if (window.XMLHttpRequest)
    {
        xmlHttp = new XMLHttpRequest();
    }
    else
    {
        alert("LoadCatchphrase::() 出错");
        return null;
    }
    return xmlHttp;
}

/*
 * 功能：发送请求到server（固定为POST方式）
 * 编写：Rocky 2009-12-02 17:23:39
 * 注意：调用函数前，应考滤content是否需要 encodeURIComponent()
 */
function SubmitToServer(url, content, callback)
{
    var xmlHttp = CreateXmlRequest();
    var method = "post";
    var asyn = true; // 默认（true）为异步处理返回的数据

    if("" == url || null == xmlHttp)
    {
        alert("SaveToServer()中出错");
        return;
    }

    // 发送请求
    function Send()
    {
        // 使用了随机数，使每次请求都刷新缓存；
        url += "?" + (Math.random() + "").substr(2) + "&";

        // 发送
        xmlHttp.onreadystatechange = Deal;
        xmlHttp.open(method, url, asyn); // true 为异步处理返回的数据
        xmlHttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        xmlHttp.send(content);
    }

    // 处理返回的数据
    function Deal()
    {
        if(xmlHttp.readyState == 4 && xmlHttp.status == 200)
        {
            // alert( decodeURIComponent(xmlHttp.responseText) );
            if(typeof callback == 'function')
            {
                callback(xmlHttp.responseText);
            }
        }
    }

    // 执行
    Send();
}

/*
 * 功能：字符串字节数，一个汉字为两字节（不同于String.lenght()）
 * 编写：Rocky 2009-12-03 14:05:28
 */
function SizeofString(str)
{
    // 注意，此写法使用了正则式，可能会影响全局，引起错乱。
    // if(typeof str == "string")
    // {
    //     var zh = str.match(/[^x00-xff]/ig);
    //     return str.length + (zh ? zh.length : 0);
    // }
    var i, sum;
    sum = 0;
    for(i=0 ; i<str.length;i++)
    {
        if((str.charCodeAt(i) >= 0)  &&  (str.charCodeAt(i) <= 127))
        {
            sum = sum + 1;
        }
        else
        {
            sum = sum + 2;// 汉字为两字节
        }
    }
    return  sum;
}


// 用鼠标移动层
// [Rocky 2009-12-14 01:23:10]
// id: 层的ID或对象
function MoveLayer(id)
{
    var This = (window == this) ? MoveLayer : this;
    var bStartMove = false;
    var layer = ('object' == typeof id) ? id : GetObj(id);
    var oPosOfMouse;        // 鼠标点下时的鼠标坐标
    var oPosOfLayer;        // 鼠标点下时的层坐标
    var oSeekOfScroll = {};
    var bAllowMove = true;  // 允许用鼠标移动
    var bAllowScroll = true;// 允许随屏幕滚动

    // 设置是否允许用鼠标移动
    This.AllowMove = function(allow)
    {
        bAllowMove = allow;
    }
    
    // 设置是否允许随屏幕滚动
    This.AllowScroll = function(allow)
    {
        bAllowScroll = allow;
    }

    function Start(event)
    {
        oPosOfMouse = GetMousePos(event);
        oPosOfLayer = GetPos(layer);
        bStartMove = true;
        layer.style.cursor = "move";
    }
    function Stop()
    {
        bStartMove = false;
        layer.style.cursor = "";
    }
    function Move(event)
    {
        if(!bStartMove)
        {
            return;
        }
        var mouse = GetMousePos(event);
        var seekx = mouse.x - oPosOfMouse.x;
        var seeky = mouse.y - oPosOfMouse.y;
        var left = oPosOfLayer.x + seekx;
        var top = oPosOfLayer.y + seeky - 14;   // 14为暂定滚动条宽度（待改）
        layer.style.left = left + 'px';
        layer.style.top = top + 'px';

        // 记下当前新的文档滚动偏移
        GetScrollSeek();
    }

    function GetMousePos(event)
    {
        event = event || window.event;
        return {
            x: event.clientX + document.body.scrollLeft,
            y: event.clientY + document.body.scrollTop
        };
    }

    function GetScrollPos()
    {
        return {
            x: document.body.scrollLeft,
            y: document.body.scrollTop
        }
    }

    // 调整位置
    function AdjustPos(event)
    {
        if( !bAllowScroll )
        {
            return;
        }
        var pos = GetPos(layer);
        layer.style.left =document.body.scrollLeft + oSeekOfScroll.x + 'px';
        layer.style.top = document.body.scrollTop + oSeekOfScroll.y + 'px';
    }

    // 取当前文档滚动偏移
    function GetScrollSeek()
    {
        var oInitPosOfScroll = GetScrollPos();
        var oInitPosOfLayer = GetPos(layer);
        oSeekOfScroll.x = oInitPosOfLayer.x - oInitPosOfScroll.x;
        oSeekOfScroll.y = oInitPosOfLayer.y - oInitPosOfScroll.y;
    }

    function Init()
    {
        layer.onmousedown = function(event)
        {
            event = event || window.event;
            if( !bAllowMove )
            {
                return;
            }
            //if(event.ctrlKey)
            //{
                Start(event);
            //}
        }
        layer.onmouseup = function()
        {
            Stop();
        }
        layer.onmousemove = function(event)
        {
            event = event || window.event;
            Move(event);
        }

        // 设置滚动窗口的动作
        InsertFun(window, "scroll", AdjustPos);

        // 改变窗口大小时
        InsertFun(window, "resize", AdjustPos);

        // 取初文档滚动偏移
        GetScrollSeek();
    }

    // 执行初始化
    Init();
}


/*
 * 功能：关闭（隐藏）对象（窗口）
 * 编写：Rocky 2009-12-23 21:31:57
 * 参数：obj--对象
 *       delay--延时（毫秒）关闭
 */
function CloseWin(obj, delay)
{
    delay = delay || 0;
    obj.oldOpacity = obj.oldOpacity || GetOpacity(obj);
    var depth = obj.oldOpacity;
    function DoClose()
    {
        SetOpacity(obj, depth);
        depth -= 5;

        if(depth <= 0)
        {
            Hide();
            SetOpacity(obj, obj.oldOpacity); // 复原
            return;
        }
        obj.hCloseTimer = SetTimeout(DoClose, 30);
    }
    function Hide()
    {
        obj.style.visibility = "hidden";
    }

    if(obj.hCloseTimer)
    {
        ClearTimeout(obj.hCloseTimer);
    }

    obj.hCloseTimer = SetTimeout(DoClose, delay);
}

/*
 * 功能：打开（显示）对象（窗口）
 * 编写：Rocky 2009-12-23 21:31:57
 * 参数：obj--对象
 *       delay--延时（毫秒）关闭
 */
function OpenWin(obj)
{
    if(obj.hCloseTimer)
    {
        ClearTimeout(obj.hCloseTimer);
    }
    if(obj.oldOpacity)
    {
        SetOpacity(obj, obj.oldOpacity);
    }
    obj.style.visibility = "";
}

/*
 * 功能：浏览器版本判断
 * 编写：Rocky 2009-12-23 21:49:42
 */
function Version()
{
    var ua = navigator.userAgent.toLowerCase();

    Version.ie = function()
    {
        return ua.indexOf("msie") != -1 ;
    }

    Version.firefox = function()
    {
        return ua.indexOf("firefox") != -1 ;
    }

    Version.chrome = function()
    {
        return ua.indexOf("chrome") != -1 ;
    }

    Version.other = function()
    {
        return !Version.ie() && !Version.firefox() && !Version.chrome();
    }
}

/*
 * 功能：设置全局事件
 * 编写：Rocky 2010-01-21 17:44:20
 * 例如：   
 *       SetMouseEvent("click", menu.Close);
 */
function SetMouseEvent(event, fun)
{
    if (window.attachEvent)
    {
        //支持 IE
        document.attachEvent("on" + event, fun);
    }
    else
    {
        //支持 FF
        document.addEventListener(event, fun, false);
    }
}

function SetCookie(name, value, expire)
{
    expire = expire || 1000*24*3600*1000; // 默认1000天过期
    var exp  = new Date();
    exp.setTime(exp.getTime() + expire);
    document.cookie = name + "="+ escape (value) + ";expires=" + exp.toGMTString();
}

function GetCookie(name)
{
    var arr = document.cookie.match(new RegExp("(^| )"+name+"=([^;]*)(;|$)"));
    if(arr != null) return unescape(arr[2]); return null;
}

