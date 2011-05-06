/************************************************************
*															*
*	作者: rocky 											*
*	QQ	: 15586350											*
*	功能: 加载指示											*
*															*
************************************************************/

var str = '';
str += '<style>';
str += '#doing span{';
str += '	font-size:60px;';
str += '	color:#F3E64C;';
str += '}';
str += '</style>';
str += '<div id="doing" style="display:none;position:absolute; left:53px; top:76px;">';
str += '<span id="loading0" style="position:absolute;left:91px;top:43px;">.</span>';
str += '<span id="loading1" style="position:absolute;left:125px;top:56px;">.</span>';
str += '<span id="loading2" style="position:absolute;left:138px;top:87px;">.</span>';
str += '<span id="loading3" style="position:absolute;left:126px;top:120px;">.</span>';
str += '<span id="loading4" style="position:absolute;left:93px;top:134px;">.</span>';
str += '<span id="loading5" style="position:absolute;left:55px;top:119px;">.</span>';
str += '<span id="loading6" style="position:absolute;left:37px;top:86px;">.</span>';
str += '<span id="loading7" style="position:absolute;left:55px;top:56px;">.</span>';
str += '</div>';
document.write(str);
		
function Doing()
{
	Doing.clear = '';
	Doing.load_ary = new Array();
	
	Doing.start = Start;
	Doing.stop	= Stop;
	Doing.run	= Run;
	function Init()
	{
		for(var i=0 ; i<8 ; i++)
		{
			Doing.load_ary[i] = document.getElementById('loading' + i);
		}
	}
	
	function Run()
	{
		if(null == this.i)
		{
			this.i = 0;
			this.color = 'red';
		}
		if(this.i > 7)
		{
			this.i = 0;
			if(this.color == '#F3E64C')
				this.color = '#FA8072';
			else
				this.color = '#F3E64C';
		}
		Doing.load_ary[this.i++].style.color = this.color;
	}
	
	function Start()
	{
		Doing.clear = setInterval('Doing.run()', 200);
	}
	function Stop()
	{
		if('' == Doing.clear)
			return;
		clearInterval(Doing.clear);
		Doing.clear = '';
	}
	
	Init();
}

// Doing();
// Doing.start();
