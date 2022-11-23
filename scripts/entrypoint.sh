#!/bin/bash

if [ ! -f ./bin/main.out ]; then
    /bin/bash ./scripts/compile.sh
fi

./bin/main.out $BLOCKCHAIN_USER $BLOCKCHAIN_PORT 
