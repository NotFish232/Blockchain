#!/bin/bash

for i in `seq 1 10`; do
    echo "Generating rsa key pair for user $i"
    openssl genrsa -out "./keys/user${i}_private.pem" 2048 2> /dev/null
    openssl rsa -in "./keys/user${i}_private.pem" -pubout -out "./keys/user${i}_public.pem" 2> /dev/null
done
