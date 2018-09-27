g++ child.cpp -lfunchook -Wl,-rpath,../funchook/lib -L../funchook/lib -I../funchook/include -g -rdynamic -ldl -o child
