//
// Created by Aaron Lee on 2022/9/23.
//

#include "AssetHelper.h"
#include <android/asset_manager.h>
#include <cstdlib>

static AAssetManager* mgr = nullptr;

static const char* internalPath = "";

bool loadDataFromAsset(const char * const assetPath, void** buffer, size_t* size){
    AAsset* file = AAssetManager_open(mgr, assetPath, AASSET_MODE_UNKNOWN);
    if (file == nullptr) {
        return false;
    }
    *size = AAsset_getLength(file);
    *buffer = malloc(sizeof(char) * (*size));
    int rs = AAsset_read(file, *buffer, *size);
    AAsset_close(file);
    return rs < 0;
}

const char *getInternalPath() {
    return internalPath;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mouselee_androidwhiteboard_MyApp_bindAssertManager(JNIEnv *env, jobject thiz, jobject am) {
    mgr = AAssetManager_fromJava(env, am);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_mouselee_androidwhiteboard_MyApp_bindInternalPath(JNIEnv *env, jobject thiz,
                                                           jstring path) {
    internalPath = env->GetStringUTFChars(path, nullptr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_mouselee_androidwhiteboard_MyApp_onAppStop(JNIEnv *env, jobject thiz) {
    internalPath = "";
    mgr = nullptr;
}