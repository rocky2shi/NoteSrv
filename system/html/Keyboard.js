// Rocky 2009


/*
 *  定义按键名字串
 *
 *      按键名       代码         定义
 *     -------------------------------------------------
 *      0～9         48～57       KEY_0～KEY_9
 *      A～Z         65～90       KEY_A～KEY_Z
 *      F1～F12      112～123     KEY_F1～KEY_F12
 *      ~            192          KEY_
 *      +            107(187)     KEY_ADD
 *      -            109(189)     KEY_SUB
 *      <            188          KEY_
 *      >            190          KEY_
 *      ?            191          KEY_
 *      {            219          KEY_
 *      }            221          KEY_
 *      |            220          KEY_
 *      左箭头       37           KEY_LEFT
 *      右箭头       39           KEY_RIGHT
 *      上箭头       38           KEY_UP
 *      下箭头       40           KEY_DOWN
 *      回车         13           KEY_ENTER
 *      退格         8            KEY_BACKSPACE
 *      空格         32           KEY_SPACE
 *      Esc          27           KEY_ESC
 *      Tab          9            KEY_TAB
 *      大小写       20           KEY_CAPSLOCK
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


// 按键代码、对应串；
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

// 存放所有定义的按键对应的命名（字串）
var KEY = [];

// 动态构构造两组变量：如 KEY_A = 48; KEY[48] = 'KEY_A';
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
    var bLocked = false;    // true:表示快捷键已被锁信（Esc除外），false表示没锁；
    var bLockAll= false;    // 锁定所有键，包括Esc键 [2010-05-29]

    // 初始化
    Init();

    /*
     *  功能：对外接口，注册按钮操作；
     *  参数：$1--函数名（必须）
     *        $2~n--函数$1的参数（可选）
     */
    THIS.Register = function()
    {
        var key = arguments[0];
        var keys = MakeKeyStr(key['ctrl'], key['alt'], key['shift'], key['key']);

        if(mapFun[keys])
        {
            alert('按键已被占用: ' + keys);
            return;
        }

        // 取出函数名、参数
        var fun = arguments[1];
        var args = [].slice.call(arguments, 2);

        mapFun[keys] = fun;
        mapArgs[keys] = args;
        return true;
    }

    /*
     *  快捷键是否被锁定
     */
    THIS.IsLock = function()
    {
        return bLocked;
    }

    /*
     *  锁定快捷键
     */
    THIS.Lock = function()
    {
        bLocked = true;
    }
    // 也锁定Esc键
    THIS.LockAll = function()
    {
        bLocked = true;
        bLockAll = true;
    }

    /*
     *  解锁快捷键
     */
    THIS.Unlock = function()
    {
        bLocked = false;
        bLockAll = false;
    }

    /*
     *  获取按键对应的组合串
     */
    function MakeKeyStr(ctrl, alt, shift, code)
    {
        var keys = '';

        // CTRL + ALT + SHIFT + 按键
        if(ctrl && alt && shift)
        {
            keys = 'CTRL_' + 'ALT_' + 'SHIFT_';
        }
        // CTRL + ALT + 按键
        else if(ctrl && alt)
        {
            keys = 'CTRL_' + 'ALT_';
        }
        // CTRL + SHIFT + 按键
        else if(ctrl && shift)
        {
            keys = 'CTRL_' + 'SHIFT_';
        }
        // ALT + SHIFT + 按键
        else if(alt && shift)
        {
            keys = 'ALT_' + 'SHIFT_';
        }
        // CTRL + 按键
        else if(ctrl)
        {
            keys = 'CTRL_';
        }
        // ALT + 按键
        else if(alt)
        {
            keys = 'ALT_';
        }
        // SHIFT + 按键
        else if(shift)
        {
            keys = 'SHIFT_';
        }


        if(null == KEY[code])
        {
            return null;
        }
        keys = "KEY_" + keys + KEY[code];  // 再加上前缀
        return keys;
    }

    /*
     *  执行按键的动作
     */
    function OnKeyDown(event)
    {
        event = event || window.event;

        var keyCode = event.keyCode;

        // 统一键盘代码
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

        // 禁止快捷键
        if( bLockAll || (27 != keyCode && bLocked) )
        {
            window.status = '按键被锁定';
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
     *  设置系统全局按键反应
     */
    function Init()
    {
        if( Version.ie() )
        {
            //支持 IE
            document.attachEvent("onkeydown", OnKeyDown);
        }
        else
        {
            //支持 FF
            document.addEventListener("keydown", OnKeyDown, false);
        }
    }
}









/*
 *  使用示例
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
