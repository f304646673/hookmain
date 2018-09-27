#include <iostream>
#include <dlfcn.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>

#define SUBHOOK_X86_64
#include "subhook.h"

int main(int argc, char** argv) {
	std::cout << "main start" << std::endl;
	//void *handle = dlopen(NULL, RTLD_LAZY);
	//if (!handle) {
	//	std::cerr << "dlopen error" << std::endl;
	//}
	//else {
	//	std::cout << "cur module address:" << handle << std::endl;
	//}

	//void *ptr = dlsym(handle, "main");
	//void *ptr = dlsym(RTLD_NEXT, "main");
	//if (!ptr) {
	//	std::cerr << "dlsym error:" << dlerror() << std::endl;
	//}
	//std::cout << "client main ptr:" << ptr << std::endl;
	//std::cout << "client main ptr(by compile):" << reinterpret_cast<const void *>(main) << std::endl;

	for (int i = 0; i < argc; i++) {
		std::cout << "argv[" << i << "]\t" << argv[i] << std::endl;
	}
	return 0;
}

typedef int (*main_func)(int, char**);

int main_stub(int argc, char** argv);
subhook::Hook _main_hook;

class Inject {
public:
	Inject() {
		std::cout << "inject OK" << std::endl;

		char path[PATH_MAX];
		if(readlink("/proc/self/exe", path, PATH_MAX) <= 0){
			std::cerr << "readlink error" << std::endl;
		}
		else {
			std::cout << "cur path:" << path << std::endl;
		}
		void *handle = dlopen(NULL, RTLD_NOW|RTLD_GLOBAL);
		void *ptr = dlsym(handle, "main");
		std::cout << "inject main ptr:" << ptr << std::endl;
		_main_hook.Install((void*)main, (void*)main_stub);
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

int main_stub(int argc, char** argv) {
	std::cout << "main_stub start" << std::endl;
	while (true) {
		sleep(1);
		((main_func)_main_hook.GetTrampoline())(argc, argv);
	}
	return 0;
}
