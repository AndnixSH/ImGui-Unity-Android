#ifndef IMGUI_UNITY_TOUCH_IMGUIUTILS_H
#define IMGUI_UNITY_TOUCH_IMGUIUTILS_H

IMGUI_IMPL_API int32_t  ImGui_Android_HandleInputEvent(int x, int y, int type);
IMGUI_IMPL_API void     ImGui_AndroidNewFrame(int window_width, int window_height);

static double g_Time = 0.0;

int32_t ImGui_Android_HandleInputEvent(int x, int y, int type = 0)
{
    ImGuiIO& io = ImGui::GetIO();

    switch (type)
    {
        case 0:
            io.MouseDown[0] = true;
            break;
        case 1:
            io.MouseDown[0] = false;
            break;
        default:
            break;
    }
    io.MousePos = ImVec2((float)x, (float)y);
    return 0;
}

void ImGui_AndroidNewFrame(int window_width, int window_height)
{
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    int display_width = window_width;
    int display_height = window_height;

    io.DisplaySize = ImVec2((float)window_width, (float)window_height);
    if (window_width > 0 && window_height > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_width / window_width, (float)display_height / window_height);

    // Setup time step
    struct timespec currentTimeSpec{};
    clock_gettime(CLOCK_MONOTONIC, &currentTimeSpec);
    double current_time = (double)(currentTimeSpec.tv_sec) + (currentTimeSpec.tv_nsec / 1000000000.0);
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
    g_Time = current_time;
}

#endif