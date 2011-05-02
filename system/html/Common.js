/**********************************************************************
*                                                                     *
*   ���� : �;���Ӧ���޹صĹ���ʵ�֣�                                 *
*   ��д : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   ���� : 2008-10-10                                                 *
*   �汾 : 1.0                                                        *
*                                                                     *
**********************************************************************/


//����
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
 *  ���ܣ���дalert()���������
 *  ��д��Rocky 2009-12-07
 */
function Alert(/* �ɱ���� */)
{
    Alert.i = (Alert.i || 0) + 1;
    alert('----------[' + Alert.i + ']----------\r\n' + '******[' + [].slice.call(arguments).join(']\r\n******[') + ']');
}

/*
 *  ���ܣ��ڱ����������ݣ����ڵ��ԣ�
 *  ��д��Rocky 2008-10-29
 */
function Title(/* �ɱ���� */)
{
    Title.i = (Title.i || 0) + 1;
    document.title = '[' + Title.i + ']' + '[' + [].slice.call(arguments) + ']';
}

/*
 *  ���ܣ���״̬�������ݣ����ڵ��ԣ�
 *  ��д��Rocky 2008-10-29
 */
function Status(/* �ɱ���� */)
{
    Title.i = (Title.i || 0) + 1;
    window.status = '[' + Title.i + ']' + '[' + [].slice.call(arguments) + ']';
}

function Print(str)
{
    document.write(str);
}

/*
 *  ���ܣ����ݿؼ�id��name���ؿؼ�����
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
 *  ���ܣ���ؼ������в����Զ���Ķ�����������
 *  ���ڣ�2008-10-27 02:01:05
 *  ���ʣ���IE�У�(document == obj)��(obj == document)��
 *        ����(window == obj)��(obj == window)�ıȽϽ����ͬ
 */
function InsertFun(obj, fun_str, fun_name)
{
    if(typeof obj != 'object' || typeof fun_name != 'function')
    {
        alert('Common.js: ���������������ö���' + InsertFun.caller);
        return false;
    }

    // ȡ������
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
 *  ���ܣ�ȡ�ÿؼ��ľ���λ��(x, y), objIDΪ����ID;
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
 *  ���ܣ��г��������е����ԡ�����������Ӧ��ֵ��
 */
function ListOjb(obj)
{
    // �����������е��������ƺ�ֵ
    var prop = "" ;
    var props = "";
    var ary = [];

    // ��ʼ����
    for(var p in obj )
    {
        // ����
        if(typeof(obj[p]) == "function")
        {
            prop = "<b>" + obj[p] + "</b>";
            ary.push(prop);
        }
        else
        {
            // p Ϊ�������ƣ�obj[p]Ϊ��Ӧ���Ե�ֵ
            prop = p + " = <font color=red>" + obj [p] + "</font>" ;
            ary.push(prop);
        }
    }

    // ����
    ary.sort();

    // �����ʾ���е�����
    document.write(ary.join('<br>')) ;
}

/*
 *  ���ܣ���ʱִ��ָ��������������window.setTimeout��
 *  �÷���
 *          SetTimeout(function, 2000, arg1, arg2, ...);
 *          SetInterval()����;
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
 *  ���ܣ���ֹ�¼�����
 */
function EventStop(event)
{
    event = event || window.event;
    if(event.preventDefault)
    {
        event.preventDefault();    // ��ֹ�¼�����[Firefox]
    }
    event.returnValue = false;     // ��ֹ�¼�����[IE]
}

/*
 * ���ܣ�ȡ����͸���ȣ���Ӧ��SetOpacity()��
 * ��д��Rocky 2009-12-23 21:31:57
 * ������obj--����
 */
function GetOpacity(obj)
{
    var depth = 100;    // Ĭ�Ϸ�����͸��
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
 * ���ܣ����ö���͸����
 * ��д��Rocky 2009-08-17 23:23:43
 * ������obj--����
 *       depth--͸���ȣ�ȡֵ[0, 100]��ֵԽСԽ͸����0ʱȫ͸����
 */
function SetOpacity(obj, depth)
{
    if(document.all)
    {
        // IE��
        obj.style.filter = "Alpha(Opacity=" + depth + ")";
    }
    else
    {
        // FF��
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
 *  ���ܣ�ȡʱ�䴮��ΪKEY����ע�⣬ÿ����һ��װ����һ����KEY
 *  ��д��Rocky 2008-11-10
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
 *  ���ܣ���ȡҳ����ѡ�е��ı�
 *  ��д��2008-10-26 22:28:31
 *  ˵�����������������
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
 *  ���ܣ���ȡҳ����ѡ�е�html���루�ı�����ǣ�
 *  ��д��Rocky 2009-12-14 01:14:47
 */
function GetSelectHtml()
{
    // Firefox, Chrome�ȣ�bug: ѡ�����ܿ��� [XXX]��
    if(window.getSelection)
    {
        var id = Math.random();
        // ����һ���
        var range = window.getSelection().getRangeAt(0);
        var span = document.createElement("span");
        span.id = id;
        range.surroundContents(span);

        // ȡ��ѡ�е�html�ı�
        var html = span.innerHTML;

        // ��ȥ���ղ���ı��
        var mark_rex = "(<span [^>]*" + id + "[^>]*>)" + "([^<]*)" + "(</span>)";
        var re = new RegExp(mark_rex, "ig");
        var str = span.parentNode.innerHTML.replace(re, "$2");
        span.parentNode.innerHTML = str;

        // ����html�ı�
        return html;
    }
    // IE
    return document.selection.createRange().htmlText;
}

/*
 * ���ܣ�ת��ҳ�����
 * ��д��2009-08-19 00:38:45
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
 * ���ܣ�ʱ�䴮ת��
 *          1. 20060324120254 => 2006��03��24��12��02��54��
 *          2. 20060324120254 => 2006.03.24 12:02:54:
 * ���ã�TimeTo("20060324120254", 1)
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
            t = t.replace(/(\d{4})(\d{2})(\d{2})(\d{2})(\d{2})(\d{2})/, "$1��$2��$3��$4��$5��$6��");
            break;
        case 2:
            t = t.replace(/(\d{4})(\d{2})(\d{2})(\d{2})(\d{2})(\d{2})/, "$1.$2.$3 $4:$5:$6");
            break;
        default:
        {
            // �������ֵ
            var re = new RegExp("([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})", "g");
            t = re.exec(t);
            if(null != t)
            {
                t.shift(); // ȥ����һ��
            }
            break;
        }
    }
    return t;
}

/*
 * ���ܣ������������
 * ��д��Rocky 2009-11-28
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
        alert("LoadCatchphrase::() ����");
        return null;
    }
    return xmlHttp;
}

/*
 * ���ܣ���������server���̶�ΪPOST��ʽ��
 * ��д��Rocky 2009-12-02 17:23:39
 * ע�⣺���ú���ǰ��Ӧ����content�Ƿ���Ҫ encodeURIComponent()
 */
function SubmitToServer(url, content, callback)
{
    var xmlHttp = CreateXmlRequest();
    var method = "post";
    var asyn = true; // Ĭ�ϣ�true��Ϊ�첽�����ص�����

    if("" == url || null == xmlHttp)
    {
        alert("SaveToServer()�г���");
        return;
    }

    // ��������
    function Send()
    {
        // ʹ�����������ʹÿ������ˢ�»��棻
        url += "?" + (Math.random() + "").substr(2) + "&";

        // ����
        xmlHttp.onreadystatechange = Deal;
        xmlHttp.open(method, url, asyn); // true Ϊ�첽�����ص�����
        xmlHttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        xmlHttp.send(content);
    }

    // �����ص�����
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

    // ִ��
    Send();
}

/*
 * ���ܣ��ַ����ֽ�����һ������Ϊ���ֽڣ���ͬ��String.lenght()��
 * ��д��Rocky 2009-12-03 14:05:28
 */
function SizeofString(str)
{
    // ע�⣬��д��ʹ��������ʽ�����ܻ�Ӱ��ȫ�֣�������ҡ�
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
            sum = sum + 2;// ����Ϊ���ֽ�
        }
    }
    return  sum;
}


// ������ƶ���
// [Rocky 2009-12-14 01:23:10]
// id: ���ID�����
function MoveLayer(id)
{
    var This = (window == this) ? MoveLayer : this;
    var bStartMove = false;
    var layer = ('object' == typeof id) ? id : GetObj(id);
    var oPosOfMouse;        // ������ʱ���������
    var oPosOfLayer;        // ������ʱ�Ĳ�����
    var oSeekOfScroll = {};
    var bAllowMove = true;  // ����������ƶ�
    var bAllowScroll = true;// ��������Ļ����

    // �����Ƿ�����������ƶ�
    This.AllowMove = function(allow)
    {
        bAllowMove = allow;
    }
    
    // �����Ƿ���������Ļ����
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
        var top = oPosOfLayer.y + seeky - 14;   // 14Ϊ�ݶ���������ȣ����ģ�
        layer.style.left = left + 'px';
        layer.style.top = top + 'px';

        // ���µ�ǰ�µ��ĵ�����ƫ��
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

    // ����λ��
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

    // ȡ��ǰ�ĵ�����ƫ��
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

        // ���ù������ڵĶ���
        InsertFun(window, "scroll", AdjustPos);

        // �ı䴰�ڴ�Сʱ
        InsertFun(window, "resize", AdjustPos);

        // ȡ���ĵ�����ƫ��
        GetScrollSeek();
    }

    // ִ�г�ʼ��
    Init();
}


/*
 * ���ܣ��رգ����أ����󣨴��ڣ�
 * ��д��Rocky 2009-12-23 21:31:57
 * ������obj--����
 *       delay--��ʱ�����룩�ر�
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
            SetOpacity(obj, obj.oldOpacity); // ��ԭ
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
 * ���ܣ��򿪣���ʾ�����󣨴��ڣ�
 * ��д��Rocky 2009-12-23 21:31:57
 * ������obj--����
 *       delay--��ʱ�����룩�ر�
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
 * ���ܣ�������汾�ж�
 * ��д��Rocky 2009-12-23 21:49:42
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
 * ���ܣ�����ȫ���¼�
 * ��д��Rocky 2010-01-21 17:44:20
 * ���磺   
 *       SetMouseEvent("click", menu.Close);
 */
function SetMouseEvent(event, fun)
{
    if (window.attachEvent)
    {
        //֧�� IE
        document.attachEvent("on" + event, fun);
    }
    else
    {
        //֧�� FF
        document.addEventListener(event, fun, false);
    }
}

function SetCookie(name, value, expire)
{
    expire = expire || 1000*24*3600*1000; // Ĭ��1000�����
    var exp  = new Date();
    exp.setTime(exp.getTime() + expire);
    document.cookie = name + "="+ escape (value) + ";expires=" + exp.toGMTString();
}

function GetCookie(name)
{
    var arr = document.cookie.match(new RegExp("(^| )"+name+"=([^;]*)(;|$)"));
    if(arr != null) return unescape(arr[2]); return null;
}

