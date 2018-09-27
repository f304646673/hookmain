#include <iostream>
#include <dlfcn.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>

#define SUBHOOK_X86_64
#include "subhook.h"

//extern "C" int main(int argc, char** argv) {
//	std::cout << "inject main" << std::endl;
//	return 0;
//}

typedef int (*main_func)(int, char**);
subhook::Hook _main_hook;

int main_stub(int argc, char** argv) {
	std::cout << "main_stub start" << std::endl;
	for (int i = 0; i < argc; i++) {
		std::cout << "main_stub argv[" << i << "]\t" << argv[i] << std::endl;
	}

	while (true) {
		sleep(1);
		std::cout << "main_stub argc address:" << static_cast<void*>(&argc) << std::endl;
		std::cout << "main_stub argv address:" << argv << std::endl;
		((main_func)_main_hook.GetTrampoline())(argc, argv);
	}
	return 0;
}

/*
typedef int (*libc_start_main_func)();
subhook::Hook libc_start_main_hook;
typedef int (*main_func)(int, char**);
main_func main_ori = nullptr;
libc_start_main_func pfunc = nullptr;

extern "C" int main_stub(int argc, char** argv) {
	std::cout << "inject main" << std::endl;
	return 0;
}

int libc_start_main_stub() {
	//std::cout << "libc_start_main_stub start" << std::endl;
	//main_ori = (main_func)p;
	//return ((libc_start_main_func)libc_start_main_hook.GetTrampoline())();
	return pfunc();
}
*/

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
		if (!handle) {
			std::cerr << "dlopen error" << std::endl;
		}
		else {
			std::cout << "cur module address:" << handle << std::endl;
		}

		/*
		void *ptr = dlsym(handle, "__libc_start_main");
		std::cout << "inject __libc_start_main ptr:" << ptr << std::endl;
		//sleep(20);
		if (libc_start_main_hook.Install((void*)ptr, (void*)libc_start_main_stub), 1) {
			std::cout << "hook suc" << std::endl;
			pfunc = (libc_start_main_func)libc_start_main_hook.GetTrampoline();
		}
		else {
			std::cerr << "hook fail" << std::endl;
		}
		*/

		void *ptr = dlsym(handle, "main");
		std::cout << "inject main ptr:" << ptr << std::endl;
		//sleep(20);
		if (_main_hook.Install((void*)ptr, (void*)main_stub), 1) {
			std::cout << "hook suc" << std::endl;
		}
		else {
			std::cerr << "hook fail" << std::endl;
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

//int main_test(int argc, char** argv) {
//	std::cout << "inject main" << std::endl;
//	return 0;
//}
