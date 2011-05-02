/**********************************************************************
*                                                                     *
*   ���� : ��ʾ������ָ���ؼ������ƶ��������м䣻                     *
*   ��д : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   ���� : 2008-09-10                                                 *
*   �汾 : 1.0                                                        *
*   �޸ġ�ʹ�÷��������ļ���˵��                                      *
*                                                                     *
**********************************************************************/


function ShowToCenter()
{
    // ����ӿ�
    ShowToCenter.MoveToCenter = MoveToCenter;
    ShowToCenter.Open = Open;
    ShowToCenter.Close = Close;

    /*
     *  ���ڲ����Ŀؼ����У�ÿ��Open()ʱ���ͼ��¸ÿؼ��������飻
     */
    var aryId = [];
    var isIE = (navigator.userAgent.toLowerCase().indexOf("msie") != -1);

    /*
     *  ��ʾ�ؼ�
     */
    function Open(id)
    {
        var obj = GetObj(id);

        // �Ƶ��м�
        MoveToCenter(id);

        // ��ʾ
        obj.style.display = 'block';

        /*
         *  ������У�
         *  ע�⣬����δ���Զ����и�Ԫ���Ƿ��Ѵ��ڣ�����ͬһ�ؼ����������
         *  ��Open()ʱ���ܳ������������һ��ʹ��ʱ��Ӧȷ����������������
         *  Open()��������һ��Open()��Ӧ�����ǵ���Close()��
         */
         aryId.push(id);
    }

    /*
     *  ���أ��رգ��ؼ�
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
     *  ��idȡ�ؼ�
     */
    function GetObj(id)
    {
        var obj = document.getElementById(id);
        if(null == obj)
        {
            alert("ShowToCenter.js: ȡ�ؼ�����id=" + id);
            return null;
        }
        return obj;
    }

    /*
     *  �ؼ���������
     */
    function MoveToCenter(id)
    {
        var obj = GetObj(id);
        var opacity = '';
        var display = '';

        /*
         *  Step 1:
         *      ʹ�ؼ���ʾ��������Ϊ͸������Ϊȡ�ߡ���ֻ�ܶ�style.displeyΪ
         *      ""���գ��Ŀؼ���Ч��ע�⣬�ڲ�ͬ������ж����ԵĲ�ͬ���á�
         */
        if(isIE)
        {
            // IE��
            opacity = obj.style.filter;
            obj.style.filter = "Alpha(Opacity=0)";
        }
        else
        {
            // FF��
            opacity = obj.style.MozOpacity;
            obj.style.MozOpacity = 0;
        }
        display = obj.style.display;
        obj.style.display = '';

        /*
         *  Step 2:
         *      ���С���ע�⣬��������positionֵ����Ч����
         */
        obj.style.position = "absolute";
        // ���ڸߡ�����������������
        var nWinWidth = document.body.clientWidth;
        var nWinHeight = document.body.clientHeight;

        // ע�⣬����ؼ������صģ���obj.offsetWidth��offsetHeightΪ0��
        var nObjWidth = obj.offsetWidth;
        var nObjHeight = obj.offsetHeight;

        // ȡ�ؼ����Ͻ�λ�ã�ע�������ƫ�ƣ�
        var nLeft = (nWinWidth - nObjWidth) / 2 + document.body.scrollLeft;
        var nTop = (nWinHeight - nObjHeight) / 2 + document.body.scrollTop;

        // ����
        obj.style.left = nLeft + 'px';
        obj.style.top = nTop + 'px';

        /*
         *  Step 3:
         *      ��ԭ
         */
        if(isIE)
        {
            // IE��
            obj.style.filter = opacity;
            obj.style.display = display;
        }
        else
        {
            obj.style.display = display;
            // FF��
            obj.style.MozOpacity = opacity;
            // Chrome
            obj.style.opacity = opacity;
        }
    }
}

// ֱ�ӳ�ʼ��
ShowToCenter();











/**********************************************************************
ʹ�÷�����

        ֱ�ӵ��ã�

        ShowToCenter.Open(id)   -- ��ʾ
        ShowToCenter.Close()    -- �ر�


History:

 2008-09-25 : 1.1��
 2008-09-10 : 1.0��
**********************************************************************/
