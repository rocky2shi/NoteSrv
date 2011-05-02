// Rocky 2009


/*
 *  ���尴�����ִ�
 *
 *      ������       ����         ����
 *     -------------------------------------------------
 *      0��9         48��57       KEY_0��KEY_9
 *      A��Z         65��90       KEY_A��KEY_Z
 *      F1��F12      112��123     KEY_F1��KEY_F12
 *      ~            192          KEY_
 *      +            107(187)     KEY_ADD
 *      -            109(189)     KEY_SUB
 *      <            188          KEY_
 *      >            190          KEY_
 *      ?            191          KEY_
 *      {            219          KEY_
 *      }            221          KEY_
 *      |            220          KEY_
 *      ���ͷ       37           KEY_LEFT
 *      �Ҽ�ͷ       39           KEY_RIGHT
 *      �ϼ�ͷ       38           KEY_UP
 *      �¼�ͷ       40           KEY_DOWN
 *      �س�         13           KEY_ENTER
 *      �˸�         8            KEY_BACKSPACE
 *      �ո�         32           KEY_SPACE
 *      Esc          27           KEY_ESC
 *      Tab          9            KEY_TAB
 *      ��Сд       20           KEY_CAPSLOCK
 *      Delete       46           KEY_DELETE
 *      PageUp       33           KEY_PAGEUP
 *      PageDown     34           KEY_PAGEDOWN
 *      Home         36           KEY_HOME
 *      End          35           KEY_END
 *      Ctrl         17           KEY_CTRL
 *      Alt          18           KEY_ALT
 *      Shift        16           KEY_SHIFT
 *     -------------------------------------------------
 */


// �������롢��Ӧ����
var KeyStr = [
        [48, "0"], [49, "1"], [50, "2"], [51, "3"], [52, "4"], [53, "5"], [54, "6"], [55, "7"], [56, "8"], [57, "9"],
        [65, "A"], [66, "B"], [67, "C"], [68, "D"], [69, "E"], [70, "F"], [71, "G"], [72, "H"], [73, "I"], [74, "J"], [75, "K"], [76, "L"], [77, "M"], [78, "N"], [79, "O"], [80, "P"], [81, "Q"], [82, "R"], [83, "S"], [84, "T"], [85, "U"], [86, "V"], [87, "W"], [88, "X"], [89, "Y"], [90, "Z"],
        [112, "F1"], [113, "F2"], [114, "F3"], [115, "F4"], [116, "F5"], [117, "F6"], [118, "F7"], [119, "F8"], [120, "F9"], [121, "F10"], [122, "F11"], [123, "F12"],
        [37, "LEFT"],
        [39, "RIGHT"],
        [38, "UP"],
        [40, "DOWN"],
        [13, "ENTER"],
        [8 , "BACKSPACE"],
        [32, "SPACE"],
        [27, "ESC"],
        [9 , "TAB"],
        [20, "CAPSLOCK"],
        [46, "DELETE"],
        [33, "UP"],
        [34, "DOWN"],
        [36, "HOME"],
        [35, "END"],
        [107,"ADD"],
        [109,"SUB"]
    ];

// ������ж���İ�����Ӧ���������ִ���
var KEY = [];

// ��̬����������������� KEY_A = 48; KEY[48] = 'KEY_A';
var keys = '';
var p='';
for(var i=0; (p = KeyStr[i]); i++)
{
    // KEY_A = 48; KEY[48] = 'KEY_A';
    keys += 'KEY_' + p[1] + ' = ' + p[0] + '; KEY[' + p[0] + '] = "' + p[1] + '"; \n'
}
eval(keys);




function ShortKey()
{
    var THIS = ShortKey;
    var mapFun = {};
    var mapArgs = {};
    var bLocked = false;    // true:��ʾ��ݼ��ѱ����ţ�Esc���⣩��false��ʾû����
    var bLockAll= false;    // �������м�������Esc�� [2010-05-29]

    // ��ʼ��
    Init();

    /*
     *  ���ܣ�����ӿڣ�ע�ᰴť������
     *  ������$1--�����������룩
     *        $2~n--����$1�Ĳ�������ѡ��
     */
    THIS.Register = function()
    {
        var key = arguments[0];
        var keys = MakeKeyStr(key['ctrl'], key['alt'], key['shift'], key['key']);

        if(mapFun[keys])
        {
            alert('�����ѱ�ռ��: ' + keys);
            return;
        }

        // ȡ��������������
        var fun = arguments[1];
        var args = [].slice.call(arguments, 2);

        mapFun[keys] = fun;
        mapArgs[keys] = args;
        return true;
    }

    /*
     *  ��ݼ��Ƿ�����
     */
    THIS.IsLock = function()
    {
        return bLocked;
    }

    /*
     *  ������ݼ�
     */
    THIS.Lock = function()
    {
        bLocked = true;
    }
    // Ҳ����Esc��
    THIS.LockAll = function()
    {
        bLocked = true;
        bLockAll = true;
    }

    /*
     *  ������ݼ�
     */
    THIS.Unlock = function()
    {
        bLocked = false;
        bLockAll = false;
    }

    /*
     *  ��ȡ������Ӧ����ϴ�
     */
    function MakeKeyStr(ctrl, alt, shift, code)
    {
        var keys = '';

        // CTRL + ALT + SHIFT + ����
        if(ctrl && alt && shift)
        {
            keys = 'CTRL_' + 'ALT_' + 'SHIFT_';
        }
        // CTRL + ALT + ����
        else if(ctrl && alt)
        {
            keys = 'CTRL_' + 'ALT_';
        }
        // CTRL + SHIFT + ����
        else if(ctrl && shift)
        {
            keys = 'CTRL_' + 'SHIFT_';
        }
        // ALT + SHIFT + ����
        else if(alt && shift)
        {
            keys = 'ALT_' + 'SHIFT_';
        }
        // CTRL + ����
        else if(ctrl)
        {
            keys = 'CTRL_';
        }
        // ALT + ����
        else if(alt)
        {
            keys = 'ALT_';
        }
        // SHIFT + ����
        else if(shift)
        {
            keys = 'SHIFT_';
        }


        if(null == KEY[code])
        {
            return null;
        }
        keys = "KEY_" + keys + KEY[code];  // �ټ���ǰ׺
        return keys;
    }

    /*
     *  ִ�а����Ķ���
     */
    function OnKeyDown(event)
    {
        event = event || window.event;

        var keyCode = event.keyCode;

        // ͳһ���̴���
        if(Version.ie() || Version.chrome()) // [Rocky 2010-05-26 16:43:39]
        {
            switch(keyCode)
            {
            case 187:   // +
                keyCode = 107;
                break;
            case 189:   // -
                keyCode = 109;
                break;
            }
        }
        else
        {
            switch(keyCode)
            {
            case 61:   // +, Firefox/2.0.0.7
                keyCode = 107;
                break;
            }
        }

        // ��ֹ��ݼ�
        if( bLockAll || (27 != keyCode && bLocked) )
        {
            window.status = '����������';
            return;
        }

        try
        {
            var keys = MakeKeyStr(event.ctrlKey, event.altKey, event.shiftKey, keyCode);
            var fun = mapFun[keys];
            var args = mapArgs[keys];

            if(typeof fun != 'function')
            {
                window.status = keys;
                return;
            }
            fun.apply(null, args);
        }
        catch(e)
        {
            // alert("Keyboard.js: " + e);
            window.status = "Keyboard.js: " + e;
        }
    }

    /*
     *  ����ϵͳȫ�ְ�����Ӧ
     */
    function Init()
    {
        if( Version.ie() )
        {
            //֧�� IE
            document.attachEvent("onkeydown", OnKeyDown);
        }
        else
        {
            //֧�� FF
            document.addEventListener("keydown", OnKeyDown, false);
        }
    }
}









/*
 *  ʹ��ʾ��
 *
 *  function AltN()
 *  {
 *      alert(arguments[1]);
 *      ShortKey.Lock();
 *  }
 *
 *  function Esc()
 *  {
 *      alert(arguments.length);
 *      ShortKey.Unlock();
 *  }
 *
 *  ShortKey.Register({alt:true, key:KEY_N}, AltN, 1, "AltN()", 333);
 *  ShortKey.Register({key:KEY_ESC}, Esc);
 *
 */
