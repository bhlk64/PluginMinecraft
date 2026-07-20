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
  LOGI("DrawGUI called");
  glClearColor(1, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  LOGI("glError = %x", glGetError());
  LOGI("context = %p", eglGetCurrentContext());

  static bool initialized = false;

  if (!initialized) {
    if (eglGetCurrentContext() != EGL_NO_CONTEXT) {
      ImGui::CreateContext();
  
      ImGui_ImplOpenGL3_Init( "#version 300 es");
  
      initialized = true;
    }
  }
  ImGuiIO& io = ImGui::GetIO();
  LOGI("Display = %.1f %.1f", io.DisplaySize.x, io.DisplaySize.y);

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