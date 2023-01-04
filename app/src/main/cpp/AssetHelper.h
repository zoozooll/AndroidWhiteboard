//
// Created by Aaron Lee on 2022/9/23.
//

#ifndef NATIVE3DENGINE_ASSETHELPER_H
#define NATIVE3DENGINE_ASSETHELPER_H

#include <android/asset_manager_jni.h>

bool loadDataFromAsset(const char * const assetPath, void** buffer, size_t* size);

const char* getInternalPath();

#endif //NATIVE3DENGINE_ASSETHELPER_H
