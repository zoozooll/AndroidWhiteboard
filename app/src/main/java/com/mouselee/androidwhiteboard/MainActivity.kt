package com.mouselee.androidwhiteboard

import android.graphics.Color
import android.graphics.drawable.ColorDrawable
import android.os.Bundle
import android.view.*
import android.widget.PopupWindow
import android.widget.SeekBar
import android.widget.SeekBar.OnSeekBarChangeListener
import android.widget.TextView
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.mouselee.androidwhiteboard.databinding.ActivityMainBinding
import com.skydoves.colorpickerview.ColorPickerDialog
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener


class MainActivity : AppCompatActivity() {
    private var mSizeWindow: PopupWindow? = null
    private var mColorDialog: AlertDialog? = null
//    private var mColorWindow: PopupWindow? = null

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        supportActionBar!!.title = ""
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        when (item.itemId) {
            R.id.size -> {
                if (mSizeWindow == null) {
                    showSizeSelectorWindow(atView = binding.root)
                }
            }
            R.id.color -> {
                if (mColorDialog == null) {
                    val builder = ColorPickerDialog.Builder(this)
                        .setTitle("ColorPicker Dialog")
                        .setPreferenceName("MyColorPickerDialog")
                        .setPositiveButton(getString(android.R.string.ok),
                            ColorEnvelopeListener { envelope, fromUser ->
                                if (fromUser) {
                                    Configs.color = envelope.color
                                    mColorDialog!!.dismiss()
                                    mColorDialog = null
                                }
                            })
                        .setNegativeButton(getString(android.R.string.cancel)) {
                            dialogInterface, i -> dialogInterface.dismiss()
                            mColorDialog = null
                        }
                        .attachAlphaSlideBar(true) // the default value is true.
                        .attachBrightnessSlideBar(true) // the default value is true.
                        .setBottomSpace(12) // set a bottom space between the last slidebar and buttons.
                    mColorDialog = builder.show()
                }
            }
            R.id.eraser -> {
                Configs.eraserMode = !Configs.eraserMode
            }

            else -> { return false }
        }
        return true
    }

    private fun showSizeSelectorWindow(atView: View) {
        mColorDialog?.dismiss()
        val view: View = LayoutInflater.from(this).inflate(R.layout.main_window_size_selector, null)
        val seekBar = view.findViewById<View>(R.id.seek_bar) as SeekBar
        val size = view.findViewById<View>(R.id.size) as TextView
        val numSize = if (!Configs.eraserMode) Configs.width.toInt() else Configs.eraserWidth.toInt()
        seekBar.progress = numSize
        size.text = numSize.toString()
        mSizeWindow = PopupWindow(
            view, ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.WRAP_CONTENT,
            true
        )
        mSizeWindow!!.isOutsideTouchable = true
        mSizeWindow!!.setBackgroundDrawable(ColorDrawable(Color.TRANSPARENT))
        mSizeWindow!!.setOnDismissListener {
            mSizeWindow = null
        }
        mSizeWindow!!.showAtLocation(atView, Gravity.TOP or Gravity.CENTER_HORIZONTAL, 0, (supportActionBar?.height?:0) * 2)
        seekBar.setOnSeekBarChangeListener(object : OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar, progress: Int, fromUser: Boolean) {
                size.text = progress.toString()
                if (!Configs.eraserMode) {
                    Configs.width = progress.toFloat()
                } else {
                    Configs.eraserWidth = progress.toFloat()
                }
            }

            override fun onStartTrackingTouch(seekBar: SeekBar) {}
            override fun onStopTrackingTouch(seekBar: SeekBar) {}
        })
    }
}