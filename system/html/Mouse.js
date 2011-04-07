/**********************************************************************
*                                                                     *
*   功能 : 鼠标控制                                                   *
*   编写 : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   日期 : 2008-10-03                                                 *
*   版本 : 1.0                                                        *
*   修改、使用方法见本文件底说明                                      *
*                                                                     *
**********************************************************************/




// 全局单击事件
function SetMouseClick()
{
    // 当需要时在各页重载OnClick来执行单击
    if(typeof OnClick == "undefined")
    {
        OnClick = function(){};
    }
    if (window.attachEvent)
    {
        //支持 IE
        document.attachEvent("onclick", OnClick);
    }
    else
    {
        //支持 FF
        document.addEventListener("click", OnClick, false);
    }
}




// 鼠标控制
function SetMouseEvent()
{
    // 单击
    SetMouseClick();
}
