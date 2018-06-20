//
// Created by sxx.xu on 6/20/2018.
//

#ifndef SOCKETPROCESS_NATIVE_LIG_H
#define SOCKETPROCESS_NATIVE_LIG_H

#endif //SOCKETPROCESS_NATIVE_LIG_H

#include <unistd.h> // fork函数所在库
#include <sys/socket.h> // socket函数所在库
#include <sys/un.h>// socketaddr_un函数所在库
#include <string.h>//字符库
#include <stdlib.h>//标注输入输出
#include <errno.h>//Linux的错误
#include <sys/select.h>//select函数
#include <android/log.h>//android的log

#define LOG_TAG "tuch"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


int child_create_channel();

void child_listen_msg();

void child_do_work();