#include "gui.h"
#include <EGL/egl.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include <GLES3/gl3.h>
#include <chrono>

#include "explorer.h"

#include "android/log.h"
#define LOG_TAG "PluginMinecraft"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void DrawGUIS()
{
  //LOGI("DrawGUI called");

  static bool initialized = false;
  //LOGI("%s", glGetString(GL_VERSION));

  if (!initialized) {
    if (eglGetCurrentContext() != EGL_NO_CONTEXT) {
      ImGui::CreateContext();
  
      bool ok = ImGui_ImplOpenGL3_Init( "#version 300 es");
      LOGI("Init = %d", ok);
      ImGuiIO& io = ImGui::GetIO();
      io.Fonts->AddFontDefault();
  
      initialized = true;
    }
  }
  
  ImGuiIO& io = ImGui::GetIO();
  
  EGLDisplay dpy = eglGetCurrentDisplay();
  EGLSurface surf = eglGetCurrentSurface(EGL_DRAW);
  
  EGLint w, h;
  eglQuerySurface(dpy, surf, EGL_WIDTH, &w);
  eglQuerySurface(dpy, surf, EGL_HEIGHT, &h);
  
  io.DisplaySize = ImVec2((float)w, (float)h);
  
  
  //LOGI("Display = %d %d", w, h);
  ImGui_ImplOpenGL3_NewFrame();
  static auto last = std::chrono::steady_clock::now();
  auto now = std::chrono::steady_clock::now();
  
  io.DeltaTime = std::chrono::duration<float>(now - last).count();
  
  if (io.DeltaTime <= 0.0f)
      io.DeltaTime = 1.0f / 60.0f;

last = now;

  ImGui::NewFrame();


  DrawFileExplorer();


  ImGui::Render();


  ImGui_ImplOpenGL3_RenderDrawData(
    ImGui::GetDrawData()
  );
}