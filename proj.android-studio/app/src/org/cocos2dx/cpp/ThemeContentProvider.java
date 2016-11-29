package org.cocos2dx.cpp;

import android.content.ContentProvider;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Intent;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.net.Uri;
import android.util.Log;

/**
 * Created by songyang on 16/11/11.
 */
public class ThemeContentProvider extends ContentProvider {

    private final static String TAG = "fuck";


    private final static String TABLE_NAME = "theme";
    private final static String TABLE_COLUMN_ID = "id";
    private final static String TABLE_COLUMN_NAME = "name";

    private final static String AUTHORITY = "com.faith.providers.personprovider";
    private final static String PERSONS_PATH = "theme";
    private final static String PERSON_PATH = "theme/#";//#表示任意的数字

    private final static int PERSONS = 1;
    private final static int PERSON = 2;

    private final static UriMatcher sMatcher = new UriMatcher(UriMatcher.NO_MATCH);
    static{
        //UriMatcher类的一个方法
        sMatcher.addURI(AUTHORITY, PERSONS_PATH, PERSONS);
        sMatcher.addURI(AUTHORITY, PERSON_PATH, PERSON);
    }

    private DBOpenHelper mHelper = null;

    @Override
    public boolean onCreate() {
        mHelper = new DBOpenHelper(this.getContext());


        return true;
    }

    /*
     * 查询数据库中的信息
     */
    @Override
    public Cursor query(Uri uri, String[] projection, String selection,
                        String[] selectionArgs, String sortOrder) {
        Log.i(TAG, "query");
        SQLiteDatabase database = mHelper.getWritableDatabase();
        //UriMatcher中的一个重要方法，判断两个Uri是否相匹配
        switch(sMatcher.match(uri)){
            case PERSONS: {
                Log.i(TAG, "num = " + PERSONS);
                /*
                 * 当Uri为“content：//com.faith.providers.personprovider/person”时，触发这个模块
                 * 是指查询person表中所有的数据信息（当然条件是selection）
                 */
                return database.query(TABLE_NAME, projection, selection, selectionArgs, null, null, sortOrder);
            }
            case PERSON:
                /*
                 * 当Uri为“content：//com.faith.providers.personprovider/person/#”时，触发这个模块
                 * 是指查询当id = #(所代表的数字)时的那一条数据的具体信息
                 * 需要添加一个条件（在原先的selection条件的基础上添加）
                 */
                long id = ContentUris.parseId(uri);
                String where = TABLE_COLUMN_ID + "=" + id;
                if(selection != null && !"".equals(selection)){
                    where = where + " and " + selection;
                }
                return database.query(TABLE_NAME, projection, where, selectionArgs, null, null, sortOrder);
            default :
                //均不匹配的时候，抛出异常
                throw new IllegalArgumentException("Unknown Uri : " + uri);
        }
//        return null;
    }

    /*
     * 向数据库中插入一条信息
     */
    @Override
    public Uri insert(Uri uri, ContentValues values) {
        SQLiteDatabase database = mHelper.getWritableDatabase();
        //由于此时的values中具体是否含有数据是不确定的，所以此时需要在第二个参数中添加person表中的非主键的一列

        String themeName = values.getAsString("name");
        Cursor cursor = database.query(TABLE_NAME,new String[]{"id", "name", "isNew"},"name=?",new String[]{themeName},null,null, null);
        long id;
        if (cursor.getCount() == 0){
            id = database.insert(TABLE_NAME, null, values);
            Log.i(TAG, "tcp insert = " + values);
        } else {
            id = database.update(TABLE_NAME, values, "name=?",new String[]{themeName});
            Log.i(TAG, "tcp update = " + values);
        }

        return ContentUris.withAppendedId(uri, id);
    }

    /*
     * 删除数据库中的信息
     */
    @Override
    public int delete(Uri uri, String selection, String[] selectionArgs) {
        //此方法同query方法
//        SQLiteDatabase database = mHelper.getWritableDatabase();
//        switch(sMatcher.match(uri)){
//            case PERSONS:
//                return database.delete(TABLE_NAME, selection, selectionArgs);
//            case PERSON:
//                long id = ContentUris.parseId(uri);
//                String where = TABLE_COLUMN_ID + "=" + id;
//                if(selection != null && !"".equals(selection)){
//                    where = where + " and " + selection;
//                }
//                return database.delete(TABLE_NAME, where, selectionArgs);
//            default:
//                throw new IllegalArgumentException("Unknown Uri : " + uri);
//        }
        return 0;
    }

    /*
     *更新数据库中的信息
     */
    @Override
    public int update(Uri uri, ContentValues values, String selection,
                      String[] selectionArgs) {
        //此方法同query方法
//        SQLiteDatabase database = mHelper.getWritableDatabase();
//        switch(sMatcher.match(uri)){
//            case PERSONS:
//                return database.update(TABLE_NAME, values, selection, selectionArgs);
//            case PERSON:
//                long id = ContentUris.parseId(uri);
//                String where = TABLE_COLUMN_ID + "=" + id;
//                if(selection != null && !"".equals(selection)){
//                    where = where + " and " + selection;
//                }
//                return database.update(TABLE_NAME, values, where, selectionArgs);
//            default:
//                throw new IllegalArgumentException("Unknown Uri : " + uri);
//        }
        return 0;
    }

    /*
     * 取得Uri所对应的值的类型，是集合型或者非集合型
     * 集合型需要在其返回值前添加“vnd.android.cursor.dir/”
     * 非集合型需要在其返回值前添加"vnd.android.cursor.item/"
     */
    @Override
    public String getType(Uri uri) {
        switch(sMatcher.match(uri)){
            case PERSONS:
                //集合类型，返回值前面一般是固定的，后面的值是自己添加的，也可以加上包路径
                return "vnd.android.cursor.dir/" + TABLE_NAME;
            case PERSON:
                //非集合类型数据，返回值前面一般是固定的，后面的值是自己添加的，也可以加上包路径
                return "vnd.android.cursor.item/" + TABLE_NAME;
            default:
                throw new IllegalArgumentException("Unknown Uri : " + uri);
        }
    }
}
