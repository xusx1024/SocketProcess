#include <jni.h>
#include <string>
#include "native_lib.h"

int m_child;
const char *userId;
const char *PATH = "data/data/com.shengxing.socketprocess/my.sock";

int child_create_channel() { // 创建
    unlink(PATH);
    int connfd = 0;
    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(sockaddr_un));// 清空内存
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, PATH);
    if (bind(listenfd, (const sockaddr *) &addr, sizeof(sockaddr_un)) < 0) {
        LOGE("绑定错误");
    }

    listen(listenfd, 5);//最大监听5个客户端
    while (1) {
        // 返回客户端地址, 阻塞式函数
        connfd = accept(listenfd, NULL, NULL);
        if (connfd < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                LOGE("读取错误");
                return 0;
            }
        }
        m_child = connfd;
        break;
    }
    return 1;
}

void child_listen_msg() {// 读
    fd_set rfds;
    struct timeval timeout = {3, 0};
    while (1) {
        FD_ZERO(&rfds);
        FD_SET(m_child, &rfds);
        int r = select(m_child + 1, &rfds, NULL, NULL, &timeout);
        LOGE("读取消息前 %d ", r);
        if (r > 0) {
            char pkg[256] = {0};
            if (FD_ISSET(m_child, &rfds)) {// 保证读到的信息是指定的客户端的
                int result = read(m_child, pkg, sizeof(pkg));
                // 开启服务
                execlp("am", "am", "startservice", "--user", userId,
                       "com.shengxing.socketprocess/com.shengxing.socketprocess.ProcessService",
                       (char *) NULL);
                break;
            }
        }
    }
}

void child_do_work() {
// 开启socket
    if (child_create_channel()) {
        child_listen_msg();
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_shengxing_socketprocess_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_shengxing_socketprocess_Watcher_createWatcher(JNIEnv *env, jobject instance,
                                                       jstring userId_) {
    userId = env->GetStringUTFChars(userId_, 0);
// 开启双进程

    pid_t fpid = fork();
    if (fpid == 0) {// 子进程,即我们的守护进程
        child_do_work();
    }


    env->ReleaseStringUTFChars(userId_, userId);
}
/**
 * 双工机制
 * 单工机制
 */
extern "C" JNIEXPORT void JNICALL
Java_com_shengxing_socketprocess_Watcher_connectToMonitor(JNIEnv *env, jobject instance) {
    int socked;
    struct sockaddr_un addr;
    while (1) {
        LOGE("客户端,父进程开始连接...");
        socked = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (socked < 0) {
            LOGE("连接失败了");
            return;
        }

        memset(&addr, 0, sizeof(sockaddr_un));
        addr.sun_family = AF_LOCAL;
        strcpy(addr.sun_path, PATH);

        if (connect(socked, (const sockaddr *) &addr, sizeof(sockaddr_un)) < 0) {
            LOGE("连接失败");
            close(socked);
            sleep(1);
            continue;
        }
        LOGE("连接成功");
        break;
    }

}
