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
    // ImGui chưa khởi tạo thì bỏ qua
    if (ImGui::GetCurrentContext() == nullptr)
        return;

    // Cache class + method ID
    static jclass cls = nullptr;
    static jmethodID midGetActionMasked = nullptr;
    static jmethodID midGetX = nullptr;
    static jmethodID midGetY = nullptr;

    if (!cls)
    {
        cls = (jclass)env->NewGlobalRef(env->GetObjectClass(motionEvent));

        midGetActionMasked = env->GetMethodID(
            cls,
            "getActionMasked",
            "()I"
        );

        midGetX = env->GetMethodID(
            cls,
            "getX",
            "()F"
        );

        midGetY = env->GetMethodID(
            cls,
            "getY",
            "()F"
        );
    }

    const jint action = env->CallIntMethod(
        motionEvent,
        midGetActionMasked
    );

    const jfloat x = env->CallFloatMethod(
        motionEvent,
        midGetX
    );

    const jfloat y = env->CallFloatMethod(
        motionEvent,
        midGetY
    );

    ImGuiIO& io = ImGui::GetIO();

    io.MousePos = ImVec2(x, y);

    switch (action)
    {
        case 0: // ACTION_DOWN
            io.MouseDown[0] = true;
            break;

        case 1: // ACTION_UP
        case 3: // ACTION_CANCEL
            io.MouseDown[0] = false;
            break;

        case 2: // ACTION_MOVE
            break;
    }

    /*LOGI(
        "Touch: action=%d x=%.1f y=%.1f down=%d",
        action,
        x,
        y,
        io.MouseDown[0]
    );*/
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