package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.media.Image;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.util.Log;
import android.widget.Button;
import android.widget.ImageView;
import android.provider.MediaStore;
import android.view.*;
import android.content.*;
import android.widget.TextView;

import org.cocos2dx.lib.*;
import com.skyeye.solitaire.*;
import com.soundcloud.android.crop.Crop;

import java.io.*;
import java.io.FileOutputStream;
import java.lang.*;

public class EditThemeActivity extends Activity {

    static final String PortraitEditFile = "theme_portrait_edit.jpg";

    static final String LandscapeEditFile = "theme_landscape_edit.jpg";

    static final String PortraitFile = "theme_portrait.jpg";

    static final String LandscapeFile = "theme_landscape.jpg";

    static final String PreviewFile = "theme.png";

    Uri orginUri;

    float _aspectRatio;

    ImageView iv1;

    ImageView iv2;

    boolean isVertical;

    TextView title;

    TextView title1;

    TextView title2;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit_theme);

        iv1 = (ImageView) findViewById(R.id.imageView);
        iv2 = (ImageView) findViewById(R.id.imageView2);

        title = (TextView)findViewById(R.id.textView);
        title1 = (TextView)findViewById(R.id.textView2);
        title2 = (TextView)findViewById(R.id.textView3);

        final Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();

        mCocos2dxActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                final String str = AppActivity.getLocalization("TID_UI_EDIT_THEME");
                mCocos2dxActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        EditThemeActivity.this.title.setText(str);
                    }
                });
            }
        });

        mCocos2dxActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                final String str = AppActivity.getLocalization("TID_UI_EDIT_PORTRT");
                mCocos2dxActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        EditThemeActivity.this.title1.setText(str);
                    }
                });
            }
        });

        mCocos2dxActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                final String str = AppActivity.getLocalization("TID_UI_EDIT_LANDSCAPE");
                mCocos2dxActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        EditThemeActivity.this.title2.setText(str);
                    }
                });
            }
        });


        Button bt1 = (Button)findViewById(R.id.button3);
        bt1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Uri destination = Uri.fromFile(new File(Cocos2dxHelper.getCocos2dxWritablePath(), PortraitEditFile));
//                Log.i("fuck", "des = " + destination);
                int x = 640;
                int y = (int) (640 * _aspectRatio);
                Crop.of(orginUri, destination).withAspect(x,y).withMaxSize(x, y).start(EditThemeActivity.this);
                EditThemeActivity.this.isVertical = true;
            }
        });

        Button bt2 = (Button)findViewById(R.id.button4);
        bt2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Uri destination = Uri.fromFile(new File(Cocos2dxHelper.getCocos2dxWritablePath(), LandscapeEditFile));
                int y = 640;
                int x = (int) (640 * _aspectRatio);
                Crop.of(orginUri, destination).withAspect(x,y).withMaxSize(x, y).start(EditThemeActivity.this);
                EditThemeActivity.this.isVertical = false;
            }
        });

        final Button cancel = (Button)findViewById(R.id.button);
        cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();

                mCocos2dxActivity.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        AppActivity.onThemeFileCanceled();

                    }
                });
                EditThemeActivity.this.finish();
            }
        });

        mCocos2dxActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                final String str = AppActivity.getLocalization("TID_UI_CANCEL");
                mCocos2dxActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        cancel.setText(str);
                    }
                });
            }
        });

        final Button submit = (Button)findViewById(R.id.button2);
        mCocos2dxActivity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                final String str = AppActivity.getLocalization("TID_UI_CONFIRM");
                mCocos2dxActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        submit.setText(str);
                    }
                });
            }
        });
        submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Bitmap bitmap = ((BitmapDrawable)EditThemeActivity.this.iv1.getDrawable()).getBitmap();
                KTUtils.saveBitmap(PortraitFile, bitmap, Bitmap.CompressFormat.JPEG);

                Bitmap bitmap2 = ((BitmapDrawable)EditThemeActivity.this.iv2.getDrawable()).getBitmap();
                KTUtils.saveBitmap(LandscapeFile, bitmap2, Bitmap.CompressFormat.JPEG);

                Bitmap preview = Bitmap.createScaledBitmap(bitmap,88,131,true);
                KTUtils.saveBitmap(PreviewFile, preview, Bitmap.CompressFormat.PNG);

                final Handler handler = new Handler();
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        //Do something after 100ms
                        Cocos2dxActivity mCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();

                        mCocos2dxActivity.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                AppActivity.onThemeFileChanged();
                            }
                        });
                    }
                }, 1000 / 24);

                EditThemeActivity.this.finish();
            }
        });

        orginUri = (Uri) getIntent().getParcelableExtra("origin");

//        iv1.setImageURI(orginUri);
//        iv2.setImageURI(orginUri);


        Resources resources = this.getResources();
        DisplayMetrics dm = resources.getDisplayMetrics();
        int width3 = dm.widthPixels;
        int height3 = dm.heightPixels;
        _aspectRatio = (float) height3/width3;

        Bitmap _sourceImg = null;
        try
        {
            _sourceImg = MediaStore.Images.Media.getBitmap(this.getContentResolver() , orginUri);
        }
        catch (Exception e)
        {
            //handle exception
        }

        float imgAspRat = (float)_sourceImg.getHeight()/_sourceImg.getWidth();

        int x=0,y=0,w=0,h=0;
        if (imgAspRat > _aspectRatio) {
            w = _sourceImg.getWidth();
            h = (int) (w * _aspectRatio);
            y = (int) ((_sourceImg.getHeight()-h)*0.5F);
        } else {
            h = _sourceImg.getHeight();
            w = (int) (h/_aspectRatio);
            x = (int) ((_sourceImg.getWidth()-w)*0.5F);
        }

        Bitmap img = Bitmap.createBitmap(_sourceImg, x,y,w,h);
        img = Bitmap.createScaledBitmap(img,640,(int)(640 * _aspectRatio),true);
        iv1.setImageBitmap(img);

        x=y=w=h=0;
        if (imgAspRat > 1F/_aspectRatio) {
            w = _sourceImg.getWidth();
            h = (int) (w / _aspectRatio);
            y = (int) ((_sourceImg.getHeight()-h)*0.5F);
        } else {
            h = _sourceImg.getHeight();
            w = Math.min((int) (h*_aspectRatio), _sourceImg.getWidth());

            x = Math.max((int) ((_sourceImg.getWidth()-w)*0.5F), 0);
        }

        img = Bitmap.createBitmap(_sourceImg, x,y,w,h);
        img = Bitmap.createScaledBitmap(img,(int)(640 * _aspectRatio),640,true);
        iv2.setImageBitmap(img);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == Crop.REQUEST_CROP){
            if (resultCode == RESULT_OK){
                if (isVertical){

                    Uri destination = Uri.fromFile(new File(Cocos2dxHelper.getCocos2dxWritablePath(), PortraitEditFile));
                    Bitmap img = null;
                    try
                    {
                        img = MediaStore.Images.Media.getBitmap(EditThemeActivity.this.getContentResolver() , destination);
                        img = Bitmap.createScaledBitmap(img,640,(int)(640 * _aspectRatio),true);
                        EditThemeActivity.this.iv1.setImageBitmap(img);
                    }
                    catch (Exception e)
                    {
                        //handle exception
                    }

                } else {
                    Uri destination = Uri.fromFile(new File(Cocos2dxHelper.getCocos2dxWritablePath(), LandscapeEditFile));
                    Bitmap img = null;
                    try
                    {

                        img = MediaStore.Images.Media.getBitmap(EditThemeActivity.this.getContentResolver() , destination);
                        img = Bitmap.createScaledBitmap(img,(int)(640 * _aspectRatio),640,true);
                        EditThemeActivity.this.iv2.setImageBitmap(img);
                    }
                    catch (Exception e)
                    {
                        //handle exception
                    }

                }
            } else {

            }
        }
    }
}
