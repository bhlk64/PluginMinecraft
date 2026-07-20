#include <android/log.h>

#define LOG_TAG "PluginMinecraft"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

__attribute__((constructor))
static void init() {
  LOGI("libplugin.so loaded!");
}