#include <iostream>
#include <dlfcn.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>

extern "C" {
#include "funchook.h"
}

typedef int (*main_func)(int, char**);
main_func g_main_ori = nullptr;

int main_stub(int argc, char** argv) {
	std::cout << "main_stub start" << std::endl;
	for (int i = 0; i < argc; i++) {
		std::cout << "main_stub argv[" << i << "]\t" << argv[i] << std::endl;
	}

	while (true) {
		sleep(1);
		g_main_ori(argc, argv);
	}
	return 0;
}

class Inject {
public:
	Inject() {
		std::cout << "inject OK" << std::endl;
/*
		char path[PATH_MAX];
		if(readlink("/proc/self/exe", path, PATH_MAX) <= 0){
			std::cerr << "readlink error" << std::endl;
		}
		else {
			std::cout << "cur path:" << path << std::endl;
		}

*/
		void *handle = dlopen(NULL, RTLD_NOW|RTLD_GLOBAL);
		if (!handle) {
			std::cerr << "dlopen error" << std::endl;
		}
		else {
			std::cout << "cur module address:" << handle << std::endl;
		}

		void *ptr = dlsym(handle, "main");
        if (ptr) {
            std::cout << "inject main ptr:" << ptr << std::endl;
        }
        else {
            std::cerr << "inject get main ptr error" << std::endl;
        }
		//sleep(20);

		funchook_t *funchook = funchook_create();
		g_main_ori = (main_func)ptr;
		int rv = funchook_prepare(funchook, (void**)&g_main_ori, (void*)main_stub);
		if (rv) {
			std::cerr << "funchook_prepare error " << rv << std::endl;
		}

		rv = funchook_install(funchook, 0);
		if (rv) {
			std::cerr << "funchook_install error " << rv << std::endl;
		}
	}
};

class InjectHolder {
public:
	InjectHolder() {
	};
private:
	Inject _inject;
};

static InjectHolder g_inject_holder;
