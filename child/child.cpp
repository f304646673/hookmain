#include <iostream>
#include <dlfcn.h>

int main(int argc, char** argv) {
    std::cout << "client main start" << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "]\t" << argv[i] << std::endl;
    }
    std::cout << "client main end" << std::endl;
    return 0;
}
