/**********************************************************************
*                                                                     *
*   功能 : 装饰logo标志                                               *
*   编写 : Rocky 2009-11-25 17:29:01                                  *
*   mail : mypyramid@126.com                                          *
*   修改、使用方法见本文件底说明                                      *
*                                                                     *
**********************************************************************/


// id: logo控件的ID
function Logo(id)
{
    var pos = GetPosition(id);
    var scan = null;
    var thick = 3;  // 扫描线厚度（像素）
    var dir = 1;    // 称动方向: 1--左到右, -1--右到左
    
    // Rocky 2009-12-02 12:51:01
    if(null == pos)
    {
        return;
    }
    
    // 设置初始状态
    function Init()
    {   
        // 创建扫描线
        var body = document.getElementsByTagName("body")[0];
        scan = document.createElement("div");
        // 加入
        body.appendChild(scan);
        // 设置初始值
        scan.style.cssText = 'position:absolute;display:none;background-color:#808080;z-index:11;'
        scan.style.left = pos.x + 'px';
        scan.style.top = pos.y + 'px';
        scan.style.width = thick + 'px';
        scan.style.height = pos.h + 'px';
        scan.style.display = '';
    }
    
    // 移动扫描线（往返移动）
    function Move()
    {
        var left = parseInt(scan.style.left);
        if(left >= pos.x + pos.w)// 转方向
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