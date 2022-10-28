//
// Created by Admin on 10/25/2022.
//

#ifndef IMGUI_UNITY_TOUCH_JNIHOOKS_H
#define IMGUI_UNITY_TOUCH_JNIHOOKS_H
#include "ImGuiUtils.h"

jmethodID MotionEvent_getX;
jmethodID MotionEvent_getY;
jmethodID MotionEvent_getAction;
jmethodID KeyEvent_getUnicodeChar;
jmethodID KeyEvent_getMetaState;
jmethodID KeyEvent_getAction;
jmethodID KeyEvent_getKeyCode;
jboolean (*old_nativeInjectEvent)(JNIEnv *, jobject, jobject);
jboolean nativeInjectEvent(JNIEnv *env, jobject thiz, jobject inputEvent)
{
    if (MENU::setup)
    {
        jclass MotionEventCls = env->FindClass(OFC("android/view/MotionEvent"));
        jclass KeyEventCls = env->FindClass(OFC("android/view/KeyEvent"));
        if (env->IsInstanceOf(inputEvent, MotionEventCls))
        {
            if (!MotionEvent_getX)
                MotionEvent_getX = env->GetMethodID(MotionEventCls, OFC("getX"),
                                                    OFC("()F"));
            if (!MotionEvent_getY)
                MotionEvent_getY = env->GetMethodID(MotionEventCls, OFC("getY"),
                                                    OFC("()F"));
            if (!MotionEvent_getAction)
                MotionEvent_getAction = env->GetMethodID(MotionEventCls, OFC("getAction"),
                                                         OFC("()I"));
            ImGuiIO &io = ImGui::GetIO();
            auto x = env->CallFloatMethod(inputEvent, MotionEvent_getX);
            auto y = env->CallFloatMethod(inputEvent, MotionEvent_getY);
            auto action = env->CallIntMethod(inputEvent,
                                             MotionEvent_getAction);
//            LOGD("X:%f\nY:%f\nAction:%i", x, y, action);

            ImGui_Android_HandleInputEvent(x,y,action);

            if (io.WantCaptureMouse)
                return true;
        } else if (env->IsInstanceOf(inputEvent, KeyEventCls))
        {
            if (!KeyEvent_getAction)
                KeyEvent_getAction = env->GetMethodID(KeyEventCls, OFC("getAction"),
                                                      OFC("()I"));
            if (env->CallIntMethod(inputEvent, KeyEvent_getAction) == 0)
            {
                if (!KeyEvent_getKeyCode)
                    KeyEvent_getKeyCode = env->GetMethodID(KeyEventCls, OFC("getKeyCode"),
                                                           OFC("()I"));
                if (!KeyEvent_getUnicodeChar)
                    KeyEvent_getUnicodeChar = env->GetMethodID(KeyEventCls,
                                                               OFC("getUnicodeChar"),
                                                               OFC("(I)I"));
                if (!KeyEvent_getMetaState)
                    KeyEvent_getMetaState = env->GetMethodID(KeyEventCls, OFC("getMetaState"),
                                                             OFC("()I"));
                ImGuiIO &io = ImGui::GetIO();
                int KeyCode = env->CallIntMethod(inputEvent, KeyEvent_getKeyCode);
                switch (KeyCode)
                {
                    case 19:
                        io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]] = true;
                        break;
                    case 20:
                        io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]] = true;
                        break;
                    case 21:
                        io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = true;
                        break;
                    case 22:
                        io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = true;
                        break;
                    case 61:
                        io.KeysDown[io.KeyMap[ImGuiKey_Tab]] = true;
                        break;
                    case 66:
                        io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = true;
                        break;
                    case 67:
                        io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = true;;
                        break;
                    case 92:
                        io.KeysDown[io.KeyMap[ImGuiKey_PageUp]] = true;
                        break;
                    case 93:
                        io.KeysDown[io.KeyMap[ImGuiKey_PageDown]] = true;
                        break;
                    case 111:
                        io.KeysDown[io.KeyMap[ImGuiKey_Escape]] = true;
                        break;
                    case 112:
                        io.KeysDown[io.KeyMap[ImGuiKey_Delete]] = true;
                        break;
                    case 122:
                        io.KeysDown[io.KeyMap[ImGuiKey_Home]] = true;
                        break;
                    case 123:
                        io.KeysDown[io.KeyMap[ImGuiKey_End]] = true;
                        break;
                    case 124:
                        io.KeysDown[io.KeyMap[ImGuiKey_Insert]] = true;
                        break;
                    default:
                        io.AddInputCharacter(env->CallIntMethod(inputEvent, KeyEvent_getUnicodeChar,
                                                                env->CallIntMethod(inputEvent,
                                                                                   KeyEvent_getMetaState)));
                        break;
                }
            }
        }
    }
    return old_nativeInjectEvent(env, thiz, inputEvent);
}
jint (*orig_RegisterNatives)(JNIEnv*, jclass, const JNINativeMethod*, jint);
jint hook_RegisterNatives(JNIEnv* env, jclass klazz, const JNINativeMethod* methods, jint methodcount)
{
    for (int i = 0; i < methodcount; ++i)
    {
        auto method = methods[i];
        if (strcmp(method.name, OFC("nativeInjectEvent")) == 0)
        {
            DobbyHook((void *) method.fnPtr, (dobby_dummy_func_t) nativeInjectEvent,
                 (dobby_dummy_func_t *) &old_nativeInjectEvent);
        }
    }
    return orig_RegisterNatives(env, klazz, methods, methodcount);
}

#endif //IMGUI_UNITY_TOUCH_JNIHOOKS_H
