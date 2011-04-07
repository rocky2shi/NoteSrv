// Rocky 2010-05-09 23:57:55
#include "Page.h"
#include "Tag_ImgDisplay.h"
namespace TAG_IMGDISPLAY_SPACE
{






// 标明模块
static const string THIS_MODULE = "ImgDisplay";





Tag_ImgDisplay::Tag_ImgDisplay()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_ImgDisplay::Tag_ImgDisplay(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // 函数轨迹跟综
}

Tag_ImgDisplay::~Tag_ImgDisplay()
{
    FUNCTION_TRACK(); // 函数轨迹跟踪
}




int Tag_ImgDisplay::DoInit()
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return Tag::DoInit();
}

// 子类对象创建器
Tag *Tag_ImgDisplay::DoNew()
{
    FUNCTION_TRACK(); // 函数轨迹跟综
    return new Tag_ImgDisplay;
}








/******************************** 业务代码 ********************************/





// 取标题 [2010-05]
string Tag_ImgDisplay::Get(Page *page)
{
    FUNCTION_TRACK(); // 函数轨迹跟综

    return atoi( page->GetResult("ImageListCount").c_str() ) == 0 ? "none" : "";
}

















// 设置为全局标记；
static Tag_ImgDisplay tmp("ImageList", THIS_MODULE);

}// end of TAG_IMGDISPLAY_SPACE

