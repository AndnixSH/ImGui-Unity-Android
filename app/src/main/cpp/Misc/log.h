//
// Created by Admin on 10/23/2022.
//

#ifndef IMGUI_UNITY_TOUCH_LOG_H
#define IMGUI_UNITY_TOUCH_LOG_H
#include <android/log.h>
#include "Utils.h"

#define LOG_TAG OFC("ModMenu")

#define LOGD(...) ((void)__android_log_print(3, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(6, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(4,  LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(5,  LOG_TAG, __VA_ARGS__))


#endif //IMGUI_UNITY_TOUCH_LOG_H
