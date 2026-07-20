#include "gui.h"
#include <EGL/egl.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "android/log.h"
#include <GLES3/gl3.h>
#define LOG_TAG "PluginMinecraft"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void DrawGUI()
{
  //LOGI("DrawGUI called");

  static bool initialized = false;
  LOGI("%s", glGetString(GL_VERSION));

  if (!initialized) {
    if (eglGetCurrentContext() != EGL_NO_CONTEXT) {
      ImGui::CreateContext();
  
      bool ok = ImGui_ImplOpenGL3_Init( "#version 300 es");
      LOGI("Init = %d", ok);
      
  
      initialized = true;
    }
  }
  
  EGLDisplay dpy = eglGetCurrentDisplay();
  EGLSurface surf = eglGetCurrentSurface(EGL_DRAW);
  
  EGLint w, h;
  eglQuerySurface(dpy, surf, EGL_WIDTH, &w);
  eglQuerySurface(dpy, surf, EGL_HEIGHT, &h);
  
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)w, (float)h);
  io.Fonts->AddFontDefault();
  
  //LOGI("Display = %d %d", w, h);
  ImGui_ImplOpenGL3_NewFrame();

  ImGui::NewFrame();


  ImGui::Begin(
    "PluginMinecraft"
  );

  ImGui::Text(
    "Hello Minecraft"
  );


  if (ImGui::Button(
    "Click me"
  ))
  {
    ImGui::Text("nice");
  }


  ImGui::End();


  ImGui::Render();


  ImGui_ImplOpenGL3_RenderDrawData(
    ImGui::GetDrawData()
  );
}