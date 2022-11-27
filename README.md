### Implementation of a Blockchain in c++ for the Conrad 2022 challenge.
\
To run the demo, run ```/bin/bash ./scripts/demo.sh```
\
You can specify the number of docker containers to run, for instance ```/bin/bash ./scripts/demo.sh 5``` would run 5 containers
\
Arguments specified after the # of containers are flags used by the compiler
\
To run with debug mode, run ```/bin/bash ./scripts/demo.sh 5 -D DEBUG```
\
To run a single instance, run ```docker compose up -d``` or, without docker, run ```/bin/bash ./scripts/entrypoint.sh```
