package com.mouselee.androidwhiteboard

import android.content.Context
import android.content.res.Resources
import android.graphics.Bitmap
import java.io.BufferedOutputStream
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.text.SimpleDateFormat
import java.util.*

object Util {
    fun saveBitmapToFile(bitmap: Bitmap, file: String) {
        try {
            val bos = BufferedOutputStream(FileOutputStream(file))
            bitmap.compress(Bitmap.CompressFormat.PNG, 100, bos)
            bos.close()
        } catch (e: IOException) {
            e.printStackTrace()
        }

    }

    fun saveTempBitmap(bitmap: Bitmap, context: Context) {
        val fileName = SimpleDateFormat("yyyyMMddHHmmssSSS").format(Date()) + ".png"
        saveBitmapToFile(bitmap, File(context.externalCacheDir, fileName).path)
    }
}