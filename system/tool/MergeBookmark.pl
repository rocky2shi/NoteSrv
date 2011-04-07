#!/usr/bin/perl

#
# 合并书签数据
#







#
# 2008-11-09
# 串转为关联数组（数组追加）
#   ToArray($src, \@dest);
#       $dest[i]{name} = value
#
sub ToArray
{
    my $str = $_[0];
    my @ary = ();
    my %list = ();

    # 取出{...}
    $str =~ s/({(?:\'([^\']+)\':\'((?:\\\'|[^\'])+)\'[,]?)+})/push(@ary, $1)/eg;

    # 取出name:value
    my $i = @{$_[1]};   # 指向数组末尾
    my $it;
    foreach $it (@ary)
    {
        # 匹配'和'之间包含（非'或\'）的字符，如：'rocky\\'1111111'
        $it =~ s/\'([^\']+)\':\'((?:\\\'|[^\'])+)\'/{$_[1]->[$i]{$1}=$2}/eg;
        $i++;
    }
}

#
# 2008-11-09
# 关联数组化为串
#
sub ToStr
{
    my ($i, $str);
    my @ary = @{$_[0]};

    $str = '[';
    for($i=0; $i<@ary; $i++)
    {
        $str .='{';
        foreach $it (keys %{$ary[$i]})
        {
            $str .= "'$it':'$ary[$i]{$it}',";
        }
        $str =~ s/,$//;
        $str .='},';
        $index = $i;
    }
    $str =~ s/,$//;
    $str .= ']';

    return $str;
}

#
# 2008-11-09
# 以键'create'（即书签位置）排序，去掉重复项；注意，函数的写法固定了其只能用于本（书签）模块；
#   @ary = Uniq(\@ary);
#
sub Uniq
{
    my ($i, $n, $index);
    my @ary = sort( {$$a{'create'} cmp $$b{'create'}} @{$_[0]} );
    my @ary2 = (); # 保存去重结果

    # 删除重复项
    $index = 0;
    for($i=1; $i<@ary; $i++)
    {
        if('' ne $ary[$i]{'create'} && $ary[$index]{'create'} eq $ary[$i]{'create'})
        {
            delete $ary[$i];
            next;
        }
        $index = $i;
    }

    # 去掉空项
    $n = 0;
    @ary2 = ();
    for($i=0; $i<@ary; $i++)
    {
        $ary2[$n++] = $ary[$i] if(exists($ary[$i]));
    }

    # 返回前，复原次序（按创建时间）；
    return @ary2;
}

# 执行合并
# $bookmark = Merge($bookmark1, $bookmark2);
sub Merge
{
    my (@bookmark);
    my $bookmark1 = $_[0];
    my $bookmark2 = $_[1];

    # 合并书签
    @bookmark = ();
    ToArray($bookmark1, \@bookmark);
    ToArray($bookmark2, \@bookmark);
    @bookmark = Uniq(\@bookmark);

    return ToStr(\@bookmark);
}

# 读文件内容到串符串中
sub ReadFile
{
    my $filename = $_[0];
    open(FILE, "< $filename") or exit 1; 
    my $str = do{local $/; <FILE>}; # 一次读入整个文件
    return $str;
}

sub WriteFile
{
    my $filename = $_[0];
    my $str = $_[1];
    open(FILE, "> $filename") or exit 1; 
    print FILE $str;
}

#my $b1 = "[{'key':'20100526000242','title':'易讯','create':'20100526000254'},{'key':'20100525014042','title':'中华人民共和国','create':'20100526000120'}]";

#my $b2 = "[{'key':'20100526000242','title':'500wan','create':'20100526000254'},{'key':'20100525014042','title':'中华人民共和国','create':'20100526000120'}]";


$ARGC = @ARGV;
if(3 != $ARGC)
{
    exit 1;
}


my $b1 = ReadFile($ARGV[0]);
my $b2 = ReadFile($ARGV[1]);
my $b3 = Merge($b1, $b2);

WriteFile($ARGV[2], $b3);

exit 0;

=end
print "$b1\n";
print "--------------------------------------------------------\n";
print "$b2\n";
print "--------------------------------------------------------\n";
print "$b3\n";


