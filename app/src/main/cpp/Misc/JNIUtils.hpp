//
// Created by Admin on 10/22/2022.
//

#ifndef IMGUI_UNITY_TOUCH_JNIUTILS_HPP
#define IMGUI_UNITY_TOUCH_JNIUTILS_HPP
#include <jni.h>
#include "log.h"

JavaVM *jvm;
jclass cls_UnityPlayer;
jfieldID fid_CurrUnityPlayer;
JNIEnv* getEnv() {
    JNIEnv *env;
    int status = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if(status < 0) {
        status = jvm->AttachCurrentThread(&env, NULL);
        if(status < 0) {
            LOGE(OFC("Error Getting JNI"), 1);
            return nullptr;
        }
    }
    return env;
}
jobject getGlobalContext(JNIEnv *env)
{
    jclass activityThread = env->FindClass(OFC("android/app/ActivityThread"));
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, OFC("currentActivityThread"), OFC("()Landroid/app/ActivityThread;"));
    jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = env->GetMethodID(activityThread, OFC("getApplication"), OFC("()Landroid/app/Application;"));
    jobject context = env->CallObjectMethod(at, getApplication);
    return context;
}
void displayKeyboard(bool pShow) {
    JNIEnv *env = getEnv();
    jclass ctx = env->FindClass(OFC("android/content/Context"));
    jfieldID fid = env->GetStaticFieldID(ctx, OFC("INPUT_METHOD_SERVICE"), OFC("Ljava/lang/String;"));
    jmethodID mid = env->GetMethodID(ctx, OFC("getSystemService"), OFC("(Ljava/lang/String;)Ljava/lang/Object;"));
    jobject context = env->GetStaticObjectField(cls_UnityPlayer, fid_CurrUnityPlayer);
    jobject InputManObj = env->CallObjectMethod(context, mid, (jstring) env->GetStaticObjectField(ctx, fid));
    jclass ClassInputMethodManager = env->FindClass(OFC("android/view/inputmethod/InputMethodManager"));
    jmethodID toggleSoftInputId = env->GetMethodID(ClassInputMethodManager, OFC("toggleSoftInput"), OFC("(II)V"));
    if (pShow) {
        env->CallVoidMethod(InputManObj, toggleSoftInputId, 2, 0);
    } else {
        env->CallVoidMethod(InputManObj, toggleSoftInputId, 0, 0);
    }
}

#endif //IMGUI_UNITY_TOUCH_JNIUTILS_HPP
