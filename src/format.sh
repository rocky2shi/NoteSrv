#!/bin/bash
# Rocky 2009-10-21 14:55:47







#####################################################
#   ����svn�ţ��Ա��ڲ鿴�����Դ����Ķ�Ӧ��ϵ
#####################################################

#    ## ����svn�������ϴ�ŵĴ����Ӧ���
#    svn=$(svn info svn://svn.boss.com/500wan/trunk/cplus/middle/trade.v3.x/src | grep ����޸ĵİ汾 | awk '{print $2}')
#    # ȡHelp.cpp���snv��
#    help=$(grep '#define SVN' Help.cpp | tr '"' ' ' | awk '{print $3}')
#    # �޸��ļ�Help.cpp
#    if [[ $help != $svn ]];
#    then
#        sed -i "s/#define SVN [^\$]*/#define SVN \"$svn\"/" Help.cpp
#    fi



#####################################################
#   ȥ����Դ�������к�Ŀո�
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
#   �����޸�˵��
#####################################################
dos2unix CHANGES
echo '// ע�����ļ��ɱ���ϵͳ�Զ��޸ģ�����������CHANGES�ļ���' > Changes.h
echo >> Changes.h
echo 'const char CHANGES[] = "\' >> Changes.h   #'
sed 's/$/\\n\\/' CHANGES | sed 's/\"/\\\"/g' >> Changes.h
echo '";' >> Changes.h
sed -i 's/\\n\\";/\\n\\\n";/' Changes.h



#####################################################
#   ʹ�汾��ͬ������Version.h��İ���޸�ʱ��
#   [Rocky 2010-06-09 14:13:15
#####################################################
touch Tag/Tag_Version.cpp Help.cpp -r Version.h
