/************************************************************
*                                                           *
*   功能   : 计算器                                         *
*   作者   : shizw                                          *
*   E-mail : mypyramid@126.com                              *
*   日期   : 2007-2-15                                      *
*   日期   :                                      			*
*   修改   :                                      			*
*   版本   : 0.1                                            *
*	主程序来自www.126.com邮箱的个人助理                     *
*                                                           *
************************************************************/



function Calculator()
{
	var THIS = Calculator;
	THIS.objWin = '';
	THIS.objFrame = '';
	THIS.Open = function()
	{
		if(null != THIS.objFrame && '' == THIS.objFrame.src)
		{
			THIS.objFrame.src = '/general/calculator.htm';
		}
		THIS.objWin.style.display = '';
	}
	THIS.Close = function()
	{
		THIS.objWin.style.display = 'none';
	}
	function Create()
	{
		var html = '';
		html += ' <div id="calendar_div" style="display:none;position:absolute;';
		html += '	right:80px;top:158px;';
		html += ' 	border:1px solid;background-color:#FA8072;z-index:8;">';
		html += ' <table><tr><td>';
		html += ' <iframe id="calculator_iframe" ';
		html += ' 	width="258" height="230" frameborder="0" scrolling="no">';
		html += ' </iframe>';
		html += ' </td></tr><tr><td align="right">';
		html += ' <hr><span class="comment">说明：本程序来自www.126.com</span>';
		html += ' <input type=button value="关闭" onClick="Calculator.Close()">';
		html += ' </td></tr></table>';
		html += ' </div>';
		//Print(html);
		GetObj('MonthCalculator').innerHTML = html;
	}
	function Init()
	{
		THIS.objWin = GetObj('calendar_div');
		if(null == THIS.objWin)
		{
			alert("计算器出错");
			return;
		}
		THIS.objFrame = GetObj('calculator_iframe');
		if(null == THIS.objFrame)
		{
			alert("取计算器载全出错");
			return;
		}
	}
	Create();
	Init();
}

// 母体
document.write('<span id="MonthCalculator"></span>');
