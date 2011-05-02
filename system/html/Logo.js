/**********************************************************************
*                                                                     *
*   ���� : װ��logo��־                                               *
*   ��д : Rocky 2009-11-25 17:29:01                                  *
*   mail : mypyramid@126.com                                          *
*   �޸ġ�ʹ�÷��������ļ���˵��                                      *
*                                                                     *
**********************************************************************/


// id: logo�ؼ���ID
function Logo(id)
{
    var pos = GetPosition(id);
    var scan = null;
    var thick = 3;  // ɨ���ߺ�ȣ����أ�
    var dir = 1;    // �ƶ�����: 1--����, -1--�ҵ���
    
    // Rocky 2009-12-02 12:51:01
    if(null == pos)
    {
        return;
    }
    
    // ���ó�ʼ״̬
    function Init()
    {   
        // ����ɨ����
        var body = document.getElementsByTagName("body")[0];
        scan = document.createElement("div");
        // ����
        body.appendChild(scan);
        // ���ó�ʼֵ
        scan.style.cssText = 'position:absolute;display:none;background-color:#808080;z-index:11;'
        scan.style.left = pos.x + 'px';
        scan.style.top = pos.y + 'px';
        scan.style.width = thick + 'px';
        scan.style.height = pos.h + 'px';
        scan.style.display = '';
    }
    
    // �ƶ�ɨ���ߣ������ƶ���
    function Move()
    {
        var left = parseInt(scan.style.left);
        if(left >= pos.x + pos.w)// ת����
        {
            dir = -1;
        }
        if(left <= pos.x)
        {
            dir = 1;
        }
        left += thick * dir;
        scan.style.left =  left + 'px';
    }
    
    Init();
    SetInterval(Move, 50);
}