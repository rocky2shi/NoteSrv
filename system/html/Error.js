/**********************************************************************
*                                                                     *
*   ���� : ͳһ�����쳣���                                           *
*   ��д : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   ���� : 2008-10-17                                                 *
*   �汾 : 0.1                                                        *
*                                                                     *
**********************************************************************/





window.onerror = function(sMessage, sUrl, sLine)
{
    var err = "Error.js����:\n"
              + "\nURL   : " + sUrl
              + "\nLine  : " + sLine
              + "\nEerror: " + sMessage;
    alert(err);
    return true;
}