package org.cocos2dx.cpp;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

/**
 * Created by songyang on 16/11/14.
 */
public class DBOpenHelper extends SQLiteOpenHelper {
    public DBOpenHelper(Context context) {
        super(context, "theme_setup.db", null, 1);
    }

    //数据库第一次创建时候调用，
    public void onCreate(SQLiteDatabase db) {
        Log.i("fuck", "onCreate");
        db.execSQL("create table theme(id integer primary key autoincrement, name varchar(20), isNew varchar(20))");
    }

    //数据库文件版本号发生变化时调用
    public void onUpgrade(SQLiteDatabase arg0, int arg1, int arg2) {

    }
}
