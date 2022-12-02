#!/bin/bash

if [ ! -f ./bin/main.out ]; then
    /bin/bash ./scripts/compile.sh
fi

# if any env variable isn't set, run without args
if [[ -z $BC_username || -z $BC_url || -z $BC_connection_url ]]; then
    ./bin/main.out
else
    ./bin/main.out $BC_username $BC_url $BC_connection_url 
fi
