package com.mouselee.androidwhiteboard

import android.graphics.Canvas
import android.graphics.Path

class CurveShape : DrawShape() {
    protected var mPath: Path = Path()
    override fun touchDown(startX: Int, startY: Int) {
        super.touchDown(startX, startY)
        //设置曲线开始点
        mPath.moveTo(startX.toFloat(), startY.toFloat())
        val a = floatArrayOf(startX.toFloat(), startY.toFloat(), 0f, 0f)
    }

    override fun touchMove(currentX: Int, currentY: Int) {
        val mMiddleX = ((currentX + startX) / 2).toFloat()
        val mMiddleY = ((currentY + startY) / 2).toFloat()

        // 贝赛尔曲线
        mPath.quadTo(startX.toFloat(), startY.toFloat(), mMiddleX, mMiddleY)
        val temp = floatArrayOf(startX.toFloat(), startY.toFloat(), mMiddleX, mMiddleY)

        startX = currentX
        startY = currentY
    }

    //把曲线绘制到画布上
    override fun draw(canvas: Canvas) {
        canvas.drawPath(mPath, mPaint)
    }
}