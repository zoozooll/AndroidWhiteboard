package com.mouselee.androidwhiteboard

import android.content.Context
import android.graphics.*
import android.graphics.drawable.GradientDrawable.LINE
import android.graphics.drawable.GradientDrawable.RECTANGLE
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View

class CanvasView : View {
    constructor(context: Context?) : super(context) {}
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs) {}
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    ) {
    }

    constructor(
        context: Context?,
        attrs: AttributeSet?,
        defStyleAttr: Int,
        defStyleRes: Int
    ) : super(context, attrs, defStyleAttr, defStyleRes) {
    }

    private val editHistory = ArrayList<DrawShape>()

    private var curShape: DrawShape? = null

    private lateinit var cacheBitmap: Bitmap
    private lateinit var cacheCanvas: Canvas

    var onCanvasDrew: ((Bitmap) -> Unit)? = null

    init {

    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        cacheBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888)
        cacheCanvas = Canvas(cacheBitmap)
    }

    override fun onDraw(canvas: Canvas) {
        val count = canvas.saveCount
//        redrawAll(canvas)
//        curShape?.draw(canvas)
        canvas.restoreToCount(count)
    }

    private fun redrawAll(canvas: Canvas) {
        editHistory.forEach {
            it.draw(canvas)
        }
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val currentX = event.x.toInt()
        val currentY = event.y.toInt()
        return when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                val mStartX = event.x.toInt()
                val mStartY = event.y.toInt()
                when (Configs.drawType) {
                    DrawType.CURVE -> curShape = CurveShape()
                    DrawType.WIPE -> {}
                    DrawType.RECTANGLE -> {}
                    DrawType.OVAL -> {}
                    DrawType.LINE -> {}
                    DrawType.MULTI_LINE -> {}
                }
                curShape?.touchDown(mStartX, mStartY)
                true
            }
            MotionEvent.ACTION_MOVE -> {
                curShape?.touchMove(currentX, currentY)
                invalidate()
                true
            }
            MotionEvent.ACTION_UP -> {
                curShape?.touchUp(currentX, currentY)
                editHistory.add(curShape!!)
                drawCacheBitmap()
                curShape = null
                invalidate()
//                Util.saveTempBitmap(cacheBitmap, context)
                onCanvasDrew?.invoke(cacheBitmap)
                true
            }
            else -> false
        }
    }

    private fun drawCacheBitmap() {
        cacheCanvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
        curShape?.draw(cacheCanvas)
    }
}