/************************************************************
*                                                           *
*   ����: rocky                                             *
*   QQ  : 15586350                                          *
*   ����: ҳ���ܿ�ģ��                                      *
*   ����: 2006-3                                            *
*   �޸�:                                                   *
*   �汾: 2.0                                               *
*                                                           *
************************************************************/

var g_error = '';
var g_user  = '';//��ǰ��¼���û�
var g_isIE  = ('Microsoft Internet Explorer'==navigator.appName?true:false);
var g_bShortKeyOk = true; //��ݼ��Ƿ���Ч
var g_PageName = '';


function DefaultInitProc()
{
    Version();                      // ������汾�ж�
    Tips();                         // ��ʾ����
    MenuInit('menu');
    //MonthCalendar();                // �������ڿؼ�
    //Calculator();                   // ������
    LoginUser();
    OutErrMsg();                    // �����Ϣ
    LoginWin();                     // ��¼����
    Loadding();                     // Ԥ����
    // Jump("scroll_up_down");      // ��ʼ��ת��
    SetClock('clock');              // ������ʾʱ��
    SetFormSubmit();                // ����Ĭ�ϵ�form�ύ����
    SetControlInitStyle();          // ����INPUT���ķ�񣬼��Լ������ķ�Ӧ
    // SetMouseEvent();             // ����ȫ�ֵ����¼�[Mouse.js]
    ShortKey();                     // ���̿�ݼ���ʼ��
    Logo('logo_img');               // װ��logo��־
    LoadCatchphrase();              // ����ҳͷ�����Ծ���
    
    return true;
}

function Init()
{
    DefaultInitProc();
    return true;
}


str = '';
str += '<meta http-equiv="Content-Type" content="text/html; Charset=gb2312">';
str += '<body id="_body" OnLoad="Init()">';
if(document.title != '')
{
    // Rocky 2009-11-25 12:22:47
    str += '<table width=100% border=0 cellpadding=0 cellspacing=0>';
    str += '  <tr>  ';
    str += '    <td width=17% valign=top rowspan="2" style="height:76px;"><img id="logo_img" src="/general/logo.gif" border=0 width=90><font id=title_item >���±�</font><font id=title_item2>���±�</font></td>';
    str += '    <td align=right valign=top colspan="2">&nbsp;</td>';
    str += '    </td>';
    str += '  </tr>';
    str += '  <tr>';
    str += '    <td align=center valign=bottom><font color="#57514A"><i><span id="catchphrase">���ڼ������Ծ��� ......</span></i></font></td>';
    str += '    <td align=right valign=top width=17%><span id=login_user></span><font class=welcome>��ӭ�� !</font><br>&nbsp;&nbsp;&nbsp;<span id="clock"></span></td>';
    str += '  </tr>';
    str += '  <tr>';
    str += '    <td width=100% height=1 colspan=3></td>';
    str += '  </tr>';
    str += '  <tr>';
    str += '    <td bgcolor=#006500 width=100% height=1 colspan=3></td>';
    str += '  </tr>';
    str += '  <tr>';
    str += '    <td width=100% height=1 colspan=3></td>';
    str += '  </tr>';
    str += '</table>';

    str += '<table width="200" border=0 align=right cellpadding=0 style="position:absolute;width:16%;z-index:30;right:5%;">';
    str += '    <tr>';
    str += '        <td width=98% height=18 align=right colspan="2">';

    str += '        <input id="operator" type="button" class="button hand hidden" onClick="MenuInit.MenuShow();" onMouseOut="MenuInit.Menu();" value="�˵�">';
    //����ʽ�˵�
    str += '        <table id=menu class="hidden" border=0 align=right cellpadding=0>';
    str += '        <tr><td>';
    str += '        <span id=menu_html></span>';
    str += '        <a href="javascript:Logout();">ע����Alt+X</a>';
    str += '        </td></tr>';
    str += '        </table>';

    str += '        </td>';
    str += '    </tr>';
    str += '    <tr>';
    str += '        <td bgcolor="#F2E125" height=1 colspan="2"></td>';
    str += '    </tr>';
    str += '    <tr>';
    str += '        <td height=10 colspan="2"></td>';
    str += '    </tr>';
    str += '    <tr>';
    str += '        <td colspan="2"><hr align="right" width="56%" size=1 color="#FA8072"></td>';
    str += '    </tr>';
    str += '    <tr>';
    str += '        <td colspan="2"><hr align="right" width="25%" size=1 color="#0000FF"></td>';
    str += '    </tr>';
    str += '    <tr>';
    str += '        <td colspan="2"><hr align="right" width="12%" size=1 color="#ADD8E6"></td>';
    str += '    </tr>';
    str += '    <tr>';
    str += '        <td colspan="2"><hr align="right" width="5%" size=1 color="#000000"></td>';
    str += '    </tr>';
    str += '    <tr>';
    str += '        <td colspan="2"><hr align="right" width="2%" size=1 color="#FFFFFF"></td>';
    str += '    </tr>';
    str += '</table><br>';

    str += '<iframe name="tmp_iframe" id="tmp_iframe" style="display:none;"></iframe>';
}
document.write(str);

document.write('<script language=javascript src="/Error.js"><\/script>\n');
document.write('<script language=javascript src="/Common.js"><\/script>\n');
document.write('<link rel="stylesheet" href="/css.css" type="text/css">\n');
document.write('<script language=javascript src="/library.js"><\/script>\n');
document.write('<script language=javascript src="/clock.js"><\/script>\n');
//document.write('<script language=javascript src="/calendar.js"><\/script>\n');
//document.write('<script language=javascript src="/calculator.js"><\/script>\n');
document.write('<script language=javascript src="/OverLayer.js"><\/script>\n');
document.write('<script language=javascript src="/ShowToCenter.js"><\/script>\n');
document.write('<script language=javascript src="/Keyboard.js"><\/script>\n');
document.write('<script language=javascript src="/Menu.js"><\/script>\n');
document.write('<script language=javascript src="/Bookmark.js"><\/script>\n');
document.write('<script language=javascript src="/Logo.js"><\/script>\n');
document.write('<script language=javascript src="/Tips.js"><\/script>\n');  // [Rocky 2009-12-08 01:04:48]
document.write('<script language=javascript src="/Login.js"><\/script>\n'); // [Rocky 2010-05-29 17:13:05]

