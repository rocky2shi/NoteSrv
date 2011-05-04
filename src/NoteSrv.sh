#!/bin/bash

[[ $1 != "" ]] && port=$1 || port=17890

echo Listen on $port

export LOG_DEBUG=0
cd ../system/ && nohup ./NoteSrv $port >/dev/null 2>&1 &

