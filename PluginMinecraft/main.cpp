#include <android/log.h>
#include <EGL/egl.h>

#include "shadowhook.h"
#include "gui.h"

#define LOG_TAG "PluginMinecraft"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


typedef EGLBoolean (*eglSwapBuffers_t)(
  EGLDisplay display,
  EGLSurface surface
);


static eglSwapBuffers_t orig_eglSwapBuffers = nullptr;
static void *eglSwapBuffers_stub = nullptr;


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

  shadowhook_init(
    SHADOWHOOK_MODE_UNIQUE,
    false
  );


  void *addr = shadowhook_dlsym(
    "libEGL.so",
    "eglSwapBuffers"
  );


  if (addr)
  {
    eglSwapBuffers_stub = shadowhook_hook_func_addr(
      addr,
      (void *)hook_eglSwapBuffers,
      (void **)&orig_eglSwapBuffers
    );


    if (eglSwapBuffers_stub)
    {
      LOGI("eglSwapBuffers hooked!");
    }
    else
    {
      LOGI("hook failed");
    }
  }
  else
  {
    LOGI("failed to find eglSwapBuffers");
  }
}