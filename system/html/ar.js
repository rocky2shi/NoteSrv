/************************************************************
*                                                           *
*   功能   : 日期设置控件                                   *
*   作者   : shizw                                          *
*   E-mail : mypyramid@126.com                              *
*   日期   : 2007-1-30                                      *
*   修改   : 2007-2-6                                       *
*   版本   : 1.0                                            *
*	使用方法见本文件底说明                                  *
*                                                           *
************************************************************/



/******************************************************************************/
// 如果外部已定义了这几个函数，则使用外部定义的
if(typeof GetObj != 'function')
{
	GetObj = function(obj)
	{
		var tmp = document.getElementById(obj);
		if(null!=tmp && null!=tmp.id && ''!=tmp.id)
			return tmp;
		tmp = document.getElementsByName(obj);
		if(null!=tmp && tmp.length>0)
			return tmp;
	}
}
//取得控件的绝对位置(x, y), objID为对象ID;
if(typeof GetPosition != 'function')
{
	GetPosition = function(objID)
	{
		function POS()
		{
			this.x = 0;
			this.y = 0;
		}
		var pos = new POS();
		if((e=GetObj(objID))==null)
			return null;
		var t=e.offsetTop;
		var l=e.offsetLeft;
		while(e=e.offsetParent)
		{
			t+=e.offsetTop;
			l+=e.offsetLeft;
		}
		pos.x = l;
		pos.y = t;
		return pos;
	}
}
// 取当月天数，及当月1日是星期几
if(typeof calendar != 'function')
{
	calendar = function(y,m)
	{
	    var solarMonth=new Array(31,28,31,30,31,30,31,31,30,31,30,31);
	    var sDObj = new Date(y,m,1,0,0,0,0);    //当月一日日期
	
	    this.length    = solarDays(y, m);    //公历当月天数
	    this.firstWeek = sDObj.getDay();    //公历当月1日星期几
	
	    // 返回公历 y年某m+1月的天数
	    function solarDays(y,m)
	    {
	        if(m==1)
	        {
	            return(((y%4 == 0) && (y%100 != 0) || (y%400 == 0))? 29: 28);
	        }
	        else
	        {
	            return(solarMonth[m]);
	        }
	    }
	}
}
/******************************************************************************/





/* 类代码 */
function MonthCalendar(idSpan)
{
    var THIS = MonthCalendar;
    // 日期控件颜色
    THIS.colorDateOver = '#F3EF98';
    THIS.colorDateSel = '#F3EF98';
    // 当前记录的日期
    THIS.thisYear = '';
    THIS.thisMonth = '';
    THIS.thisDate = '';

    THIS.objOwner = '';
    THIS.idSpan = idSpan?idSpan:'MonthCalendar';
    THIS.error = '';
    // 日期控件上的显示控件句柄
    THIS.objCalendarDiv = '';
    THIS.objCalendarYearmon = '';
    THIS.objCalendarDay = '';
    THIS.objCalendarToday = '';
    THIS.objPrevMonth = '';
    THIS.objNextMonth = '';
    
    // 日期控件的大小
    THIS.posCalendarWidth = '';
    THIS.posCalendarHeight = '';

    // 对外接口
    THIS.Set = Set;
    THIS.PrevMonth = PrevMonth;
    THIS.NextMonth = NextMonth;

    /* 外部调用接口函数 */
    /***********************************************************************/
    // 先从控件上取值并显示
    function Set(obj)
    {
        THIS.objOwner = obj;

        var re = new RegExp("([0-9]+)-([0-9]+)-([0-9]+)","g");
        var arr = re.exec(obj.value);
        if(null != arr)
        {
            THIS.thisYear = RegExp.$1;
            THIS.thisMonth = RegExp.$2;
            THIS.thisDate = RegExp.$3;
        }
        else
        {
            // 今天日期
            var date = new Date();
            THIS.thisYear = date.getFullYear();
            THIS.thisMonth = date.getMonth() + 1;
            THIS.thisDate = date.getDate();
        }
        // 填充值后显示
        FillData();
        Show();
    }
    
    /* 内部函数 */
    /***********************************************************************/
    function Create()
    {
        // 使用this.id是为防止出现相同ID（多次创建此控件就会有此情况）
        if(null == this.id)
        {
            this.id = 0;
        }
        try
        {
            this.id++;
            var idCalendarDiv = "calendar" + this.id + "_div_id";
            var idCalendarPrev = "calendar" + this.id + "_prev_id";
            var idCalendarYearmon = "calendar" + this.id + "_yearmon_id";
            var idCalendarNext = "calendar" + this.id + "_next_id";
            var idCalendarDay = "calendar" + this.id + "_day_id";
            var idCalendarToday = "calendar" + this.id + "_today_id";
            var idCalendarPosRB = "calendar" + this.id + "_PosRB_id";
            var idCalendarBGOver = "calendar" + this.id + "_BGOver_id";
            var html = '';
            html += '<div id="' + idCalendarDiv + '" style="position:absolute;';
            html += '   left:-1000px;top:-10000px;border:1px solid;background-color:#FA8072;"';
            html += '	 title="上、下箭头键选年，左、右箭头选月。">';
            // 在日期的div下垫上一个iframe，因为在IE中，div不能盖住select。
            html += '<iframe id="' + idCalendarBGOver + '" frameborder="0" ';
            html += '   style="position:absolute;top:0px;width:0px;height:0px;top:0px;left:0px;z-index:6;"></iframe>';
            html += '<div style="position:absolute;z-index:7;background-color:#FA8072;"">';
            html += '<table width="210" border="0">';
            html += '  <tr align="center">';
            html += '    <td align="left">';
            html += '		<input type="button" id="' + idCalendarPrev + '" style="width:28px;height:23px;" value="<" >';
            html += '	 </td>';
            html += '    <td>';
            html += '		<span id="' + idCalendarYearmon + '" size="8" readOnly></span>';
            html += '	 </td>';
            html += '    <td align="right">';
            html += '		<input type="button" id="' + idCalendarNext + '" style="width:28px;height:23px;" value=">" >';
            html += '	 </td>';
            html += '  </tr>';
            html += '</table>';
            html += '<table id="' + idCalendarDay + '" width="210" border="0">';
            html += '  <tr>';
            html += '    <td>日</td>';
            html += '    <td>一</td>';
            html += '    <td>二</td>';
            html += '    <td>三</td>';
            html += '    <td>四</td>';
            html += '    <td>五</td>';
            html += '    <td>六</td>';
            html += '  </tr>';
            for(var i=0 ; i<6 ; i++)
            {
                html += '<tr>';
                for(var j=0 ; j<7 ; j++)
                {
                    html += '<td><span></span></td>';
                }
                html += '</tr>';
            }
            html += '</table>';
            html += '今天是：<span id="' + idCalendarToday + '"></span>';
            // 这个span用于取得div的右下角坐标（用于间接取div长宽）
            html += '<span id="' + idCalendarPosRB + '" style="position:absolute;right:0px;width:0px;height:0px;"><span>';
            html += '</div></div>';
            GetObj(THIS.idSpan).innerHTML = html;

            // 取日期控件长宽（右下角坐标减去左上角坐标）
            var posRB = GetPosition(idCalendarPosRB);
            var posLT = GetPosition(idCalendarDiv);
            THIS.posCalendarWidth = posRB.x - posLT.x;
            THIS.posCalendarHeight = posRB.y - posLT.y + 15;
            // 修改iframe长宽和日期控件一样
            var over = GetObj(idCalendarBGOver);
            over.style.width = THIS.posCalendarWidth;
            over.style.height = THIS.posCalendarHeight;
            
            // 取句柄
            THIS.objCalendarDiv = GetObj(idCalendarDiv);
            THIS.objPrevMonth = GetObj(idCalendarPrev);
            THIS.objCalendarYearmon = GetObj(idCalendarYearmon);
            THIS.objNextMonth = GetObj(idCalendarNext);
            THIS.objCalendarDay = GetObj(idCalendarDay).getElementsByTagName('span');
            THIS.objCalendarToday = GetObj(idCalendarToday);

            // 按钮 向前
			THIS.objPrevMonth.onclick = function()
			{
				PrevMonth();
			}
			THIS.objPrevMonth.onfocus = function()
			{
				CloseLater(false);
			}
			THIS.objPrevMonth.onblur = function()
			{
				CloseLater(true);
			}
			THIS.objPrevMonth.onkeydown = function(evt)
			{
				THIS.objCalendarYearmon.onkeyup(evt);
			}
			// 按钮 向后
			THIS.objNextMonth.onclick = function()
			{
				NextMonth();
			}
			THIS.objNextMonth.onfocus = function()
			{
				CloseLater(false);
			}
			THIS.objNextMonth.onblur = function()
			{
				CloseLater(true);
			}
			THIS.objNextMonth.onkeydown = function(evt)	//onkeypress
			{
				THIS.objCalendarYearmon.onkeyup(evt);
			}
			// 年、月
			THIS.objCalendarYearmon.onfocus = function()
			{
				CloseLater(false);
			}
			THIS.objCalendarYearmon.onblur = function()
			{
				CloseLater(true);
			}
			THIS.objCalendarYearmon.onkeyup = function(evt)
			{
				var keyCode = GetKeyCode(evt);
				switch(keyCode)
				{
				case 27: // Esc键
					Hide();
				case 37: // 左箭头
					PrevMonth();
					break;
				case 39: // 左箭头
					NextMonth();
					break;
				case 38: // 上箭头
					PrevYear();
					break;
				case 40: // 下箭头
					NextYear();
					break;
				}
			}
			// 日
            for(var i=0 ; i<THIS.objCalendarDay.length ; i++)
            {
	            // 重载函数
	            THIS.objCalendarDay[i].onclick = function()
	            {
		            THIS.objOwner.value = THIS.thisYear + '-' + THIS.thisMonth + '-' + this.innerHTML;
                    Hide();
	            }
	            // 鼠标移动
	            THIS.objCalendarDay[i].onmouseover = function()
	            {
                    this.oldBackground = this.style.background;
                    this.oldCursor = this.style.cursor;
                    this.style.background = THIS.colorDateOver;
                    this.style.cursor = "pointer";		            
	            }
	            // 鼠标移出
	            THIS.objCalendarDay[i].onmouseout = function()
	            {
                    this.style.background = this.oldBackground;
                    this.style.cursor = this.oldCursor;
	            }		
            }
            
            // 设置今天日期的鼠标反应
            THIS.objCalendarToday.onclick = function()
            {
                THIS.objOwner.value = THIS.objCalendarToday.innerHTML;
                Hide();
            }
            THIS.objCalendarToday.onmouseover = function()
            {
                this.oldBackground = this.style.background;
                this.oldCursor = this.style.cursor;
                this.style.background = THIS.colorDateOver;
                this.style.cursor = "pointer";
            }
            THIS.objCalendarToday.onmouseout = function()
            {
                this.style.background = this.oldBackground;
                this.style.cursor = this.oldCursor;
            }

        }
        catch(e)
        {
            THIS.error = '创建日期控件时出错: ' + e.message;
        }
    }// end of Create()...
    
    // close==true: 延时关闭， close==false: 清除延时关闭
    function CloseLater(close)
    {
		if(close)
		{
			this.objTimeout = SetTimeout(Hide, 300);
		}
		else if(!close && this.objTimeout)
		{
			clearTimeout(this.objTimeout);
			this.objTimeout = '';
		}
		
    }
    function Show()
    {
        // 移到需要显示的位置
        var pos = GetPosition(THIS.objOwner.id);
        THIS.objCalendarDiv.style.left = pos.x;
        THIS.objCalendarDiv.style.top = pos.y + 22;
        // 显示
        THIS.objCalendarDiv.style.display = '';
        // 移动焦点，这样以便在失去焦点时关闭本控件。
        THIS.objPrevMonth.focus();
    }
    function Hide()
    {
        THIS.objCalendarDiv.style.display = 'none';
    }
    // 取按键代码，注意，在不同平台有不同。
    function GetKeyCode(evt)
    {
	    return (typeof event=='undefined' ? evt.keyCode : event.keyCode);
    }
    // 超时处理函数（重载超时函数setTimeout）	－－February 06, 2007 
    SetTimeout = function(fRef, mDelay) 
	{
		var _st = window.setTimeout;
		if(typeof fRef == 'function')
		{
			var argu = Array.prototype.slice.call(arguments,2);
			var f = (function(){ fRef.apply(null, argu); });
			return _st(f, mDelay);
		}
		return _st(fRef,mDelay);
	}
	function PrevYear()
	{
		THIS.thisYear--;
		FillData();
	}
	function NextYear()
	{
		THIS.thisYear++;
		FillData();
	}
    function PrevMonth()
    {
        THIS.thisMonth--;
        if(THIS.thisMonth < 1)
        {
            THIS.thisYear--;
            THIS.thisMonth = 12;
        }
        FillData();
    }
    function NextMonth()
    {
        THIS.thisMonth++;
        if(THIS.thisMonth > 12)
        {
            THIS.thisYear++;
            THIS.thisMonth = 1;
        }
        FillData();
    }
    // 把年、月、日数据显示在控件上
    function FillData()
    {
        try
        {
            // 显示年、月
            THIS.objCalendarYearmon.innerHTML = THIS.thisYear + '年' + THIS.thisMonth + '月';
            // 显示日
            /// 先清空
            for(var i=0 ; i<THIS.objCalendarDay.length ; i++)
            {
	            THIS.objCalendarDay[i].style.background = '';
	            THIS.objCalendarDay[i].style.display = 'none';
            }
            /// 再显示
            var cal = new calendar(THIS.thisYear, THIS.thisMonth-1);
            var nDayOfMonth = cal.length;
            var week = cal.firstWeek;
            var objFocus = '';
            for(var date=1 ; date<=nDayOfMonth ; date++)
            {
                var objCalendarDay = THIS.objCalendarDay[week++];
                objCalendarDay.innerHTML = date;
                objCalendarDay.style.display = '';
                // 标记选定的日期
                if(THIS.thisDate == date)
                {
                    //objCalendarDay.style.background = THIS.colorDateSel;
                    objCalendarDay.style.width = "100px";
                    objCalendarDay.style.borderCollapse = 'collapse';
                    objCalendarDay.style.border = '1px solid #000000';
                }
            }
            /// 今天日期
            var date = new Date();
            THIS.objCalendarToday.innerHTML = date.getFullYear() + '-' + (date.getMonth()+1) + '-' + date.getDate();
        }
        catch(e)
        {
            THIS.error = '显示调用出错: ' + e;
            alert(THIS.error);
        }
    }// end of FillData()...
    function Init()
    {
        if(THIS.error != '')
        {
            alert(THIS.error);
        }
    }

    Create();
    Init();
}

// 母体
document.write('<span id="MonthCalendar"></span>');



/**********************************************************************

使用方法：
１．在页面里包含本文件，如：
		<script src="calendar.js"></script>
		
２．在页面里的初如化函数中调用MonthCalendar()，进行日期控件初始化，当
	初化没带参数时，它使用的是默认的ID：MonthCalendar，见上定义的母体。
	当需要多个日期控件时，可定义多个母体，然后带参初化调用：
		MonthCalendar('母体ID')
	注：多母体还未测试过。

３．在需要日期地地方设置一控件，调用这个日期控件，来取日期，如下：
		<input id="calendar" onClick="MonthCalendar.Set(this)">
	这样，在点击这个input时，就会显示日期控，选择日期后，自动把日期
	填到这个控件中。

技巧：
　可合用上、下、左、右箭头来选择前后年、月，Esc键退出。

**********************************************************************/
