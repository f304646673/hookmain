g++ inject.cpp ../subhook/subhook.c -I../subhook -g -ldl -fPIC -rdynamic -shared -o inject.so
