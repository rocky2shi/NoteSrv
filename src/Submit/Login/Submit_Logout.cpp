// Rocky 2010-05-23 12:37:58
#include "Session.h"
#include "Page.h"
#include "Request.h"
#include "Submit_Logout.h"
namespace SUBMIT_LOGOUT_SPACE
{



// ±êÃ÷Ä£¿é
static const string THIS_MODULE = "Logout";





Submit_Logout::Submit_Logout()
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Û
}

Submit_Logout::Submit_Logout(const string &page, const string &element)
                        : Submit(page, element)
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Û
}

Submit_Logout::~Submit_Logout()
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Ù
}




int Submit_Logout::DoInit()
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Û

    return Submit::DoInit();
}

// ×ÓÀà¶ÔÏó´´½¨Æ÷
Submit *Submit_Logout::DoNew()
{
    FUNCTION_TRACK(); // º¯Êý¹ì¼£¸ú×Û
    return new Submit_Logout;
}





/******************************** ÒµÎñ´úÂë ********************************/



int Submit_Logout::Deal(Page *page)
{
    const Request *request = page->GetRequest();
    const string &username = request->GetCurrentUser();

    Session::Get(request)->SetInvalid();
    LOG_INFO("User logout: [%s]", username.c_str());

    return OK;
}













static Submit_Logout tmp("login", THIS_MODULE);

}// end of SUBMIT_LOGOUT_SPACE

