/************************************************************
*                                                           *
*   作者: rocky                                             *
*   QQ  : 15586350                                          *
*   功能: 函数实现                                          *
*                                                           *
************************************************************/

try
{
    if(typeof g_error == "undefined")
    {
        g_error = '';
    }
}
catch(e)
{}



/***********************************************************/
//菜单操作函数
function MenuInit(menu_id)
{
    MenuInit.objMenu    = GetObj(menu_id);
    if(null == MenuInit.objMenu)
        return;
    MenuInit.bHide  = false;
    MenuInit.bTOut  = false;
    MenuInit.bAOut  = false;
    MenuInit.clear  = '';
    MenuInit.Menu       = Menu;
    MenuInit.MenuHide   = MenuHide;
    MenuInit.MenuShow   = MenuShow;

    MenuInit.objMenu.onmouseover = function()
    {
        MenuInit.bTOut = false;
        MenuInit.Menu();
    }
    MenuInit.objMenu.onmouseout = function()
    {
        MenuInit.bTOut = true;
        MenuInit.Menu();
    }
    MenuInit.objMenu.onclick = function()
    {
        MenuInit.MenuHide();
    }
    function Menu()
    {
        MenuInit.bHide = (MenuInit.bTOut || MenuInit.bAOut);
        if(MenuInit.clear)
            clearTimeout(MenuInit.clear);
        MenuInit.clear = setTimeout("if(MenuInit.bHide)MenuInit.MenuHide();", 312);
    }
    function MenuHide()
    {
        MenuInit.objMenu.className = 'hidden';
    }
    function MenuShow()
    {
        MenuInit.objMenu.className = '';
    }
    function Init()
    {
        if(g_isIE)
        {
            MenuInit.objMenu.style.right = "0px";
            MenuInit.objMenu.style.top = "3px";
        }
        else
        {
            MenuInit.objMenu.style.right = "5%";
        }
        if(window.location.href.indexOf('login') < 0)
        {
            var obj = GetObj('operator');
            DelStyle(obj, 'hidden');
        }
    }
    Init();//初始化
}


/***********************************************************/
//form操作函数

//设置默认的form提交函数，在DefaultInitProc()中调用
function SetFormSubmit()
{
    for(i=0 ; i<document.forms.length ; i++)
    {
        var form = document.forms[i];
        if(form.onsubmit==null)
            form.onsubmit = DefaultSubmitProc;
    }
}

//默认的form处理函数
function DefaultSubmitProc()
{
    // 如果定久了FormSubmitOtherCheck函数，调用它，做除了默认检查以外的检查。
    // 把这句放在前，可以少免去部分不必要的检查。
    // February 04, 2007
    if( (typeof FormSubmitOtherCheck == 'function') && (! FormSubmitOtherCheck(this)) )
    {
        return false;
    }
    var element = "";
    //检查各个输入框
    for(var i=0; (element = this.elements[i]); i++)
    {
        if(element.disabled || 'hidden' == element.type)//如果该控件是无效的，跳过
        {
            continue;
        }
        else if(!NotEmpty(element))
        {
            if('file' != element.type)
            {
                element.value = "这里不能为空"; // 设置该值，若是file类型会抛出异常；[2008-10-23]
            }
            alert("这里不能为空");
            break;
        }
        else if(CheckClass(element, "FLOAT") && !isFloat(element.value))
        {
            alert('这里只能填实数，如1.2、10、0.23等。');
            break;
        }
        else if(CheckClass(element, "USERNAME") && !isUserName(element.value))
        {
            alert('含有非法字符。\r\n\r\n注意，这里不能含有下列字符之一：\r\n\r\n\\ / : * ? " < > |   \'');
            break;
        }
    }

    if(element)
    {
        SetError(element);//给控件加上错误提示风格
        if(typeof element.select == 'function')element.select();
        if(typeof element.focus == 'function')element.focus();
        return false;
    }

    return true;
}


/*********************************************************
* 检查控件的class属性中是否含有classStr风格              *
**********************************************************/
function CheckClass(obj, classStr)
{
    if(obj == null)
        return true;
    if(obj.className.indexOf(classStr) != -1)
        return true;
    else
        return false;
}

/*********************************************************
* 如果该控件的class中没有'EMPTY'，则不能取空字符         *
* 串否则返回false                                        *
*********************************************************/
function NotEmpty(obj)
{
    if(obj==null)
        return true;
    else if(obj.className.indexOf("NOTEMPTY")!=-1 && obj.value=='')
        return false;
    return true;
}

/*********************************************************
* 串中是否只含：\ / : * ? " < > | '                      *
*********************************************************/
function isUserName(str)
{
    return ''==str.replace(/([^\\\/:\*?\"<>\|\s']*)/g, '');
}

/*********************************************************
* 实数                                                   *
*********************************************************/
function isFloat(str)
{
    return ''==str.replace(/([0-9]+(.[0-9]+)?)/g, '');
}

///////////////////////////////////////////
//设置INPUT标检的风格，即对键盘鼠标的反应
function SetControlInitStyle()
{
    //设置form里的控件
    for(var i=0 ; i<document.forms.length ; i++)
    {
        for(j=0 ; j<document.forms[i].elements.length ; j++)
        {
            var obj = document.forms[i].elements[j];
            InsertFun(obj, "click", OnClickProc);
            InsertFun(obj, "keypress", OnKeyPressProc);
            InsertFun(obj, "keydown", OnKeyDownProc);
            InsertFun(obj, "keyup", OnKeyUpProc);
            InsertFun(obj, "mouseover", OnMouseOverProc);
            InsertFun(obj, "mouseout", OnMouseOutProc);
            InsertFun(obj, "focus", OnFocusProc);
            InsertFun(obj, "blur", OnBlurProc);

            SetButtonInitClass(obj);
        }
    }

    //处理特定标签
    SetTagAttrib();
}

function SetButtonInitClass(obj)
{
    if(obj == null)
        return;
    try
    {
        switch(obj.type)
        {
        case "submit":
        case "button":
        case "reset":
            AddStyle(obj, 'button');
            break;
        case "text":
        case "password":
        case "textarea":
            AddStyle(obj, 'input_text');
            if(obj.disabled)
                AddStyle(obj, 'disabled');
            if(obj.readOnly)
                AddStyle(obj, 'readonly');
            break;
        case "checkbox":
            AddStyle(obj, 'input_checkbox');
            break;
        default:
            break;
        }

        if(CheckClass(obj, "FOCUS"))
        {
            if(typeof obj.select == 'function')obj.select();
            if(typeof obj.select == 'function')obj.focus();
        }
    }
    catch(e)
    {
        g_error=e;
    }
}

//处理特定标签，如有其他处理，可在各页面重载这个函数
function SetTagAttrib()
{
    var ol = document.getElementsByTagName('ol');
    for(var j=0; j<ol.length; j++)
    {
        if(ol[j].className.indexOf('DISCOLOR') == -1)
        {
            continue;
        }
        var li = ol[j].getElementsByTagName('li');
        for(var i=0 ; i<li.length ; i++)
        {
            li[i].onmouseover = liMouseOver;
            li[i].onmouseout  = liMouseOut;
            li[i].ondblclick  = liDblClick;
        }
    }
    //<li>标签
    function liMouseOver()
    {
        this.className = 'li_mouseover';
        try{liDoSome1(this);}catch(e){g_error=e;}
    }
    function liMouseOut()
    {
        this.className = 'li_mouseout';
    }
    function liDblClick()
    {
        this.bChoice = this.bChoice?false:true;
        this.style.color = this.bChoice?'#E7D329':'';
        this.style.background = this.bChoice?'#858574':'';
        try{liDoSome2(this);}catch(e){g_error=e;}
    }
}

function OnClickProc()
{
    ClearError(this);
    return true;
}
function OnKeyPressProc()
{
}
function OnKeyDownProc()
{
    ClearError(this);
    if(9 == this.event.keyCode)
    {
        // 取出TABTO_id中的id字段值，如：
        //      [NOTEMPTY FOCUS TABTO_input_type_select input_text]
        var re = new RegExp("TABTO_([^ ]+)", "g");
        var arr = re.exec(this.className);
        if(null != arr)
        {
            EventStop(this.event);
            var o = $(RegExp.$1);
            o = o[0] || o; // 由name或id取对象
            o.focus();
            return false;
        }
    }
}
function OnKeyUpProc()
{
}

function OnMouseOverProc()
{
    // Rocky 2010-05-31 16:52:04
    //this.focus();
}

function OnMouseOutProc()
{
}

function OnFocusProc()
{
    if(typeof this.select == 'function')
    {
        this.select();
    }
}

function OnBlurProc()
{
}

//设置控件错误状态
function SetError(thisObj)
{
    if(thisObj==null)
        return;
    if(thisObj.noError == null)
        thisObj.noError = true; //指示本控件是否已存在错误，是为false

    if(thisObj.noError && thisObj.className.indexOf('error') < 0)
        thisObj.className = thisObj.className + ' error ';  //给控件加上错误提示风格
    thisObj.noError = false;
}

//清除控件错误状态
function ClearError(thisObj)
{
    if(thisObj==null || thisObj.noError)
        return;
    if(thisObj.className.indexOf('error') >= 0)
        thisObj.className = thisObj.className.replace(/(\s*error\s*)/g, ' ');
    thisObj.noError = true;
}

//清除form中所有控件错误状态
function ClearFormError(thisForm)
{
    for(var i=0 ; i<thisForm.length ; i++)
        ClearError(thisForm.elements[i]);
}

//给控件加一风格(即向class中添加)
function AddStyle(obj, style)
{
    if(null == obj)
        return;
    if(obj.className.indexOf(style) < 0)
        obj.className += ' ' + style;
}

//和AddStyle()相对应，去除风格
function DelStyle(obj, style)
{
    if(null == obj)
        return;
    var re = new RegExp(style);
    obj.className = obj.className.replace(re, ' ');
}

function LoginUser()
{
    if(window.location.href.indexOf("login") >= 0)
        return;
    var loginuser = GetObj('login_user');
    var users     = GetObj('user');
    var user_ip = GetObj('user_ip');
    if( null==loginuser || null==users ||
        null==user_ip || ''==user_ip.value ||
        null==users.value || ''==users.value)
        return;
    loginuser.innerHTML = '<span title="' + user_ip.value + '">' + users.value + '</span>，';
    g_user = users.value;
}

/* 设置在beginID和endID之间（范围）的控件是否可用(由dis指定, true设置为有效, false为无效)
 *
 * 修改[2008-10-02]：
 *      1. 指定不需设置（例外）的控件；
 *      2. 明确范围也包括beginID、beginID在内；
 */
function SetStatusRange(formID, beginID, endID, dis)
{
    var i = 0;
    var isBarring = [];
    var form = document.getElementById(formID);
    if(form == null)
        return;

    // 跳过beginID前的控件  [2008-10-02]
    while(null != form[i] && form[i].id != beginID)i++;

    // 设置指定的控件
    for(; i<form.length ; i++)
    {
        // 排除的控件
        if(CheckClass(form[i], "NOT_SET_STATUS"))
        {
            continue;
        }

        if(form[i].type == 'text')
        {
            dis ? DelStyle(form[i], 'disabled') : AddStyle(form[i], 'disabled');
        }
        form[i].disabled = !dis;

        // 已到达最后一个需要设置的控件，跳出；
        if(form[i].id == endID)
            break;
    }
}

//从Url中取keyword对应的value
function GetValFromUrl(keyword)
{
    var page_name   = window.location.href;
    var len         = page_name.lastIndexOf(keyword + '=');  //把“=”做为查找的一部分较保险
    var value       = '';
    if(len != -1)
    {
        var str = page_name.substr(len + keyword.length + 1);
        if((len=str.indexOf("&")) != -1 || (len=str.indexOf("#")) != -1)
            value = str.substr(0, len);
        else
            value = str.substr(0);
    }
    //alert('[' + value + ']');
    return value;
}

function OpenUrl(url)
{
    window.location.href = url;
}

function OpenPage(url)
{
    OpenUrl(url);
}

// 转到系统选择界面
function SelectSys()
{
    OpenPage("/cgi-bin/system_select.cgi");
}

function Logout()
{
    users     = GetObj('user');
    if(null == users || ''==users.value)
    {
        alert('您未登录！');
        return;
    }
    if(IsSure("您确定退出？"))
    {
        OpenUrl('login?logout=1');
    }
}

//由具体页面重载
function Help()
{
    var str =  '\n'
    str += '记事本2.8　欢迎交流　QQ:15586350\n';
    // str += '＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n';
    // str += '　快捷键列表:\n';
    // str += '　*　Alt+1 : 快捷留言：到达公司时间；　　　　\n';
    // str += '　*　Alt+2 : 快捷留言：离开公司时间；　\n';
    // str += '　*　Alt+B : 从编辑页面转到浏览页面；　\n';
    // str += '　*　Alt+E : 从浏览页面转到编辑页面；　\n';
    // str += '　*　Alt+Enter  : 转到预览页面；　 　　\n';
    // str += '　*　Ctrl+Enter : 编辑页面提交；　　 　';
    alert(str);
}

function IsSure(str)
{
    if(null == str)
        str = "确定吗？";
    return (confirm(str));
}

function OnOK(id)
{
    alert("library.js: OnOK() 已不使用，暂留，稍后删除[2008-11-01]");
}

function OnCancel(id)
{
    alert("library.js: OnCancel() 已不使用，暂留，稍后删除[2008-11-01]");
}

function OutErrMsg()
{
    var obj1 = GetObj('err_msg');//反台返回信息
    if(null == obj1)
        return;
    var obj2 = GetObj('error');
    if(null != obj2 && '*' != obj1.value.charAt(0))
        obj2.innerHTML = obj1.value;
    else
    {
        var str = obj1.value;
        str = str.replace(/\\r/g, "\r");
        str = str.replace(/\\n/g, "\n");
        if('*' == str.charAt(0))
            str = str.substr(1);
        alert(str);
    }
}

//打开高级选项窗口
function Setting(str)
{
    alert("library.js: Setting() 已不使用，暂留，稍后删除[2008-11-01]");
}

function Esc()
{
    OnCancel()
}

function AltX()
{
    Logout();
}

//预加载
function Loadding()
{
    function Load()
    {
        try{LoadSetting();}catch(e){g_error=e;}
    }
    SetTimeout(Load, 1000);
}

// 加载页头的名言警句 [Rocky 2009-11-28 02:07:41]
function LoadCatchphrase()
{
    var xmlHttp = CreateXmlRequest();
    var catchphrase = GetObj('catchphrase');
    var url = "/cgi-bin/catchphrase?" + Math.random();// 使用了随机数，使每次请求都刷新缓存；

    // Rocky 2009-12-02 12:50:43
    if(null == xmlHttp || null == catchphrase)
    {
        return;
    }

    // 发送请求
    function Send()
    {
        xmlHttp.onreadystatechange = Deal;
        xmlHttp.open("get", url, true); // true 为异步处理返回的数据
        xmlHttp.send(null);
    }

    // 处理返回的数据
    function Deal()
    {
        if(xmlHttp.readyState == 4 && xmlHttp.status == 200)
        {
            catchphrase.innerHTML = xmlHttp.responseText;
        }
    }
    
    // 发送请求（取内容）
    Send();
}

//玩玩花样
function Flaring1()
{
    if(null == this.obj)
    {
        this.b = true;
        this.obj = $('bookmark_2');
        return;
    }
    this.obj.style.color = this.b?'#8C8A8E':'';
    this.b = !this.b;
}

//循环定时执行(1次/秒)，在clock.js中调用
function LoopRun()
{
    Flaring1();
    // 各个页面重载PageLoopRun，来定时执行代码，这样就不需要再起一个定时器了。
    if(typeof PageLoopRun == 'function')
    {
        PageLoopRun();
    }
}

/*
 *  功能：把控件src_id移到dest_id的下方。
 *  日期：2008-10-02 21:33:10
 */
function MoveToObjBelow(src_id, dest_id)
{
    var dest = GetPosition(dest_id);
    var src = GetObj(src_id);

    if(null == dest || null == src)
    {
        return;
    }
    src.style.left = dest.x + 1 + 'px';
    src.style.top = dest.y + dest.h + 1 + 'px';
}

/*
 *  功能：显示(yes=true)或隐藏(yes=false)由id指定的控件；
 *        注意，控件必须是 .style.visibility 设置的。
 *  日期：2008-10-03 15:49:15
 */
function ShowObj(id, yes/*=true*/)
{
    var obj = GetObj(id);

    if(null == obj)
    {
        return;
    }
    if(null == yes)
    {
        yes = true;
    }
    obj.style.visibility = yes ? "visible" : "hidden";
}

/*
 *  功能：标签页设置
 *  编写：Rocky 2008-10-19
 *  用法：
 *        SetTagBox( {head:"H1", body:"B1", fun:TagDo, on:true},
 *                  {head:"H2", body:"B2",},
 *                  {head:"H3", body:"B3"},
 *                  {head:"H4", body:"B4"},
 *                  {head:"H5", body:"B5"});
 *       on:true -- 设置该标签为显示
 *       fun:TagDo -- 指定点击标签头时执行的函数
 */
function SetTagBox()
{
    var tags = arguments;
    var prev = '';  // 记录上次打开的标签页

    Init();

    function Init()
    {
        if(0 == tags.length)
        {
            return false;
        }

        for(var i=0, tag=''; (tag = tags[i]); i++)
        {
            var head = $(tag.head);

            head.body = $(tag.body);
            head.fun = (typeof tag.fun == 'function') ? tag.fun : function(){};
            head.tag = tag;
            head.onclick = function()
            {
                if(this == this.prev)
                {
                    return;
                }
                // 先关掉前次的标签
                if('' != prev)
                {
                    prev.className = '';
                    prev.body.className = '';
                }
                // 打开当前点击的标签
                this.className = 'cur';
                this.body.className = 'cur';
                this.blur();
                this.fun(this.tag, tags); // 执行外部函数（回调）
                prev = this;
            }

            if(tag.on)
            {
                head.onclick();
            }
        }

        if('' == prev)  // 默认显示第一页
        {
            $(tags[0].head).onclick();
        }
    }
}


// 改由Bookmark.js中实现
///*
// *  功能：设置书签控制
// *  编写：Rocky 2008-10-23
// */
//function SetBookmark(button_id, form_data_id)
//{
//    var button = $(button_id);
//    var data = $(form_data_id);
//    var pos = [];
//    var index = 0;
//
//    button.up = '';     // 上一个书签按钮
//    button.add = '';    // 添加、清空书签按钮
//    button.down = '';   // 下一个书签按钮
//
//
//    if(null == button || null == data)
//    {
//        return false;
//    }
//
//    Init();
//
//
//    // 跳到上一位置
//    function Prev()
//    {
//        if(index > 0)
//        {
//            window.scrollTo(0, pos[ --index ].y);
//            AdjustPrompt();
//        }
//    }
//
//    // 跳到下一位置
//    function Next()
//    {
//        if(pos.length > 0 && index < pos.length-1)
//        {
//            window.scrollTo(0, pos[ ++index ].y);
//            AdjustPrompt();
//        }
//    }
//
//    // 跳到当前书签
//    function Cur()
//    {
//        if(pos && pos[index])
//        {
//            window.scrollTo(0, pos[index].y);
//            AdjustPrompt();
//        }
//    }
//
//    // 到最顶
//    function Top()
//    {
//        window.scrollTo(0, 0);
//    }
//
//    // 到最底
//    function Bottom()
//    {
//        window.scrollTo(0, document.body.scrollHeight);
//    }
//
//    // 提交到服务器
//    function Submit()
//    {
//        // 提交
//        data.form.submit();
//    }
//
//    // 当前文档位置记入书签
//    function Add()
//    {
//        var str = GetSelectText() || "[空]";
//        if(str.length > 80)
//        {
//            var n = str.length - 80;
//            str = str.substr(0, 80);
//            str += "...... [略去" + n + "字节]";
//        }
//        str = str.replace(/[\s]/g, " ");
//
//        /*
//         *  用页面上选取的文本作为书签说明
//         *     如果还是当书签，则替换当前的说明为这次选定的；
//         *     否则新增到书签列表；
//         */
//        if(pos[index] && document.body.scrollTop == pos[index].y)
//        {
//            pos[index].title = str;
//        }
//        else
//        {
//            // 先创建一空位置
//            pos.push({});
//            // 指向当前书签
//            index = pos.length - 1;
//            // 填充值
//            pos[index].y = document.body.scrollTop;
//            pos[index].title = str;
//            pos[index].create = GetKeyStr();
//        }
//        data.value = ToStr(pos);
//        AdjustPrompt();
//        Submit();
//        window.status = '增加书签成功';
//    }
//
//    // 册除当前书签
//    function Del()
//    {
//        if(!pos || !pos[index])
//        {
//            return;
//        }
//        if(!window.confirm("删除编号为[" + (index + 1) + "]的书签，确定吗？"))
//        {
//            return;
//        }
//        // 删除
//        delete pos[index];
//        // 刷新
//        data.value = ToStr(pos);
//        pos = ToArray(data.value);
//        index = pos[0] ? pos.length - 1 : 0;
//        AdjustPrompt();
//        window.status = '删除成功';
//        Submit();
//    }
//
//    // 清除所有书签
//    function Clear()
//    {
//        if(window.confirm("清除所有书签，确定吗？"))
//        {
//            // 清除数据
//            data.value = "";
//            pos = [];
//            index = 0;
//            AdjustPrompt();
//            Submit();
//            window.status = '已清除所有书签';
//        }
//    }
//
//    // 修改当前书签标注
//    function Modify()
//    {
//        if(!pos || !pos[index])
//        {
//            return;
//        }
//        var str = window.prompt("修改当前书签[" + (index + 1) + "]标注，请输入新的说明：", pos[index].title);
//        if(!str)
//        {
//            return;
//        }
//        // 修改
//        pos[index].title = str;
//        // 刷新
//        data.value = ToStr(pos);
//        AdjustPrompt();
//        window.status = '修改成功';
//        Submit();
//    }
//
//    // 调整（设置）书签提示
//    function AdjustPrompt()
//    {
//        if(pos.length == 0)
//        {
//            button.add.title = "无书签。[左击]：跳到当前书签；[Ctrl+左击]：修改；[右击]：把当前加入书签；[Ctrl+右击]：删除；[Ctrl+Alt+右击]：清除所有；";
//            button.up.title = "[左击]：跳到上一个书签处；[右击]：跳到最顶端；";
//            button.down.title = "[左击]：跳到下一个书签处；[右击]：跳到最底端；";
//            return;
//        }
//        var n = (index + 1);
//        button.up.title = '到上个书签';
//        button.add.title = n + '/' + pos.length + ': ' + pos[index].title;
//        button.down.title = '到下个书签';
//        window.status = '[书签]'
//                        + '  当前: ' + pos[index].title
//                        + '  上一个: ' + (pos[index - 1] ? pos[index - 1].title : '[空]')
//                        + '  下一个: ' + (pos[index + 1] ? pos[index + 1].title : '[空]');
//    }
//
//    // 调整位置（定位于左侧）
//    function AdjustPos()
//    {
//        var nWinHeight = document.body.clientHeight;
//        var nObjHeight = button.offsetHeight;
//        var nTop = (nWinHeight - nObjHeight) /2 + document.body.scrollTop;
//        button.style.top = nTop + 'px';
//
//    }
//
//    // 从串到数组
//    function ToArray(str)
//    {
//        var ary = [];
//        if("" == str)
//        {
//            str = "[]";
//        }
//        try     // 确保当数据有误时js也是可用的。 [2008-11-06 22:13:10]
//        {
//            eval("ary=" + str);
//        }
//        catch(e)
//        {
//            ary = [];
//            alert('书签数据出错，清除书签。');
//        }
//        return ary;
//    }
//
//    // 从数组到串
//    function ToStr(ary)
//    {
//        var i, e;
//        var str="[";
//        var s = '';
//        for(i in ary)
//        {
//            e=ary[i];
//            str += '{';
//            for(var o in e)
//            {
//                // 转义\        [2008-11-06 22:07:49]
//                s = (e[o] + "").replace(/\\/g, "\\\\");
//                // 转义'
//                s = s.replace(/'/g, "\\'");
//                str += "'" + o + "':'" + s + "',";
//            }
//            str = str.replace(/,$/, '');    // 去掉最后的‘,’
//            str += "},";
//        }
//        str = str.replace(/,$/, '');    // 去掉最后的‘,’
//        str += "]";
//
//        return str;
//    }
//
//    /*
//     *  功能：获取页面上选中的文本
//     *  编写：2008-10-26 22:28:31
//     *  说明：由网上整理而来
//     */
//    function GetSelectText()
//    {
//        if (window.getSelection)
//        {
//            return window.getSelection().toString();
//        }
//        else if (document.getSelection)
//        {
//            return document.getSelection();
//        }
//        else if (document.selection)
//        {
//            return document.selection.createRange().text;
//        }
//    }
//
//    // 初始化
//    function Init()
//    {
//        // 读入已有书签记录
//        pos = ToArray(data.value);
//        index = pos[0] ? pos.length - 1 : 0;
//
//        // 初始位置
//        AdjustPos();
//        button.style.visibility = "visible";
//
//        // 设置滚动窗口的动作
//        InsertFun(window, "scroll", AdjustPos);
//
//        // 设置单击动作
//        var a = button.getElementsByTagName('a');
//        button.up = a[0];
//        button.add = a[1];
//        button.down = a[2];
//
//        // 上
//        button.up.onclick = function()
//        {
//            Prev();
//            return false;
//        }
//        button.up.oncontextmenu = function(event)
//        {
//            event = event || window.event;
//            EventStop(event);   // 中止事件传递
//            Top();
//            return false;
//        }
//        /*
//         *  书签中键定义:
//         *      1. 单击跳到当前书签
//         *      2. 按下Ctrl后左击则为修改当前标注
//         *      3. 右击增加书签（并以页面上选定的文字为标注）
//         *      4. 按下Ctrl后右周则为删除当前书签
//         */
//        button.add.onclick = function(event)
//        {
//            event = event || window.event;
//            if(event.ctrlKey)
//            {
//                // 修改当前书签标注
//                Modify();
//            }
//            else
//            {
//                // 跳到当前书签处
//                Cur();
//            }
//            return false;
//        }
//        button.add.oncontextmenu = function(event)
//        {
//            event = event || window.event;
//            EventStop(event);   // 中止事件传递
//            if(event.ctrlKey && event.altKey)
//            {
//                // 清除所有书签
//                Clear();
//            }
//            else if(event.ctrlKey)
//            {
//                // 删除当前书签
//                Del();
//            }
//            else
//            {
//                // 加入书签
//                Add();
//            }
//            return false;
//        }
//
//        // 下
//        button.down.onclick = function()
//        {
//            Next();
//            return false;
//        }
//        button.down.oncontextmenu = function(event)
//        {
//            event = event || window.event;
//            EventStop(event);   // 中止事件传递
//            Bottom();
//            return false;
//        }
//
//        // 设置书签提示
//        AdjustPrompt();
//
//        // 跳到当前书签
//        Cur();
//    }
//}


/*
 * 功能：设置控件的显示值
 * 编写：Rocky 2009-11-28 01:01:50
 */
function SetInnerHTML(objID, value)
{
    var tmp = document.getElementById(objID);
    if(null != tmp)
    {
        tmp.innerHTML = value;
    }
}
