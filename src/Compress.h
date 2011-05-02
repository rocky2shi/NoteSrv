#ifndef __COMPRESS_H_20100510__
#define __COMPRESS_H_20100510__
#include "Common.h"
namespace COMPRESS_SPACE
{




// ѹ������ʹ��zip�ȿ⣩
class Compress
{
public:
    virtual ~Compress();

    // ���ʼ������������ʱ�ĳ�ʼ����
    static int init();

    // ���û��ȡCompress��������������ѡ�ָ��ͬ�㷨��zip��rar�ȣ���
    inline static Compress *instance(Compress *executor=NULL)
    {
        static Compress *obj = (Compress *)(Environment::instance()->ClassInit("Compress", executor));
        return obj;
    }

    /*
     * �ڴ����
     */
    virtual int Do(const void *pSrc, int nSrcLen, void *pDest, int nDestLen);   // ѹ��
    virtual int Undo(const void *pSrc, int nSrcLen, void *pDest, int nDestLen); // ��ѹ��

    /*
     * �ļ�����
     */
    virtual int Do(const FileObj &src, FileObj &dest);    // ѹ��
    virtual int Undo(const FileObj &src, FileObj &dest);  // ��ѹ��

//    /*
//     * �ڴ����� <=> ѹ���ļ�
//     */
//    virtual int Do(const void *pSrc, int nSrcLen, FileObj &dest);       // ѹ��
//    virtual int Undo((const FileObj &src, void *pDest, int nDestLen);   // ��ѹ��
//
//    /*
//     * �ڴ����� <=> ѹ���ļ�
//     */
//    virtual int Do(const Ini &src, FileObj &dest);          // ѹ��
//    virtual int Undo(const FileObj &obj, const Ini &dest);  // ��ѹ��





private:
    Compress();
};







}// end of COMPRESS_SPACE
using namespace COMPRESS_SPACE;
#endif // end of __COMPRESS_H_20100510__
