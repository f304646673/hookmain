g++ inject.cpp -lfunchook -Wl,-rpath,../funchook/lib -L../funchook/lib -I../funchook/include -g -ldl -fPIC -rdynamic -shared -o inject.so
