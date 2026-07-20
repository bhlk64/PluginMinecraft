#include <android/log.h>
#include <dobby.h>
#include <EGL/egl.h>

#include "gui.h"

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
  DrawGUI();

  if (orig_eglSwapBuffers)
  {
    return orig_eglSwapBuffers(
      display,
      surface
    );
  }

  return EGL_FALSE;
}


__attribute__((constructor))
static void init()
{
  LOGI("libplugin.so loaded!");

  void* addr = DobbySymbolResolver(
    "libEGL.so",
    "eglSwapBuffers"
  );

  if (addr)
  {
    DobbyHook(
      addr,
      (void*)hook_eglSwapBuffers,
      (void**)&orig_eglSwapBuffers
    );

    LOGI("eglSwapBuffers hooked!");
  }
  else
  {
    LOGI("failed to find eglSwapBuffers");
  }
}