/**********************************************************************
*                                                                     *
*   功能 : 统一处理异常情况                                           *
*   编写 : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   日期 : 2008-10-17                                                 *
*   版本 : 0.1                                                        *
*                                                                     *
**********************************************************************/





window.onerror = function(sMessage, sUrl, sLine)
{
    var err = "Error.js报告:\n"
              + "\nURL   : " + sUrl
              + "\nLine  : " + sLine
              + "\nEerror: " + sMessage;
    alert(err);
    return true;
}