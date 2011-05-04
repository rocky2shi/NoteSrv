#ifndef __CONNECT_TCP_H_20100412__
#define __CONNECT_TCP_H_20100412__
#include "Common.h"
#include "Connect.h"
#include "Connect/linux/yCTcp.h"
namespace CONNECT_TCP_SPACE
{




// TCP连接类
class Connect_TCP : public Connect
{
public:
    Connect_TCP();
    virtual ~Connect_TCP();

    virtual int InitAsServer(int port);
    virtual Connect *Accept();
    virtual int Send(const char *buf, unsigned int len);
    virtual int Recv(char *buf, unsigned int len);
    virtual int Close();
    virtual const string GetPeerAddr() const;
    virtual int GetLine(char *buf, unsigned int max);

private:
    yCTcp m_tcp;
    bool m_bClose;
};







}// end of CONNECT_TCP_SPACE
using namespace CONNECT_TCP_SPACE;
#endif // end of __CONNECT_TCP_H_20100412__
