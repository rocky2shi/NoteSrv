/**********************************************************************
*                                                                     *
*   ���� : ������                                                   *
*   ��д : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   ���� : 2008-10-03                                                 *
*   �汾 : 1.0                                                        *
*   �޸ġ�ʹ�÷��������ļ���˵��                                      *
*                                                                     *
**********************************************************************/




// ȫ�ֵ����¼�
function SetMouseClick()
{
    // ����Ҫʱ�ڸ�ҳ����OnClick��ִ�е���
    if(typeof OnClick == "undefined")
    {
        OnClick = function(){};
    }
    if (window.attachEvent)
    {
        //֧�� IE
        document.attachEvent("onclick", OnClick);
    }
    else
    {
        //֧�� FF
        document.addEventListener("click", OnClick, false);
    }
}




// ������
function SetMouseEvent()
{
    // ����
    SetMouseClick();
}
