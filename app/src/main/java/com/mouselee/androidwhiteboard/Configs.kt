package com.mouselee.androidwhiteboard

import android.graphics.Color
import androidx.annotation.ColorInt

object Configs {
    var drawType: DrawType = DrawType.CURVE

    var width: Float = 4f.toPx

    @ColorInt
    var color: Int = Color.YELLOW

    var eraserAlpha: Int = 255

    var eraserWidth: Float = 16f.toPx

    var eraserMode: Boolean = false
}