rm bin/*.o
for file in *.cpp; do
    g++ -o bin/$(basename $file .cpp).o -c $file
done
g++ -o bin/run.out bin/*.o  -lcrypto
bin/run.out