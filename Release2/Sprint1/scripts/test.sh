#!/bin/bash

for i in $(seq 50)
do
    echo -e 'POST /index.html HTTP/1.0\r\nHost: www.toto.com:8080\r\n\r\n\0' | nc www.toto.com 8080
done
