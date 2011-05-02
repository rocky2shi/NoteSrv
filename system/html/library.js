/************************************************************
*                                                           *
*   ����: rocky                                             *
*   QQ  : 15586350                                          *
*   ����: ����ʵ��                                          *
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
//�˵���������
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
    Init();//��ʼ��
}


/***********************************************************/
//form��������

//����Ĭ�ϵ�form�ύ��������DefaultInitProc()�е���
function SetFormSubmit()
{
    for(i=0 ; i<document.forms.length ; i++)
    {
        var form = document.forms[i];
        if(form.onsubmit==null)
            form.onsubmit = DefaultSubmitProc;
    }
}

//Ĭ�ϵ�form������
function DefaultSubmitProc()
{
    // ���������FormSubmitOtherCheck��������������������Ĭ�ϼ������ļ�顣
    // ��������ǰ����������ȥ���ֲ���Ҫ�ļ�顣
    // February 04, 2007
    if( (typeof FormSubmitOtherCheck == 'function') && (! FormSubmitOtherCheck(this)) )
    {
        return false;
    }
    var element = "";
    //�����������
    for(var i=0; (element = this.elements[i]); i++)
    {
        if(element.disabled || 'hidden' == element.type)//����ÿؼ�����Ч�ģ�����
        {
            continue;
        }
        else if(!NotEmpty(element))
        {
            if('file' != element.type)
            {
                element.value = "���ﲻ��Ϊ��"; // ���ø�ֵ������file���ͻ��׳��쳣��[2008-10-23]
            }
            alert("���ﲻ��Ϊ��");
            break;
        }
        else if(CheckClass(element, "FLOAT") && !isFloat(element.value))
        {
            alert('����ֻ����ʵ������1.2��10��0.23�ȡ�');
            break;
        }
        else if(CheckClass(element, "USERNAME") && !isUserName(element.value))
        {
            alert('���зǷ��ַ���\r\n\r\nע�⣬���ﲻ�ܺ��������ַ�֮һ��\r\n\r\n\\ / : * ? " < > |   \'');
            break;
        }
    }

    if(element)
    {
        SetError(element);//���ؼ����ϴ�����ʾ���
        if(typeof element.select == 'function')element.select();
        if(typeof element.focus == 'function')element.focus();
        return false;
    }

    return true;
}


/*********************************************************
* ���ؼ���class�������Ƿ���classStr���              *
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
* ����ÿؼ���class��û��'EMPTY'������ȡ���ַ�         *
* �����򷵻�false                                        *
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
* �����Ƿ�ֻ����\ / : * ? " < > | '                      *
*********************************************************/
function isUserName(str)
{
    return ''==str.replace(/([^\\\/:\*?\"<>\|\s']*)/g, '');
}

/*********************************************************
* ʵ��                                                   *
*********************************************************/
function isFloat(str)
{
    return ''==str.replace(/([0-9]+(.[0-9]+)?)/g, '');
}

///////////////////////////////////////////
//����INPUT���ķ�񣬼��Լ������ķ�Ӧ
function SetControlInitStyle()
{
    //����form��Ŀؼ�
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

    //�����ض���ǩ
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

//�����ض���ǩ�����������������ڸ�ҳ�������������
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
    //<li>��ǩ
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
        // ȡ��TABTO_id�е�id�ֶ�ֵ���磺
        //      [NOTEMPTY FOCUS TABTO_input_type_select input_text]
        var re = new RegExp("TABTO_([^ ]+)", "g");
        var arr = re.exec(this.className);
        if(null != arr)
        {
            EventStop(this.event);
            var o = $(RegExp.$1);
            o = o[0] || o; // ��name��idȡ����
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

//���ÿؼ�����״̬
function SetError(thisObj)
{
    if(thisObj==null)
        return;
    if(thisObj.noError == null)
        thisObj.noError = true; //ָʾ���ؼ��Ƿ��Ѵ��ڴ�����Ϊfalse

    if(thisObj.noError && thisObj.className.indexOf('error') < 0)
        thisObj.className = thisObj.className + ' error ';  //���ؼ����ϴ�����ʾ���
    thisObj.noError = false;
}

//����ؼ�����״̬
function ClearError(thisObj)
{
    if(thisObj==null || thisObj.noError)
        return;
    if(thisObj.className.indexOf('error') >= 0)
        thisObj.className = thisObj.className.replace(/(\s*error\s*)/g, ' ');
    thisObj.noError = true;
}

//���form�����пؼ�����״̬
function ClearFormError(thisForm)
{
    for(var i=0 ; i<thisForm.length ; i++)
        ClearError(thisForm.elements[i]);
}

//���ؼ���һ���(����class�����)
function AddStyle(obj, style)
{
    if(null == obj)
        return;
    if(obj.className.indexOf(style) < 0)
        obj.className += ' ' + style;
}

//��AddStyle()���Ӧ��ȥ�����
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
    loginuser.innerHTML = '<span title="' + user_ip.value + '">' + users.value + '</span>��';
    g_user = users.value;
}

/* ������beginID��endID֮�䣨��Χ���Ŀؼ��Ƿ����(��disָ��, true����Ϊ��Ч, falseΪ��Ч)
 *
 * �޸�[2008-10-02]��
 *      1. ָ���������ã����⣩�Ŀؼ���
 *      2. ��ȷ��ΧҲ����beginID��beginID���ڣ�
 */
function SetStatusRange(formID, beginID, endID, dis)
{
    var i = 0;
    var isBarring = [];
    var form = document.getElementById(formID);
    if(form == null)
        return;

    // ����beginIDǰ�Ŀؼ�  [2008-10-02]
    while(null != form[i] && form[i].id != beginID)i++;

    // ����ָ���Ŀؼ�
    for(; i<form.length ; i++)
    {
        // �ų��Ŀؼ�
        if(CheckClass(form[i], "NOT_SET_STATUS"))
        {
            continue;
        }

        if(form[i].type == 'text')
        {
            dis ? DelStyle(form[i], 'disabled') : AddStyle(form[i], 'disabled');
        }
        form[i].disabled = !dis;

        // �ѵ������һ����Ҫ���õĿؼ���������
        if(form[i].id == endID)
            break;
    }
}

//��Url��ȡkeyword��Ӧ��value
function GetValFromUrl(keyword)
{
    var page_name   = window.location.href;
    var len         = page_name.lastIndexOf(keyword + '=');  //�ѡ�=����Ϊ���ҵ�һ���ֽϱ���
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

// ת��ϵͳѡ�����
function SelectSys()
{
    OpenPage("/cgi-bin/system_select.cgi");
}

function Logout()
{
    users     = GetObj('user');
    if(null == users || ''==users.value)
    {
        alert('��δ��¼��');
        return;
    }
    if(IsSure("��ȷ���˳���"))
    {
        OpenUrl('login?logout=1');
    }
}

//�ɾ���ҳ������
function Help()
{
    var str =  '\n'
    str += '���±�2.8����ӭ������QQ:15586350\n';
    // str += '����������������������������������������\n';
    // str += '����ݼ��б�:\n';
    // str += '��*��Alt+1 : ������ԣ����﹫˾ʱ�䣻��������\n';
    // str += '��*��Alt+2 : ������ԣ��뿪��˾ʱ�䣻��\n';
    // str += '��*��Alt+B : �ӱ༭ҳ��ת�����ҳ�棻��\n';
    // str += '��*��Alt+E : �����ҳ��ת���༭ҳ�棻��\n';
    // str += '��*��Alt+Enter  : ת��Ԥ��ҳ�棻�� ����\n';
    // str += '��*��Ctrl+Enter : �༭ҳ���ύ������ ��';
    alert(str);
}

function IsSure(str)
{
    if(null == str)
        str = "ȷ����";
    return (confirm(str));
}

function OnOK(id)
{
    alert("library.js: OnOK() �Ѳ�ʹ�ã��������Ժ�ɾ��[2008-11-01]");
}

function OnCancel(id)
{
    alert("library.js: OnCancel() �Ѳ�ʹ�ã��������Ժ�ɾ��[2008-11-01]");
}

function OutErrMsg()
{
    var obj1 = GetObj('err_msg');//��̨������Ϣ
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

//�򿪸߼�ѡ���
function Setting(str)
{
    alert("library.js: Setting() �Ѳ�ʹ�ã��������Ժ�ɾ��[2008-11-01]");
}

function Esc()
{
    OnCancel()
}

function AltX()
{
    Logout();
}

//Ԥ����
function Loadding()
{
    function Load()
    {
        try{LoadSetting();}catch(e){g_error=e;}
    }
    SetTimeout(Load, 1000);
}

// ����ҳͷ�����Ծ��� [Rocky 2009-11-28 02:07:41]
function LoadCatchphrase()
{
    var xmlHttp = CreateXmlRequest();
    var catchphrase = GetObj('catchphrase');
    var url = "/cgi-bin/catchphrase?" + Math.random();// ʹ�����������ʹÿ������ˢ�»��棻

    // Rocky 2009-12-02 12:50:43
    if(null == xmlHttp || null == catchphrase)
    {
        return;
    }

    // ��������
    function Send()
    {
        xmlHttp.onreadystatechange = Deal;
        xmlHttp.open("get", url, true); // true Ϊ�첽�����ص�����
        xmlHttp.send(null);
    }

    // �����ص�����
    function Deal()
    {
        if(xmlHttp.readyState == 4 && xmlHttp.status == 200)
        {
            catchphrase.innerHTML = xmlHttp.responseText;
        }
    }
    
    // ��������ȡ���ݣ�
    Send();
}

//���滨��
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

//ѭ����ʱִ��(1��/��)����clock.js�е���
function LoopRun()
{
    Flaring1();
    // ����ҳ������PageLoopRun������ʱִ�д��룬�����Ͳ���Ҫ����һ����ʱ���ˡ�
    if(typeof PageLoopRun == 'function')
    {
        PageLoopRun();
    }
}

/*
 *  ���ܣ��ѿؼ�src_id�Ƶ�dest_id���·���
 *  ���ڣ�2008-10-02 21:33:10
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
 *  ���ܣ���ʾ(yes=true)������(yes=false)��idָ���Ŀؼ���
 *        ע�⣬�ؼ������� .style.visibility ���õġ�
 *  ���ڣ�2008-10-03 15:49:15
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
 *  ���ܣ���ǩҳ����
 *  ��д��Rocky 2008-10-19
 *  �÷���
 *        SetTagBox( {head:"H1", body:"B1", fun:TagDo, on:true},
 *                  {head:"H2", body:"B2",},
 *                  {head:"H3", body:"B3"},
 *                  {head:"H4", body:"B4"},
 *                  {head:"H5", body:"B5"});
 *       on:true -- ���øñ�ǩΪ��ʾ
 *       fun:TagDo -- ָ�������ǩͷʱִ�еĺ���
 */
function SetTagBox()
{
    var tags = arguments;
    var prev = '';  // ��¼�ϴδ򿪵ı�ǩҳ

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
                // �ȹص�ǰ�εı�ǩ
                if('' != prev)
                {
                    prev.className = '';
                    prev.body.className = '';
                }
                // �򿪵�ǰ����ı�ǩ
                this.className = 'cur';
                this.body.className = 'cur';
                this.blur();
                this.fun(this.tag, tags); // ִ���ⲿ�������ص���
                prev = this;
            }

            if(tag.on)
            {
                head.onclick();
            }
        }

        if('' == prev)  // Ĭ����ʾ��һҳ
        {
            $(tags[0].head).onclick();
        }
    }
}


// ����Bookmark.js��ʵ��
///*
// *  ���ܣ�������ǩ����
// *  ��д��Rocky 2008-10-23
// */
//function SetBookmark(button_id, form_data_id)
//{
//    var button = $(button_id);
//    var data = $(form_data_id);
//    var pos = [];
//    var index = 0;
//
//    button.up = '';     // ��һ����ǩ��ť
//    button.add = '';    // ��ӡ������ǩ��ť
//    button.down = '';   // ��һ����ǩ��ť
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
//    // ������һλ��
//    function Prev()
//    {
//        if(index > 0)
//        {
//            window.scrollTo(0, pos[ --index ].y);
//            AdjustPrompt();
//        }
//    }
//
//    // ������һλ��
//    function Next()
//    {
//        if(pos.length > 0 && index < pos.length-1)
//        {
//            window.scrollTo(0, pos[ ++index ].y);
//            AdjustPrompt();
//        }
//    }
//
//    // ������ǰ��ǩ
//    function Cur()
//    {
//        if(pos && pos[index])
//        {
//            window.scrollTo(0, pos[index].y);
//            AdjustPrompt();
//        }
//    }
//
//    // ���
//    function Top()
//    {
//        window.scrollTo(0, 0);
//    }
//
//    // �����
//    function Bottom()
//    {
//        window.scrollTo(0, document.body.scrollHeight);
//    }
//
//    // �ύ��������
//    function Submit()
//    {
//        // �ύ
//        data.form.submit();
//    }
//
//    // ��ǰ�ĵ�λ�ü�����ǩ
//    function Add()
//    {
//        var str = GetSelectText() || "[��]";
//        if(str.length > 80)
//        {
//            var n = str.length - 80;
//            str = str.substr(0, 80);
//            str += "...... [��ȥ" + n + "�ֽ�]";
//        }
//        str = str.replace(/[\s]/g, " ");
//
//        /*
//         *  ��ҳ����ѡȡ���ı���Ϊ��ǩ˵��
//         *     ������ǵ���ǩ�����滻��ǰ��˵��Ϊ���ѡ���ģ�
//         *     ������������ǩ�б�
//         */
//        if(pos[index] && document.body.scrollTop == pos[index].y)
//        {
//            pos[index].title = str;
//        }
//        else
//        {
//            // �ȴ���һ��λ��
//            pos.push({});
//            // ָ��ǰ��ǩ
//            index = pos.length - 1;
//            // ���ֵ
//            pos[index].y = document.body.scrollTop;
//            pos[index].title = str;
//            pos[index].create = GetKeyStr();
//        }
//        data.value = ToStr(pos);
//        AdjustPrompt();
//        Submit();
//        window.status = '������ǩ�ɹ�';
//    }
//
//    // �����ǰ��ǩ
//    function Del()
//    {
//        if(!pos || !pos[index])
//        {
//            return;
//        }
//        if(!window.confirm("ɾ�����Ϊ[" + (index + 1) + "]����ǩ��ȷ����"))
//        {
//            return;
//        }
//        // ɾ��
//        delete pos[index];
//        // ˢ��
//        data.value = ToStr(pos);
//        pos = ToArray(data.value);
//        index = pos[0] ? pos.length - 1 : 0;
//        AdjustPrompt();
//        window.status = 'ɾ���ɹ�';
//        Submit();
//    }
//
//    // ���������ǩ
//    function Clear()
//    {
//        if(window.confirm("���������ǩ��ȷ����"))
//        {
//            // �������
//            data.value = "";
//            pos = [];
//            index = 0;
//            AdjustPrompt();
//            Submit();
//            window.status = '�����������ǩ';
//        }
//    }
//
//    // �޸ĵ�ǰ��ǩ��ע
//    function Modify()
//    {
//        if(!pos || !pos[index])
//        {
//            return;
//        }
//        var str = window.prompt("�޸ĵ�ǰ��ǩ[" + (index + 1) + "]��ע���������µ�˵����", pos[index].title);
//        if(!str)
//        {
//            return;
//        }
//        // �޸�
//        pos[index].title = str;
//        // ˢ��
//        data.value = ToStr(pos);
//        AdjustPrompt();
//        window.status = '�޸ĳɹ�';
//        Submit();
//    }
//
//    // ���������ã���ǩ��ʾ
//    function AdjustPrompt()
//    {
//        if(pos.length == 0)
//        {
//            button.add.title = "����ǩ��[���]��������ǰ��ǩ��[Ctrl+���]���޸ģ�[�һ�]���ѵ�ǰ������ǩ��[Ctrl+�һ�]��ɾ����[Ctrl+Alt+�һ�]��������У�";
//            button.up.title = "[���]��������һ����ǩ����[�һ�]��������ˣ�";
//            button.down.title = "[���]��������һ����ǩ����[�һ�]��������׶ˣ�";
//            return;
//        }
//        var n = (index + 1);
//        button.up.title = '���ϸ���ǩ';
//        button.add.title = n + '/' + pos.length + ': ' + pos[index].title;
//        button.down.title = '���¸���ǩ';
//        window.status = '[��ǩ]'
//                        + '  ��ǰ: ' + pos[index].title
//                        + '  ��һ��: ' + (pos[index - 1] ? pos[index - 1].title : '[��]')
//                        + '  ��һ��: ' + (pos[index + 1] ? pos[index + 1].title : '[��]');
//    }
//
//    // ����λ�ã���λ����ࣩ
//    function AdjustPos()
//    {
//        var nWinHeight = document.body.clientHeight;
//        var nObjHeight = button.offsetHeight;
//        var nTop = (nWinHeight - nObjHeight) /2 + document.body.scrollTop;
//        button.style.top = nTop + 'px';
//
//    }
//
//    // �Ӵ�������
//    function ToArray(str)
//    {
//        var ary = [];
//        if("" == str)
//        {
//            str = "[]";
//        }
//        try     // ȷ������������ʱjsҲ�ǿ��õġ� [2008-11-06 22:13:10]
//        {
//            eval("ary=" + str);
//        }
//        catch(e)
//        {
//            ary = [];
//            alert('��ǩ���ݳ��������ǩ��');
//        }
//        return ary;
//    }
//
//    // �����鵽��
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
//                // ת��\        [2008-11-06 22:07:49]
//                s = (e[o] + "").replace(/\\/g, "\\\\");
//                // ת��'
//                s = s.replace(/'/g, "\\'");
//                str += "'" + o + "':'" + s + "',";
//            }
//            str = str.replace(/,$/, '');    // ȥ�����ġ�,��
//            str += "},";
//        }
//        str = str.replace(/,$/, '');    // ȥ�����ġ�,��
//        str += "]";
//
//        return str;
//    }
//
//    /*
//     *  ���ܣ���ȡҳ����ѡ�е��ı�
//     *  ��д��2008-10-26 22:28:31
//     *  ˵�����������������
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
//    // ��ʼ��
//    function Init()
//    {
//        // ����������ǩ��¼
//        pos = ToArray(data.value);
//        index = pos[0] ? pos.length - 1 : 0;
//
//        // ��ʼλ��
//        AdjustPos();
//        button.style.visibility = "visible";
//
//        // ���ù������ڵĶ���
//        InsertFun(window, "scroll", AdjustPos);
//
//        // ���õ�������
//        var a = button.getElementsByTagName('a');
//        button.up = a[0];
//        button.add = a[1];
//        button.down = a[2];
//
//        // ��
//        button.up.onclick = function()
//        {
//            Prev();
//            return false;
//        }
//        button.up.oncontextmenu = function(event)
//        {
//            event = event || window.event;
//            EventStop(event);   // ��ֹ�¼�����
//            Top();
//            return false;
//        }
//        /*
//         *  ��ǩ�м�����:
//         *      1. ����������ǰ��ǩ
//         *      2. ����Ctrl�������Ϊ�޸ĵ�ǰ��ע
//         *      3. �һ�������ǩ������ҳ����ѡ��������Ϊ��ע��
//         *      4. ����Ctrl��������Ϊɾ����ǰ��ǩ
//         */
//        button.add.onclick = function(event)
//        {
//            event = event || window.event;
//            if(event.ctrlKey)
//            {
//                // �޸ĵ�ǰ��ǩ��ע
//                Modify();
//            }
//            else
//            {
//                // ������ǰ��ǩ��
//                Cur();
//            }
//            return false;
//        }
//        button.add.oncontextmenu = function(event)
//        {
//            event = event || window.event;
//            EventStop(event);   // ��ֹ�¼�����
//            if(event.ctrlKey && event.altKey)
//            {
//                // ���������ǩ
//                Clear();
//            }
//            else if(event.ctrlKey)
//            {
//                // ɾ����ǰ��ǩ
//                Del();
//            }
//            else
//            {
//                // ������ǩ
//                Add();
//            }
//            return false;
//        }
//
//        // ��
//        button.down.onclick = function()
//        {
//            Next();
//            return false;
//        }
//        button.down.oncontextmenu = function(event)
//        {
//            event = event || window.event;
//            EventStop(event);   // ��ֹ�¼�����
//            Bottom();
//            return false;
//        }
//
//        // ������ǩ��ʾ
//        AdjustPrompt();
//
//        // ������ǰ��ǩ
//        Cur();
//    }
//}


/*
 * ���ܣ����ÿؼ�����ʾֵ
 * ��д��Rocky 2009-11-28 01:01:50
 */
function SetInnerHTML(objID, value)
{
    var tmp = document.getElementById(objID);
    if(null != tmp)
    {
        tmp.innerHTML = value;
    }
}
