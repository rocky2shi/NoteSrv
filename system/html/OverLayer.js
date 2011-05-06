/**********************************************************************
*                                                                     *
*   功能 : 蒙盖层设置                                                 *
*   编写 : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   日期 : 2008-9-3                                                   *
*   版本 : 1.0                                                        *
*   修改、使用方法见本文件底说明                                      *
*                                                                     *
**********************************************************************/


/*
 *  参数说明：
 *      zIndex--设置层次序的初始值，默认为40；
 *      bgColor--设置层背景色，默认为#313131;
 *      opacity--设置初始透明度，默认为50（即50%）；
 */
function OverLayer(zIndex, opacity, bgColor)
{
    // 对外接口
    OverLayer.Open = Open;
    OverLayer.Close = Close;

    // 内部属性
    var tDiv = null;
    var tIindexZ = zIndex ? zIndex : 40;
    var tOpacity = opacity ? opacity : 30;
    var tBgColor = bgColor ? bgColor : "#313131";
    var isIE = (navigator.userAgent.toLowerCase().indexOf("msie") != -1);



    /*
     *  功能：显示蒙盖层
     *  参数：nIndex--层次序值，用于当需要动态调整时，一般情况下为空；
     */
    function Open(nIndex)
    {
        AdjustBgRect();
        if(nIndex)
        {
            tDiv.style.zIndex = nIndex;
        }
        tDiv.style.display = "";
    };

    /*
     * 调整背景 [Rocky 2009-08-10 22:26:32]
     */
    function AdjustBgRect()
    {
        // 对于窗口的变化，还不太清楚用那个变量是能通用的（在IE和FF等里面）
        var nWidth = document.body.clientWidth > document.body.scrollWidth ? document.body.clientWidth : document.body.scrollWidth;
        var nHeight = document.body.clientHeight > document.body.scrollHeight ? document.body.clientHeight : document.body.scrollHeight;

        tDiv.style.width = nWidth;
        tDiv.style.height = nHeight;
    }


    /*
     *  功能：关闭（隐藏）蒙盖层
     *
     */
    function Close()
    {
        tDiv.style.display = "none";
    };

    function Init()
    {
        if( !Create() )
        {
            alert("设置蒙盖层出错");
            return false;
        }

        // 改变窗口大小时
        InsertFun(window, "resize", AdjustBgRect);

        return true;
    };

    function Create()
    {
        var body = document.getElementsByTagName("body");
        tDiv = document.createElement("div");

        if(body[0] == null)
        {
            return false;
        }

        tDiv.style.position = "absolute";
        tDiv.style.top = "0px";
        tDiv.style.left = "0px";
        tDiv.style.width = "0px";
        tDiv.style.height = "0px";
        tDiv.style.zIndex = tIindexZ;
        tDiv.style.background = tBgColor;
        tDiv.style.display = "none";

        if(isIE)
        {
            // IE中
            tDiv.style.filter = "Alpha(Opacity=" + tOpacity + ")";
        }
        else
        {
            // FF中
            tDiv.style.MozOpacity = tOpacity/100;
            // chrome中 [Rocky 2009-11-30 18:57:53]
            tDiv.style.opacity = tOpacity/100;
        }

        // 注意这两句的顺序
        body[0].appendChild(tDiv);

        return true;
    }

    // 执行初始化
    return Init();
}



// 初始化加载蒙盖层
OverLayer();






/**********************************************************************
使用方法：

        在页面里的初始化函数中调用OverLayer()，进行初始化，当
    初化没带参数时，它使用的是默认值，见上面定义。调用：

        OverLayer()       -- 初始化
        OverLayer.Open()  -- 显示层
        OverLayer.Close() -- 关闭层


History:

 2008-09-03 : 0.9版
**********************************************************************/
