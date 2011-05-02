/************************************************************
*                                                           *
*   ����   : �������ÿؼ�                                   *
*   ����   : shizw                                          *
*   E-mail : mypyramid@126.com                              *
*   ����   : 2007-1-30                                      *
*   �޸�   : 2007-2-6                                       *
*   �汾   : 1.0                                            *
*	ʹ�÷��������ļ���˵��                                  *
*                                                           *
************************************************************/



/******************************************************************************/
// ����ⲿ�Ѷ������⼸����������ʹ���ⲿ�����
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
//ȡ�ÿؼ��ľ���λ��(x, y), objIDΪ����ID;
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
// ȡ����������������1�������ڼ�
if(typeof calendar != 'function')
{
	calendar = function(y,m)
	{
	    var solarMonth=new Array(31,28,31,30,31,30,31,31,30,31,30,31);
	    var sDObj = new Date(y,m,1,0,0,0,0);    //����һ������
	
	    this.length    = solarDays(y, m);    //������������
	    this.firstWeek = sDObj.getDay();    //��������1�����ڼ�
	
	    // ���ع��� y��ĳm+1�µ�����
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





/* ����� */
function MonthCalendar(idSpan)
{
    var THIS = MonthCalendar;
    // ���ڿؼ���ɫ
    THIS.colorDateOver = '#F3EF98';
    THIS.colorDateSel = '#F3EF98';
    // ��ǰ��¼������
    THIS.thisYear = '';
    THIS.thisMonth = '';
    THIS.thisDate = '';

    THIS.objOwner = '';
    THIS.idSpan = idSpan?idSpan:'MonthCalendar';
    THIS.error = '';
    // ���ڿؼ��ϵ���ʾ�ؼ����
    THIS.objCalendarDiv = '';
    THIS.objCalendarYearmon = '';
    THIS.objCalendarDay = '';
    THIS.objCalendarToday = '';
    THIS.objPrevMonth = '';
    THIS.objNextMonth = '';
    
    // ���ڿؼ��Ĵ�С
    THIS.posCalendarWidth = '';
    THIS.posCalendarHeight = '';

    // ����ӿ�
    THIS.Set = Set;
    THIS.PrevMonth = PrevMonth;
    THIS.NextMonth = NextMonth;

    /* �ⲿ���ýӿں��� */
    /***********************************************************************/
    // �ȴӿؼ���ȡֵ����ʾ
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
            // ��������
            var date = new Date();
            THIS.thisYear = date.getFullYear();
            THIS.thisMonth = date.getMonth() + 1;
            THIS.thisDate = date.getDate();
        }
        // ���ֵ����ʾ
        FillData();
        Show();
    }
    
    /* �ڲ����� */
    /***********************************************************************/
    function Create()
    {
        // ʹ��this.id��Ϊ��ֹ������ͬID����δ����˿ؼ��ͻ��д������
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
            html += '	 title="�ϡ��¼�ͷ��ѡ�꣬���Ҽ�ͷѡ�¡�">';
            // �����ڵ�div�µ���һ��iframe����Ϊ��IE�У�div���ܸ�סselect��
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
            html += '    <td>��</td>';
            html += '    <td>һ</td>';
            html += '    <td>��</td>';
            html += '    <td>��</td>';
            html += '    <td>��</td>';
            html += '    <td>��</td>';
            html += '    <td>��</td>';
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
            html += '�����ǣ�<span id="' + idCalendarToday + '"></span>';
            // ���span����ȡ��div�����½����꣨���ڼ��ȡdiv����
            html += '<span id="' + idCalendarPosRB + '" style="position:absolute;right:0px;width:0px;height:0px;"><span>';
            html += '</div></div>';
            GetObj(THIS.idSpan).innerHTML = html;

            // ȡ���ڿؼ��������½������ȥ���Ͻ����꣩
            var posRB = GetPosition(idCalendarPosRB);
            var posLT = GetPosition(idCalendarDiv);
            THIS.posCalendarWidth = posRB.x - posLT.x;
            THIS.posCalendarHeight = posRB.y - posLT.y + 15;
            // �޸�iframe��������ڿؼ�һ��
            var over = GetObj(idCalendarBGOver);
            over.style.width = THIS.posCalendarWidth;
            over.style.height = THIS.posCalendarHeight;
            
            // ȡ���
            THIS.objCalendarDiv = GetObj(idCalendarDiv);
            THIS.objPrevMonth = GetObj(idCalendarPrev);
            THIS.objCalendarYearmon = GetObj(idCalendarYearmon);
            THIS.objNextMonth = GetObj(idCalendarNext);
            THIS.objCalendarDay = GetObj(idCalendarDay).getElementsByTagName('span');
            THIS.objCalendarToday = GetObj(idCalendarToday);

            // ��ť ��ǰ
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
			// ��ť ���
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
			// �ꡢ��
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
				case 27: // Esc��
					Hide();
				case 37: // ���ͷ
					PrevMonth();
					break;
				case 39: // ���ͷ
					NextMonth();
					break;
				case 38: // �ϼ�ͷ
					PrevYear();
					break;
				case 40: // �¼�ͷ
					NextYear();
					break;
				}
			}
			// ��
            for(var i=0 ; i<THIS.objCalendarDay.length ; i++)
            {
	            // ���غ���
	            THIS.objCalendarDay[i].onclick = function()
	            {
		            THIS.objOwner.value = THIS.thisYear + '-' + THIS.thisMonth + '-' + this.innerHTML;
                    Hide();
	            }
	            // ����ƶ�
	            THIS.objCalendarDay[i].onmouseover = function()
	            {
                    this.oldBackground = this.style.background;
                    this.oldCursor = this.style.cursor;
                    this.style.background = THIS.colorDateOver;
                    this.style.cursor = "pointer";		            
	            }
	            // ����Ƴ�
	            THIS.objCalendarDay[i].onmouseout = function()
	            {
                    this.style.background = this.oldBackground;
                    this.style.cursor = this.oldCursor;
	            }		
            }
            
            // ���ý������ڵ���귴Ӧ
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
            THIS.error = '�������ڿؼ�ʱ����: ' + e.message;
        }
    }// end of Create()...
    
    // close==true: ��ʱ�رգ� close==false: �����ʱ�ر�
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
        // �Ƶ���Ҫ��ʾ��λ��
        var pos = GetPosition(THIS.objOwner.id);
        THIS.objCalendarDiv.style.left = pos.x;
        THIS.objCalendarDiv.style.top = pos.y + 22;
        // ��ʾ
        THIS.objCalendarDiv.style.display = '';
        // �ƶ����㣬�����Ա���ʧȥ����ʱ�رձ��ؼ���
        THIS.objPrevMonth.focus();
    }
    function Hide()
    {
        THIS.objCalendarDiv.style.display = 'none';
    }
    // ȡ�������룬ע�⣬�ڲ�ͬƽ̨�в�ͬ��
    function GetKeyCode(evt)
    {
	    return (typeof event=='undefined' ? evt.keyCode : event.keyCode);
    }
    // ��ʱ�����������س�ʱ����setTimeout��	����February 06, 2007 
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
    // ���ꡢ�¡���������ʾ�ڿؼ���
    function FillData()
    {
        try
        {
            // ��ʾ�ꡢ��
            THIS.objCalendarYearmon.innerHTML = THIS.thisYear + '��' + THIS.thisMonth + '��';
            // ��ʾ��
            /// �����
            for(var i=0 ; i<THIS.objCalendarDay.length ; i++)
            {
	            THIS.objCalendarDay[i].style.background = '';
	            THIS.objCalendarDay[i].style.display = 'none';
            }
            /// ����ʾ
            var cal = new calendar(THIS.thisYear, THIS.thisMonth-1);
            var nDayOfMonth = cal.length;
            var week = cal.firstWeek;
            var objFocus = '';
            for(var date=1 ; date<=nDayOfMonth ; date++)
            {
                var objCalendarDay = THIS.objCalendarDay[week++];
                objCalendarDay.innerHTML = date;
                objCalendarDay.style.display = '';
                // ���ѡ��������
                if(THIS.thisDate == date)
                {
                    //objCalendarDay.style.background = THIS.colorDateSel;
                    objCalendarDay.style.width = "100px";
                    objCalendarDay.style.borderCollapse = 'collapse';
                    objCalendarDay.style.border = '1px solid #000000';
                }
            }
            /// ��������
            var date = new Date();
            THIS.objCalendarToday.innerHTML = date.getFullYear() + '-' + (date.getMonth()+1) + '-' + date.getDate();
        }
        catch(e)
        {
            THIS.error = '��ʾ���ó���: ' + e;
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

// ĸ��
document.write('<span id="MonthCalendar"></span>');



/**********************************************************************

ʹ�÷�����
������ҳ����������ļ����磺
		<script src="calendar.js"></script>
		
������ҳ����ĳ��绯�����е���MonthCalendar()���������ڿؼ���ʼ������
	����û������ʱ����ʹ�õ���Ĭ�ϵ�ID��MonthCalendar�����϶����ĸ�塣
	����Ҫ������ڿؼ�ʱ���ɶ�����ĸ�壬Ȼ����γ������ã�
		MonthCalendar('ĸ��ID')
	ע����ĸ�廹δ���Թ���

��������Ҫ���ڵصط�����һ�ؼ�������������ڿؼ�����ȡ���ڣ����£�
		<input id="calendar" onClick="MonthCalendar.Set(this)">
	�������ڵ�����inputʱ���ͻ���ʾ���ڿأ�ѡ�����ں��Զ�������
	�����ؼ��С�

���ɣ�
���ɺ����ϡ��¡����Ҽ�ͷ��ѡ��ǰ���ꡢ�£�Esc���˳���

**********************************************************************/
