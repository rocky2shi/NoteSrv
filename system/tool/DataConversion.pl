#!/usr/bin/perl

my $filename = $ARGV[0];

open(FILE, "< $filename") or exit 1; 
my $str = do{local $/; <FILE>}; # һ�ζ��������ļ�
close(FILE);


# Rocky 2010-06-07 16:14:24
sub toHex
{
    my ($a1, $a2, $a3, $a4) = @_;
    # ��ȥ��%
    if($a2 =~ s/%(..)/pack("c",hex($1))/ge)
    {
        $a2 =~ s/(.)/sprintf("%02X", ord($1))/ge;   # �ٰ��ַ�תΪascii
    }
    return "<div><img src=\"\/image?key=$a1&amp;file=$a2\" title=\"$a3\" id=\"$a4\"><\/div>";
}


# ת��1
if($str =~ s/<div><img src=\"\/cgi-bin\/image.cgi\?key=([0-9]+)&amp;operator=out&amp;img=([^\"]+)\" title=\"([^\"]+)\" id=\"([^\"]+)\"><\/div>/<div><img src=\"\/image?key=$1&amp;file=$2\" title=\"$3\" id=\"$4\"><\/div>/g)
{
}
# ת��2
elsif($str =~ s/<div><img src=\"\/image\?key=([0-9]+)&amp;file=([^\"]+)\" title=\"([^\"]+)\" id=\"([^\"]+)\"><\/div>/toHex($1, $2, $3, $4)/eg)
{
}
# ����ת��
else
{
    exit 1;
}

# д��
if( open(FILE, "> $filename") )
{
    print FILE $str;
    close(FILE);
}


exit 0;

