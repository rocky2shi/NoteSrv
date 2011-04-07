#!/bin/bash

now=`date "+%Y-%m-%d %H:%M:%S"`
echo ===================[$now]=================== >> .history
find . | /bin/grep '\.h$\|\.cpp$' | xargs wc -l | sort -g >> .history
echo >> .history

