/************************************************************
*                                                           *
*   作者: rocky                                             *
*   QQ  : 15586350                                          *
*   功能: 页面总控模块                                      *
*   日期: 2006-3                                            *
*   修改:                                                   *
*   版本: 2.0                                               *
*                                                           *
************************************************************/

var g_error = '';
var g_user  = '';//当前登录的用户
var g_isIE  = ('Microsoft Internet Explorer'==navigator.appName?true:false);
var g_bShortKeyOk = true; //快捷键是否有效
var g_PageName = '';


function DefaultInitProc()
{
    Version();                      // 浏览器版本判断
    Tips();                         // 提示窗口
    MenuInit('menu');
    //MonthCalendar();                // 创建日期控件
    //Calculator();                   // 计算器
    LoginUser();
    OutErrMsg();                    // 输出信息
    Online();                       // 在线检测
    Loadding();                     // 预加载
    // Jump("scroll_up_down");      // 初始化转跳
    SetClock('clock');              // 设置显示时钟
    SetFormSubmit();                // 设置默认的form提交函数
    SetControlInitStyle();          // 设置INPUT标检的风格，即对键盘鼠标的反应
    // SetMouseEvent();             // 设置全局单击事件[Mouse.js]
    ShortKey();                     // 键盘快捷键初始化
    Logo('logo_img');               // 装饰logo标志
    LoadCatchphrase();              // 加载页头的名言警句
    
    return true;
}

function Init()
{
    DefaultInitProc();
    return true;
}


str = '';
str += '<meta http-equiv="Content-Type" content="text/html; Charset=utf-8">';
str += '<body id="_body" OnLoad="Init()">';
if(document.title != '')
{
    // Rocky 2009-11-25 12:22:47
    str += '<table width=100% border=0 cellpadding=0 cellspacing=0>';
    str += '  <tr>  ';
    str += '    <td width=17% valign=top rowspan="2" style="height:76px;"><img id="logo_img" src="/general/logo.gif" border=0 width=90><font id=title_item >记事本</font><font id=title_item2>记事本</font></td>';
    str += '    <td align=right valign=top colspan="2">&nbsp;</td>';
    str += '    </td>';
    str += '  </tr>';
    str += '  <tr>';
    str += '    <td align=center valign=bottom><font color="#57514A"><i><span id="catchphrase">正在加载名言警句 ......</span></i></font></td>';
    str += '    <td align=right valign=top width=17%><span id=login_user></span><font class=welcome>欢迎您 !</font><br>&nbsp;&nbsp;&nbsp;<span id="clock"></span></td>';
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

    str += '<table width="200" border=0 align=right cellpadding=0 style="position:absolute;width:16%;z-index:30;right:2%;">';
    str += '    <tr>';
    str += '        <td width=98% height=18 align=right colspan="2">';

    str += '        <input id="operator" type="button" class="button hand hidden" onClick="MenuInit.MenuShow();" onMouseOut="MenuInit.Menu();" value="菜单">';
    //弹出式菜单
    str += '        <table id=menu class="hidden" border=0 align=right cellpadding=0>';
    str += '        <tr><td>';
    str += '        <span id=menu_html></span>';
    str += '        <a href="javascript:Logout();">注销　Alt+X</a>';
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
document.write('<script language=javascript src="/OverLayer.js"><\/script>\n');
document.write('<script language=javascript src="/ShowToCenter.js"><\/script>\n');
document.write('<script language=javascript src="/Keyboard.js"><\/script>\n');
document.write('<script language=javascript src="/Menu.js"><\/script>\n');
document.write('<script language=javascript src="/Bookmark.js"><\/script>\n');
document.write('<script language=javascript src="/Logo.js"><\/script>\n');
document.write('<script language=javascript src="/Tips.js"><\/script>\n');      // [Rocky 2009-12-08 01:04:48]
document.write('<script language=javascript src="/Online.js"><\/script>\n');    // [Rocky 2010-11-18 17:58:40]

