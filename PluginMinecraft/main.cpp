#include <android/log.h>
#include <EGL/egl.h>
#include <dlfcn.h>
#include <jni.h>
#include "And64InlineHook.hpp"
#include "Gui/gui.h"
#include "imgui.h"

#define LOG_TAG "PluginMinecraft"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


typedef EGLBoolean (*eglSwapBuffers_t)(
  EGLDisplay display,
  EGLSurface surface
);


static eglSwapBuffers_t orig_eglSwapBuffers = nullptr;


EGLBoolean hook_eglSwapBuffers(
  EGLDisplay display,
  EGLSurface surface
)
{
  static int count = 0;

  if (count++ % 60 == 0)
  {
    LOGI("eglSwapBuffers frame");
  }

  DrawGUIS();

  return orig_eglSwapBuffers(display, surface);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mojang_minecraftpe_imgui_onTouch(
    JNIEnv* env,
    jclass,
    jobject motionEvent)
{
    if (ImGui::GetCurrentContext() == nullptr)
        return;

    static jclass cls = nullptr;
    static jmethodID midGetActionMasked = nullptr;
    static jmethodID midGetX = nullptr;
    static jmethodID midGetY = nullptr;

    if (!cls)
    {
        jclass local = env->GetObjectClass(motionEvent);
        cls = (jclass)env->NewGlobalRef(local);
        env->DeleteLocalRef(local);

        midGetActionMasked = env->GetMethodID(cls, "getActionMasked", "()I");
        midGetX            = env->GetMethodID(cls, "getX", "()F");
        midGetY            = env->GetMethodID(cls, "getY", "()F");
    }

    const int action = env->CallIntMethod(motionEvent, midGetActionMasked);
    const float x    = env->CallFloatMethod(motionEvent, midGetX);
    const float y    = env->CallFloatMethod(motionEvent, midGetY);

    ImGuiIO& io = ImGui::GetIO();

    io.AddMousePosEvent(x, y);

    switch (action)
    {
        case 0: // ACTION_DOWN
            io.AddMouseButtonEvent(0, true);
            break;

        case 1: // ACTION_UP
        case 3: // ACTION_CANCEL
            io.AddMouseButtonEvent(0, false);
            break;
    }
}

__attribute__((constructor))
static void init()
{
  LOGI("libplugin.so loaded!");

  void *egl = dlopen(
    "libEGL.so",
    RTLD_NOW
  );

  if (!egl)
  {
    LOGI("failed to load libEGL.so");
    return;
  }


  void *addr = dlsym(
    egl,
    "eglSwapBuffers"
  );


  if (addr)
  {
    A64HookFunction(
      addr,
      (void *)hook_eglSwapBuffers,
      (void **)&orig_eglSwapBuffers
    );

    LOGI("eglSwapBuffers hooked!");
  }
  else
  {
    LOGI("failed to find eglSwapBuffers");
  }
}