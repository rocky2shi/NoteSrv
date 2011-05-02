/**********************************************************************
*                                                                     *
*   ���� : ��ҳ������ʾʱ�䣨������ʱ�䣩                             *
*   ��д : Rocky 2009-06-23                                           *
*   mail : mypyramid@126.com                                          *
*   �޸ġ�ʹ�÷��������ļ���˵��                                      *
*                                                                     *
**********************************************************************/


/*
 *  ���ܣ����ط������ĵ�ǰʱ��
 *  ��д��Rocky 2008-10-30 19:03:02
 */
function ServerDate()
{
    if(typeof ServerDate.time == 'undefined')
    {
        // ��̬��Ա��ͳһά��������null����ʹ֮�÷���ʱ�����ѡ�
        ServerDate.time = document.getElementById('server_time');
        ServerDate.Step = function ()
        {
            ServerDate.time.value++;
        }
    }
    var now = new Date();
    now.setTime(ServerDate.time.value * 1000);
    return now;
}


var obj_clock;


/*****************************************************************************
                                      ũ�����ڼ���
*****************************************************************************/
// �Ǻǣ������������������
function chain_calendar()
{
    var lunarInfo=new Array(
    0x4bd8,0x4ae0,0xa570,0x54d5,0xd260,0xd950,0x5554,0x56af,0x9ad0,0x55d2,
    0x4ae0,0xa5b6,0xa4d0,0xd250,0xd255,0xb54f,0xd6a0,0xada2,0x95b0,0x4977,
    0x497f,0xa4b0,0xb4b5,0x6a50,0x6d40,0xab54,0x2b6f,0x9570,0x52f2,0x4970,
    0x6566,0xd4a0,0xea50,0x6a95,0x5adf,0x2b60,0x86e3,0x92ef,0xc8d7,0xc95f,
    0xd4a0,0xd8a6,0xb55f,0x56a0,0xa5b4,0x25df,0x92d0,0xd2b2,0xa950,0xb557,
    0x6ca0,0xb550,0x5355,0x4daf,0xa5b0,0x4573,0x52bf,0xa9a8,0xe950,0x6aa0,
    0xaea6,0xab50,0x4b60,0xaae4,0xa570,0x5260,0xf263,0xd950,0x5b57,0x56a0,
    0x96d0,0x4dd5,0x4ad0,0xa4d0,0xd4d4,0xd250,0xd558,0xb540,0xb6a0,0x95a6,
    0x95bf,0x49b0,0xa974,0xa4b0,0xb27a,0x6a50,0x6d40,0xaf46,0xab60,0x9570,
    0x4af5,0x4970,0x64b0,0x74a3,0xea50,0x6b58,0x5ac0,0xab60,0x96d5,0x92e0,
    0xc960,0xd954,0xd4a0,0xda50,0x7552,0x56a0,0xabb7,0x25d0,0x92d0,0xcab5,
    0xa950,0xb4a0,0xbaa4,0xad50,0x55d9,0x4ba0,0xa5b0,0x5176,0x52bf,0xa930,
    0x7954,0x6aa0,0xad50,0x5b52,0x4b60,0xa6e6,0xa4e0,0xd260,0xea65,0xd530,
    0x5aa0,0x76a3,0x96d0,0x4afb,0x4ad0,0xa4d0,0xd0b6,0xd25f,0xd520,0xdd45,
    0xb5a0,0x56d0,0x55b2,0x49b0,0xa577,0xa4b0,0xaa50,0xb255,0x6d2f,0xada0,
    0x4b63,0x937f,0x49f8,0x4970,0x64b0,0x68a6,0xea5f,0x6b20,0xa6c4,0xaaef,
    0x92e0,0xd2e3,0xc960,0xd557,0xd4a0,0xda50,0x5d55,0x56a0,0xa6d0,0x55d4,
    0x52d0,0xa9b8,0xa950,0xb4a0,0xb6a6,0xad50,0x55a0,0xaba4,0xa5b0,0x52b0,
    0xb273,0x6930,0x7337,0x6aa0,0xad50,0x4b55,0x4b6f,0xa570,0x54e4,0xd260,
    0xe968,0xd520,0xdaa0,0x6aa6,0x56df,0x4ae0,0xa9d4,0xa4d0,0xd150,0xf252,
    0xd520);



    //====================================== ����ũ�� y���������
    function lYearDays(y) {
     var i, sum = 348;
     for(i=0x8000; i>0x8; i>>=1) sum += (lunarInfo[y-1900] & i)? 1: 0;
     return(sum+leapDays(y));
    }

    //====================================== ����ũ�� y�����µ�����
    function leapDays(y) {
     if(leapMonth(y)) return( (lunarInfo[y-1899]&0xf)==0xf? 30: 29);
     else return(0);
    }

    //====================================== ����ũ�� y�����ĸ��� 1-12 , û�򷵻� 0
    function leapMonth(y) {
     var lm = lunarInfo[y-1900] & 0xf;
     return(lm==0xf?0:lm);
    }

    //====================================== ����ũ�� y��m�µ�������
    function monthDays(y,m) {
     return( (lunarInfo[y-1900] & (0x10000>>m))? 30: 29 );
    }


    //====================================== ���ũ��, �������ڿؼ�, ����ũ�����ڿؼ�
    //                                       �ÿؼ������� .year .month .day .isLeap
    function Lunar(objDate) {

       var i, leap=0, temp=0;
       var offset   = (Date.UTC(objDate.getFullYear(),objDate.getMonth(),objDate.getDate()) - Date.UTC(1900,0,31))/86400000;

       for(i=1900; i<2100 && offset>0; i++) { temp=lYearDays(i); offset-=temp; }

       if(offset<0) { offset+=temp; i--; }

       this.year = i;

       leap = leapMonth(i); //���ĸ���
       this.isLeap = false;

       for(i=1; i<13 && offset>0; i++) {
          //����
          if(leap>0 && i==(leap+1) && this.isLeap==false)
             { --i; this.isLeap = true; temp = leapDays(this.year); }
          else
             { temp = monthDays(this.year, i); }

          //�������
          if(this.isLeap==true && i==(leap+1)) this.isLeap = false;

          offset -= temp;
       }

       if(offset==0 && leap>0 && i==leap+1)
          if(this.isLeap)
             { this.isLeap = false; }
          else
             { this.isLeap = true; --i; }

       if(offset<0){ offset += temp; --i; }

       this.month = i;
       this.day = offset + 1;
    }


    var chYear = new Array('�������', '�������', '������', '������', '����һ��');
    var chMonth = new Array('һ', '��', '��', '��', '��', '��', '��', '��', '��', 'ʮ', 'ʮһ', 'ʮ��');
    var chDate = new Array('��һ', '����', '����', '����', '����', '����', '����', '����', '����', '��ʮ',
            'ʮһ', 'ʮ��', 'ʮ��', 'ʮ��', 'ʮ��', 'ʮ��', 'ʮ��', 'ʮ��', 'ʮ��', '��ʮ',
            'إһ', 'إ��', 'إ��', 'إ��', 'إ��', 'إ��', 'إ��', 'إ��', 'إ��', '��ʮ');
    var date = ServerDate();
    var lDate = new Lunar(date);
    return chYear[lDate.year-2006] + "��" + chMonth[lDate.month-1] + "��" + chDate[lDate.day-1] + "��";
}




function SetClock(id)
{
    var obj_clock = document.getElementById(id); // �ռ���ʾ�ؼ�����
    var tip = new Tips; // ������ʾС����


    function GetDate()
    {
        var now=ServerDate();
        var day = new Array();
           day[0] = "������";
           day[1] = "����һ";
           day[2] = "���ڶ�";
           day[3] = "������";
           day[4] = "������";
           day[5] = "������";
           day[6] = "������";
        var year=now.getFullYear()
        var month=now.getMonth()+1
        var today=now.getDate()
        var sec=now.getTime()
        var week=day[now.getDay()]
        var nowTime=year+'��'+month+'��'+today+'�� '+week
        return nowTime;
    }


    function GetTime()
    {
        var thistime= ServerDate();
        var hours=thistime.getHours()
        var minutes=thistime.getMinutes()
        var seconds=thistime.getSeconds()
        if (eval(hours) <10) {hours="0"+hours}
        if (eval(minutes) < 10) {minutes="0"+minutes}
        if (seconds < 10) {seconds="0"+seconds}
        thistime = hours+":"+minutes+":"+seconds
        return thistime;
    }

    function DisplayClock()
    {
        ServerDate.Step();
        obj_clock.innerHTML = GetTime();
        // LoopRun()���ⲿʹ�õĻص���ʱ����
        try{LoopRun();}catch(e){g_error=e;}
    }

    // ����������ʾ
    function SetDateDisplay()
    {
        obj_clock.onmouseover = function()
        {
            var win = tip.GetWin();
            var pos = GetPos(obj_clock);
            var s = GetDate() + ' (ũ����' + chain_calendar() + ')';
            win.style.right = "3px";
            win.style.top =  pos.y + pos.h - 3 + "px";
            tip.Open(s);
        }
        
        obj_clock.onmouseout = function()
        {
            tip.Close(500);
        }
    }



    if(null == obj_clock)
    {
        return;
    }

    // ��ʼ��
    ServerDate();

    // ����ʱ��
    SetInterval(DisplayClock, 1000);

    // ����������ʾ
    SetDateDisplay();
}








/**********************************************************************
History:

 2009-06-23: 0.1��
 2010-01-13: 0.2�棬�޸�����Ƶ�ʱ����ʱ����ʾ��
**********************************************************************/
