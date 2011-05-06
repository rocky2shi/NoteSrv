/**********************************************************************
*                                                                     *
*   功能 : 显示、隐藏指定控件，并移动到窗口中间；                     *
*   编写 : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   日期 : 2008-09-10                                                 *
*   版本 : 1.0                                                        *
*   修改、使用方法见本文件底说明                                      *
*                                                                     *
**********************************************************************/


function ShowToCenter()
{
    // 对外接口
    ShowToCenter.MoveToCenter = MoveToCenter;
    ShowToCenter.Open = Open;
    ShowToCenter.Close = Close;

    /*
     *  正在操作的控件队列，每当Open()时，就记下该控件到此数组；
     */
    var aryId = [];
    var isIE = (navigator.userAgent.toLowerCase().indexOf("msie") != -1);

    /*
     *  显示控件
     */
    function Open(id)
    {
        var obj = GetObj(id);

        // 移到中间
        MoveToCenter(id);

        // 显示
        obj.style.display = 'block';

        /*
         *  记入队列；
         *  注意，这里未测试队列中该元素是否已存在，当对同一控件多次连续调
         *  用Open()时可能出现这种情况。一般使用时，应确保不会多次连续调用
         *  Open()，即调用一次Open()后应接着是调用Close()；
         */
         aryId.push(id);
    }

    /*
     *  隐藏（关闭）控件
     */
    function Close()
    {
        if(aryId.length <= 0)
        {
            return false;
        }
        var id = aryId.pop();
        var obj = GetObj(id);
        obj.style.display = 'none';
    }

    /*
     *  由id取控件
     */
    function GetObj(id)
    {
        var obj = document.getElementById(id);
        if(null == obj)
        {
            alert("ShowToCenter.js: 取控件出错，id=" + id);
            return null;
        }
        return obj;
    }

    /*
     *  控件居中设置
     */
    function MoveToCenter(id)
    {
        var obj = GetObj(id);
        var opacity = '';
        var display = '';

        /*
         *  Step 1:
         *      使控件显示，但设置为透明。因为取高、宽只能对style.displey为
         *      ""（空）的控件有效。注意，在不同浏览器中对属性的不同设置。
         */
        if(isIE)
        {
            // IE中
            opacity = obj.style.filter;
            obj.style.filter = "Alpha(Opacity=0)";
        }
        else
        {
            // FF中
            opacity = obj.style.MozOpacity;
            obj.style.MozOpacity = 0;
        }
        display = obj.style.display;
        obj.style.display = '';

        /*
         *  Step 2:
         *      居中。（注意，必须设置position值才有效。）
         */
        obj.style.position = "absolute";
        // 窗口高、宽（不包括滚动区）
        var nWinWidth = document.body.clientWidth;
        var nWinHeight = document.body.clientHeight;

        // 注意，如果控件是隐藏的，则obj.offsetWidth、offsetHeight为0；
        var nObjWidth = obj.offsetWidth;
        var nObjHeight = obj.offsetHeight;

        // 取控件左上角位置，注意滚动的偏移；
        var nLeft = (nWinWidth - nObjWidth) / 2 + document.body.scrollLeft;
        var nTop = (nWinHeight - nObjHeight) / 2 + document.body.scrollTop;

        // 居中
        obj.style.left = nLeft + 'px';
        obj.style.top = nTop + 'px';

        /*
         *  Step 3:
         *      还原
         */
        if(isIE)
        {
            // IE中
            obj.style.filter = opacity;
            obj.style.display = display;
        }
        else
        {
            obj.style.display = display;
            // FF中
            obj.style.MozOpacity = opacity;
            // Chrome
            obj.style.opacity = opacity;
        }
    }
}

// 直接初始化
ShowToCenter();











/**********************************************************************
使用方法：

        直接调用：

        ShowToCenter.Open(id)   -- 显示
        ShowToCenter.Close()    -- 关闭


History:

 2008-09-25 : 1.1版
 2008-09-10 : 1.0版
**********************************************************************/
