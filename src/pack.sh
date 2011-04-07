#!/bin/bash
# Rocky 2010-06-03 10:42:56
#
#   打包程序文件
#



# 打包临时目录
PACK_DIR="tmp.pack.$RANDOM"

mkdir $PACK_DIR || exit
cd $PACK_DIR || exit
mkdir cgi data html log template tmp tool


strip ../NoteSrv ../Page/Page_*.so
cp -f -a ../NoteSrv                 .
cp -f -a ../Page/Page_*.so          cgi/
cp -f -a ../../system/template/*    template/
cp -f -a ../../system/html/*        html/
cp -f -a ../../system/data/*        data/
cp -f -a ../../system/tool/*        tool/

PACK_NAME=NoteSrv.$(date "+%Y_%m_%d").tar
tar cvf ../$PACK_NAME *
echo ----------------$PACK_NAME [OK]----------------
cd ..
rm -rf $PACK_DIR
