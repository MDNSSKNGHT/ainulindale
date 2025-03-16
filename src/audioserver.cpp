#include <chrono>
#include <thread>
#include <sys/wait.h>
#include <unistd.h>

#include <reflect.h>
#include <Injector.hpp>

#define LOG_TAG "audioserver"
#include "logging.hpp"

#include "audioserver.hpp"

void thread_task(pid_t pid) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Injector iSh(pid, "/system/lib64/libshadowhook.so");
    Injector iNt(pid, "/system/lib64/libnorgothrond.so");

    LOGI("Injector for libshadowhook %d", iSh.Inject());
    LOGI("Injector for libnorgothrond %d", iNt.Inject());
}

int main(int argc, char *argv[]) {
    pid_t child;
    int status;

    if ((child = fork())) {
        std::thread t(thread_task, child);
        t.join();

        waitpid(child, &status, 0);
    } else {
        reflect_execves(audioserver, argv + 1, nullptr, (size_t *)argv - 1);
    }

    return 0;
}
