#include "gui.h"
#include <EGL/egl.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"


void DrawGUI()
{
  static bool initialized = false;

  if (!initialized) {
    if (eglGetCurrentContext() != EGL_NO_CONTEXT) {
      ImGui::CreateContext();
  
      ImGui_ImplOpenGL3_Init( "#version 300 es");
  
      initialized = true;
    }
  }


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