/**********************************************************************
*                                                                     *
*   ���� : �ɸǲ�����                                                 *
*   ��д : Rocky                                                      *
*   mail : mypyramid@126.com                                          *
*   ���� : 2008-9-3                                                   *
*   �汾 : 1.0                                                        *
*   �޸ġ�ʹ�÷��������ļ���˵��                                      *
*                                                                     *
**********************************************************************/


/*
 *  ����˵����
 *      zIndex--���ò����ĳ�ʼֵ��Ĭ��Ϊ40��
 *      bgColor--���ò㱳��ɫ��Ĭ��Ϊ#313131;
 *      opacity--���ó�ʼ͸���ȣ�Ĭ��Ϊ50����50%����
 */
function OverLayer(zIndex, opacity, bgColor)
{
    // ����ӿ�
    OverLayer.Open = Open;
    OverLayer.Close = Close;

    // �ڲ�����
    var tDiv = null;
    var tIindexZ = zIndex ? zIndex : 40;
    var tOpacity = opacity ? opacity : 30;
    var tBgColor = bgColor ? bgColor : "#313131";
    var isIE = (navigator.userAgent.toLowerCase().indexOf("msie") != -1);



    /*
     *  ���ܣ���ʾ�ɸǲ�
     *  ������nIndex--�����ֵ�����ڵ���Ҫ��̬����ʱ��һ�������Ϊ�գ�
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
     * �������� [Rocky 2009-08-10 22:26:32]
     */
    function AdjustBgRect()
    {
        // ���ڴ��ڵı仯������̫������Ǹ���������ͨ�õģ���IE��FF�����棩
        var nWidth = document.body.clientWidth > document.body.scrollWidth ? document.body.clientWidth : document.body.scrollWidth;
        var nHeight = document.body.clientHeight > document.body.scrollHeight ? document.body.clientHeight : document.body.scrollHeight;

        tDiv.style.width = nWidth;
        tDiv.style.height = nHeight;
    }


    /*
     *  ���ܣ��رգ����أ��ɸǲ�
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
            alert("�����ɸǲ����");
            return false;
        }

        // �ı䴰�ڴ�Сʱ
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
            // IE��
            tDiv.style.filter = "Alpha(Opacity=" + tOpacity + ")";
        }
        else
        {
            // FF��
            tDiv.style.MozOpacity = tOpacity/100;
            // chrome�� [Rocky 2009-11-30 18:57:53]
            tDiv.style.opacity = tOpacity/100;
        }

        // ע���������˳��
        body[0].appendChild(tDiv);

        return true;
    }

    // ִ�г�ʼ��
    return Init();
}



// ��ʼ�������ɸǲ�
OverLayer();






/**********************************************************************
ʹ�÷�����

        ��ҳ����ĳ�ʼ�������е���OverLayer()�����г�ʼ������
    ����û������ʱ����ʹ�õ���Ĭ��ֵ�������涨�塣���ã�

        OverLayer()       -- ��ʼ��
        OverLayer.Open()  -- ��ʾ��
        OverLayer.Close() -- �رղ�


History:

 2008-09-03 : 0.9��
**********************************************************************/
