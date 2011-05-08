#!/bin/bash



ls -1 | while read file;
do
    name_orig=$(echo $file | g2u.sh)
    echo $name_orig
    mv "$file" "$name_orig"
done

