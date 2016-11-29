//
//  VungleWrapper.h
//  PowersOfTwo
//
//  Created by Freeman on 14-4-19.
//
//

#ifndef HLINTERFACEANDROID_H_
#define HLINTERFACEANDROID_H_


class HLInterfaceAndroid{
    
public:
    static HLInterfaceAndroid * getInstance();
    
    void startGet();
    void LoadData();

public:
    int ctrl_admob_banner_switch;
    
    char *  ctrl_admob_banner_id;
    
    char * ctrl_admob_pop_id;
    
    int ctrl_fb_banner_switch;
    
    char * ctrl_fb_banner_id;
    
    char * ctrl_fb_pop_id;
    
    int ctrl_banner_switch;
    
    char * ctrl_banner_iphone_id;
    
    char * ctrl_banner_ipad_id;
    
    char * unityad_code;
    
    char * vungle_code;  //vungle id
    
    int ctrl_pop_switch;				//插屏总开关
    
    int ctrl_admob_pop_switch;		//Admob 插屏开关
    
    int ctrl_fb_pop_switch;		//Facebook 插屏开关
    
    int ctrl_unityad_pop_switch;		//UnityAD 插屏开关
    
    int ctrl_mango_pop_switch;		//芒果 插屏开关
    
    int ctrl_vungle_pop_switch; //vungle安全开关
    
    int ctrl_unsafe_admob_pop_switch;		//Admob 插屏开关
    
    int ctrl_unsafe_fb_pop_switch;		//Facebook 插屏开关
    
    
    int ctrl_unsafe_unityad_pop_switch;		//UnityAD 插屏开关
    
    int ctrl_unsafe_mango_pop_switch;		//芒果 插屏开关
    
    int ctrl_unsafe_vungle_pop_switch;      //vungle不安全开关
    
    int ctrl_admob_pop_time;
    
    int ctrl_fb_pop_time;
    
    int ctrl_unityad_pop_time;
    
    int ctrl_mango_pop_time;
    
    int ctrl_vungle_pop_time;  //vungle冷却时间
    
    int encouraged_ad_strategy;
    
    int encouraged_ad_strategy_unityad_switch;//激励型广告 unity开关
    
    int encouraged_ad_strategy_vungle_switch;//激励型广告 vungle开关
    
    char * umeng_code;
    
    char * umeng_Channel;
    
    char * flurry_key;
    
    char * flyerDevKey;
    
    char * flyerAppID;
    
    bool firebase_Switch;
    
    
    
    int market_reviwed_status;
    
    int comment_ctrl_switch;
    
    char * comment_content;
    
    char * comment_btnok;
    
    char * comment_btncancel;
    
    char * comment_download_link;
    
    int itunes_update_ctrl_switch;
    
    char * itunes_update_content;
    
    char * itunes_update_btnok;
    
    char * itunes_update_btncancel;
    
    char * itunes_updated_url;
    
    //预留开关
    int ctrl_internal_01;
    
    int ctrl_internal_02;
    
    int ctrl_internal_03;
    
    int ctrl_internal_04;
    
    int ctrl_internal_05;
    
    int girl_start;
    
    int ctrl_unlock_img_switch;
    
    int ctrl_left_banner_switch;
    
    int ctrl_left_pop_switch;//安全补余插屏开关
    
    int ctrl_unsafe_left_pop_switch ;//不安全补余插屏开关
    char * girl_img_url;
    
    char * ctrl_left_banner_id;
    
    char * ctrl_left_pop_id;
    
    char * ctrl_fixed_pop_id;
    
    char * ctrl_btn_img_url_01;
    char * ctrl_btn_img_url_02;
    char * ctrl_btn_img_url_03;
    char * ctrl_btn_img_url_04;
    
    int loading_left_pop_switch;
    int loading_admob_pop_switch;
    int loading_fb_pop_switch;
    int loading_mango_pop_switch;
    
    int button_left_pop_switch;
    int button_unityad_pop_switch;
    int button_vungle_pop_switch;
    
    int ctrl_unlock_video_switch;
    int ctrl_unlock_default_video_switch;
    char * girl_video_url;
    
    
    int admob_pop_level;
    int unityad_pop_level;
    int vungle_pop_level;
    int fb_pop_level;
    int left_pop_level;
    
};

#endif
