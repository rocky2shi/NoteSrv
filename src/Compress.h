#ifndef __COMPRESS_H_20100510__
#define __COMPRESS_H_20100510__
#include "Common.h"
namespace COMPRESS_SPACE
{




// 压缩处理（使用zip等库）
class Compress
{
public:
    virtual ~Compress();

    // 类初始化（程序启动时的初始化）
    static int init();

    // 设置或获取Compress单例，根据配制选项，指向不同算法（zip、rar等）；
    inline static Compress *instance(Compress *executor=NULL)
    {
        static Compress *obj = (Compress *)(Environment::instance()->ClassInit("Compress", executor));
        return obj;
    }

    /*
     * 内存操作
     */
    virtual int Do(const void *pSrc, int nSrcLen, void *pDest, int nDestLen);   // 压缩
    virtual int Undo(const void *pSrc, int nSrcLen, void *pDest, int nDestLen); // 解压缩

    /*
     * 文件操作
     */
    virtual int Do(const FileObj &src, FileObj &dest);    // 压缩
    virtual int Undo(const FileObj &src, FileObj &dest);  // 解压缩

//    /*
//     * 内存数据 <=> 压缩文件
//     */
//    virtual int Do(const void *pSrc, int nSrcLen, FileObj &dest);       // 压缩
//    virtual int Undo((const FileObj &src, void *pDest, int nDestLen);   // 解压缩
//
//    /*
//     * 内存数据 <=> 压缩文件
//     */
//    virtual int Do(const Ini &src, FileObj &dest);          // 压缩
//    virtual int Undo(const FileObj &obj, const Ini &dest);  // 解压缩





private:
    Compress();
};







}// end of COMPRESS_SPACE
using namespace COMPRESS_SPACE;
#endif // end of __COMPRESS_H_20100510__
