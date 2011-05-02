// Rocky 2010-05-09 23:57:55
#include "Page.h"
#include "Tag_ImgDisplay.h"
namespace TAG_IMGDISPLAY_SPACE
{






// ����ģ��
static const string THIS_MODULE = "ImgDisplay";





Tag_ImgDisplay::Tag_ImgDisplay()
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_ImgDisplay::Tag_ImgDisplay(const string &page, const string &tag) : Tag(page, tag)
{
    FUNCTION_TRACK(); // �����켣����
}

Tag_ImgDisplay::~Tag_ImgDisplay()
{
    FUNCTION_TRACK(); // �����켣����
}




int Tag_ImgDisplay::DoInit()
{
    FUNCTION_TRACK(); // �����켣����

    return Tag::DoInit();
}

// ������󴴽���
Tag *Tag_ImgDisplay::DoNew()
{
    FUNCTION_TRACK(); // �����켣����
    return new Tag_ImgDisplay;
}








/******************************** ҵ����� ********************************/





// ȡ���� [2010-05]
string Tag_ImgDisplay::Get(Page *page)
{
    FUNCTION_TRACK(); // �����켣����

    return atoi( page->GetResult("ImageListCount").c_str() ) == 0 ? "none" : "";
}

















// ����Ϊȫ�ֱ�ǣ�
static Tag_ImgDisplay tmp("ImageList", THIS_MODULE);

}// end of TAG_IMGDISPLAY_SPACE

