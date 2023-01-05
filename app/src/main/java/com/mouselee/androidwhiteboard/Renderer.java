package com.mouselee.androidwhiteboard;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class Renderer implements GLSurfaceView.Renderer {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        init();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        sizeChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        update();
    }

    private native void init();

    private native void sizeChanged(int width, int height);

    private native void update();

    private native void finish();

    native void updateCanvasTexture(int texture, boolean eraser);

}
