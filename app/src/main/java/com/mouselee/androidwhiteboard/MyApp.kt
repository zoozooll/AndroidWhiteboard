package com.mouselee.androidwhiteboard

import android.app.Application
import android.content.res.AssetManager

class MyApp: Application() {
    override fun onCreate() {
        super.onCreate()
        bindAssertManager(assets)
        bindInternalPath(externalCacheDir!!.path)
    }

    override fun onTerminate() {
        super.onTerminate()
        onAppStop()
    }

    private external fun bindAssertManager(am: AssetManager)

    private external fun bindInternalPath(path: String)

    private external fun onAppStop()



    companion object {
        init {
            System.loadLibrary("native3d");
        }
    }
}