#include "MENU.hpp"
#include "Misc/log.h"
#include "Misc/JNIUtils.hpp"
#include "Misc/JNIHooks.h"

#include <ByNameModding/BNM.hpp>
#include <dobby.h>

#include <dlfcn.h>
#include <jni.h>
#include <unistd.h>

#define targetLib OFC("libil2cpp.so")

// Hook eglSwapBuffers for rendering ImGui
EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    if (!MENU::setup) {
        MENU::Init();
    }

    ImGuiIO &io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_AndroidNewFrame(MENU::GlWidth(), MENU::GlHeight());
    ImGui::NewFrame();

    // Render ImGui windows here.
    MENU::Menu("Mod Menu", true);

    // Rendering
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Tab]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_PageUp]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_PageDown]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Escape]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Delete]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Home]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_End]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Insert]] = false;

    if (MENU::clearMouse) {
        io.MousePos = ImVec2(-1, -1);
        MENU::clearMouse = false;
    }

    return old_eglSwapBuffers(dpy, surface);
}

void hack_thread() {
    LOGI(OFC("Hack Thread Started"));
    sleep(3);
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLib));
    BNM::AttachIl2Cpp();

    InitResolveFunc(MENU::GlHeight, std::string(OFC("UnityEngine.Screen::get_height")));
    InitResolveFunc(MENU::GlWidth, std::string(OFC("UnityEngine.Screen::get_width")));

    address = findLibrary(targetLib);

    // Get eglHandle ptr
    auto eglHandle = dlopen(OFC("libunity.so"), RTLD_LAZY);
    if (!eglHandle) {
        LOGE(OFC("Failed to get eglHandle address: %s"), dlerror());
        BNM::DetachIl2Cpp();
        pthread_exit(nullptr);
    }
    dlerror();    /* Clear any existing error */
    // Get eglSwapBuffers ptr
    auto eglSwapBuffers = dlsym(eglHandle, OFC("eglSwapBuffers"));
    if (!eglSwapBuffers) {
        LOGE(OFC("Failed to get eglSwapBuffers address: %s"), dlerror());
        BNM::DetachIl2Cpp();
        pthread_exit(nullptr);
    }

//    auto eglSwapBuffersAddress = (uintptr_t)dlsym(RTLD_NEXT, "eglSwapBuffers");

    // Hook eglSwapBuffer
    LOGI(OFC("eglSwapBuffer Address: %p"), eglSwapBuffers);
    DobbyHook(eglSwapBuffers, (dobby_dummy_func_t) hook_eglSwapBuffers,
              (dobby_dummy_func_t *) &old_eglSwapBuffers);


    dlclose(eglHandle);
    BNM::DetachIl2Cpp();
    pthread_exit(nullptr);
}

#include <thread>
[[maybe_unused]] __attribute__((constructor))
void lib_main() { std::thread(hack_thread).detach(); }

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * reserved)
{
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    BNM::HardBypass(env);

    cls_UnityPlayer = env->FindClass(OFC("com/unity3d/player/UnityPlayer"));
    fid_CurrUnityPlayer = env->GetStaticFieldID(cls_UnityPlayer,
                                                            OFC("currentActivity"),
                                                            OFC("Landroid/app/Activity;"));

    DobbyHook((void*)env->functions->RegisterNatives,
              (dobby_dummy_func_t)hook_RegisterNatives,
              (dobby_dummy_func_t *)&orig_RegisterNatives);

    return JNI_VERSION_1_6;
}
