package org.cocos2dx.cpp;

import android.graphics.Bitmap;

import org.cocos2dx.lib.Cocos2dxHelper;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;


public class KTUtils {

    public static void saveBitmap(String filename, Bitmap bmp, Bitmap.CompressFormat fmt){

        FileOutputStream out = null;
        try {
            out = new FileOutputStream(new File(Cocos2dxHelper.getCocos2dxWritablePath(), filename));
            bmp.compress(fmt, 100, out); // bmp is your Bitmap instance
            // PNG is a lossless format, the compression factor (100) is ignored
            if (out != null) {
                out.flush();
                out.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
//        finally {
//            try {
//
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//        }

    }

}
