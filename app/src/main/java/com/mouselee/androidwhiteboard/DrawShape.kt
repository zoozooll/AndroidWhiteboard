package com.mouselee.androidwhiteboard

import android.graphics.*
import kotlin.math.roundToInt

abstract class DrawShape {
    var startX = 0
        protected set
    var startY = 0
        protected set
    var endX = 0
        protected set
    var endY = 0
        protected set

    val mPaint: Paint = Paint().apply {
        isAntiAlias = true
        style = Paint.Style.STROKE
        isDither = true
        strokeJoin = Paint.Join.ROUND
        strokeCap = Paint.Cap.ROUND
        if (!Configs.eraserMode) {
            strokeWidth = Configs.width
            color = Configs.color
//            xfermode = PorterDuffXfermode(PorterDuff.Mode.SRC_OVER)
        } else {
            strokeWidth = Configs.eraserWidth
            color = Color.BLACK
            alpha = Configs.eraserAlpha
//            xfermode = PorterDuffXfermode(PorterDuff.Mode.CLEAR)
        }
    }

    open fun touchDown(startX: Int, startY: Int) {
        this.startX = startX
        this.startY = startY
    }

    open fun touchUp(endX: Int, endY: Int) {
        this.endX = endX
        this.endY = endY
    }

    abstract fun touchMove(currentX: Int, currentY: Int)

    abstract fun draw(canvas: Canvas)
}