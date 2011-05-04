#!/bin/bash
# Rocky 2009-10-21 14:55:47







#####################################################
#   设置svn号，以便于查看程序和源代码的对应关系
#####################################################

#    ## 返回svn服务器上存放的代码对应版号
#    svn=$(svn info svn://svn.boss.com/500wan/trunk/cplus/middle/trade.v3.x/src | grep 最后修改的版本 | awk '{print $2}')
#    # 取Help.cpp里的snv号
#    help=$(grep '#define SVN' Help.cpp | tr '"' ' ' | awk '{print $3}')
#    # 修改文件Help.cpp
#    if [[ $help != $svn ]];
#    then
#        sed -i "s/#define SVN [^\$]*/#define SVN \"$svn\"/" Help.cpp
#    fi



#####################################################
#   去除掉源代码中行后的空格
#####################################################
n=1;
find ./ | grep ".c$\|.cpp$\|.h$\|.ice$" 2>/dev/null | while read file;
do
    ret=`sed -n '/[ \t]$/p' $file`
    if [[ "$ret" != "" ]];
    then
        echo $n. $file;
        sed -i 's/[ \t]*$//g' $file
        n=$((n+1))
    fi
done


#####################################################
#   加入修改说明
#####################################################
dos2unix CHANGES
echo '// 注：本文件由编释系统自动修改，其内容来自CHANGES文件。' > Changes.h
echo >> Changes.h
echo 'const char CHANGES[] = "\' >> Changes.h   #'
sed 's/$/\\n\\/' CHANGES | sed 's/\"/\\\"/g' >> Changes.h
echo '";' >> Changes.h
sed -i 's/\\n\\";/\\n\\\n";/' Changes.h



#####################################################
#   使版本号同步（当Version.h里的版号修改时）
#   [Rocky 2010-06-09 14:13:15
#####################################################
touch Tag/Tag_Version.cpp Help.cpp -r Version.h
