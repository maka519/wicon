//ユーティリティ関数
#include <M5Unified.h>
#include <Arduino.h>
#include <ctime>

#include "global_config.h"

String PANID;    //通信するネットワークのPANID
String OWNID;    //自身のOWNID
String USERS[maxUsers];   //ユーザID(OWNID),ユーザ名　の文字列を要素とした配列で定義すする
String MESSAGES[maxMessages];    //メッセージID,メッセージ,カテゴリーID　の文字列を要素とした配列で定義する
String CATEGORIES[maxCategories];    //カテゴリーID,カテゴリ名　の文字列を要素とした配列で定義する

int USERS_LENGTH;       //ユーザ数
int MESSAGES_LENGTH;    //メッセージ数
int CATEGORIES_LENGTH;  //カテゴリー数

int old_bl = -1;



//データ初期化
//システム起動時に初期設定しておくべきデータはこの関数で行う
void InitData() {
    int i;
    //SDカードからユーザ、メッセージ、カテゴリデータを読み込む
    USERS_LENGTH = ReadSDData(USERS,"USERS.txt", maxUsers);
    MESSAGES_LENGTH = ReadSDData(MESSAGES,"MESSAGES.txt", maxMessages);
    CATEGORIES_LENGTH = ReadSDData(CATEGORIES,"CATEGORIES.txt", maxCategories);
    //SDカードからその他設定データを読み込む
    SetConfigFromSD();

}

//ユーザ名取得関数
//事前処理：USERS配列にIDとユーザ名を準備しておく
//ユーザidにffffを指定したら全員と返す
String getUserName(String userid){
    String tstr, vname, vval;
    int i,idx;
    userid.toLowerCase();   //OWNIDは小文字に統一
    if(userid == "ffff")    return "全員";  //ブロードキャスト
    for(i = 0; i < USERS_LENGTH; i++) {
        tstr = USERS[i];
        idx = 0;
        //IDとユーザ名を抽出
        idx = tstr.indexOf(',');
        vname = tstr.substring(0, idx);
        vval = tstr.substring(idx+1);
        vname.toLowerCase();    //OWNIDは小文字に統一
        if(userid == vname){
            return vval;    //ユーザ名を返す
        }
    }
    return "";  //該当なし
}

//メッセージ取得関数
//事前処理：MESSAGES配列にIDとメッセージ、カテゴリーIDを準備しておく
//メッセージはmstrに、そのメッセージのカテゴリーIDが戻り値となる
int getMessage(int messageid, String *mstr){
    String tstr, vval;
    int i,idx,lidx, vid, cid;
    cid = -1;
    vval = "";
    for(i = 0; i < MESSAGES_LENGTH; i++) {
        tstr = MESSAGES[i];
        idx = 0;
        lidx = 0;
        //IDとメッセージ,カテゴリーIDを抽出
        idx = tstr.indexOf(',');
        lidx = tstr.lastIndexOf(',');
        vid = tstr.substring(0, idx).toInt();
        vval = tstr.substring(idx+1,lidx);
        cid = tstr.substring(lidx+1).toInt(); 
        if(messageid == vid){
            i =  MESSAGES_LENGTH;
        }
    }
    *mstr = vval;    //メッセージ
    return cid;  //該当なし
}

//カテゴリー名取得関数
//事前処理：CATEGORIES配列にIDとカテゴリー名を準備しておく
String getCategory(int categoryid){
    String tstr, vval;
    int i,idx, vid;
    for(i = 0; i < CATEGORIES_LENGTH; i++) {
        tstr = CATEGORIES[i];
        idx = 0;
        //IDとカテゴリー名を抽出
        idx = tstr.indexOf(',');
        vid = tstr.substring(0, idx).toInt();
        vval = tstr.substring(idx+1);
        if(categoryid == vid){
            return vval;    //カテゴリー名を返す
        }
    }
    return "";  //該当なし
}

//メッセージ表示フォント設定
void SetMessageFont(){
    display.setTextSize(2);  //文字サイズ2倍
    display.setFont(&fonts::efontJA_24_b);   //全角文字24×24ピクセル（半角は12×24ピクセル）
    //M5Paperの画面幅は540なので、全角文字で22文字（24×22＝528）を1行で表示できる
    //1行の高さは48（24×2）でデザインすること
}

//RTC
//RTC初期化
void InitRTC(){
    if (!M5.Rtc.isEnabled()) {
        //display.println("RTC not found");
        while (true) {
            delay(500);
        }
    }
  //display.println("RTC found");
}

void SetRTC(m5::rtc_datetime_t utc_time){
    //UTC時間を日本時間にしてRTCにセット
    M5.Rtc.setDateTime(UTC2JST(utc_time));
}

void DispTime(){
    char buff[20];
    display.setTextSize(1);
    display.setCursor(Disw/2+65,2);
    m5::rtc_datetime_t now = M5.Rtc.getDateTime();
    DispSelTime(now);
    display.setTextSize(2);
}

void DispSelTime(m5::rtc_datetime_t stime){
    char buff[20];
    sprintf(buff, "%4d/%02d/%02d %02d:%02d", stime.date.year,stime.date.month,stime.date.date,stime.time.hours,stime.time.minutes);
    display.print(buff);
}

//バッテリーチェック
void CheckBattery(bool IsForcedDisplay) {
    int bl = M5.Power.getBatteryLevel();
    if(IsForcedDisplay || (old_bl != bl)){
        if(bl > 70)         displayImage("img/battery3.png", Disw-105, 50);
        else if(bl > 50)    displayImage("img/battery2.png", Disw-105, 50);
        else if(bl > 30)    displayImage("img/battery1.png", Disw-105, 50);
        else                displayImage("img/battery0.png", Disw-105, 50);
        old_bl = bl;
    }
}

//時間データ受信頻度表示
void DisplayFrt(int frt){
    switch(frt){
      case 0:  displayImage("img/lora0.png", Disw-205, 40); break;
      case 1:  displayImage("img/lora1.png", Disw-205, 40); break;
      case 2:  displayImage("img/lora2.png", Disw-205, 40); break;
      case 3:  displayImage("img/lora3.png", Disw-205, 40); break;
    }
}

//UTCをJSTに変換
m5::rtc_datetime_t UTC2JST(m5::rtc_datetime_t utc_time) {
    m5::rtc_datetime_t jst_time;

    int y = utc_time.date.year;
    int m = utc_time.date.month;
    int d = utc_time.date.date;
    int h = utc_time.time.hours;
    
    //日本時間に変換                 
    h += 9;
    if ( h>23 ){
        h -= 24;
        d++;;
        if (m==2){  // ２月の場合
            if ( (y % 4) == 0 ){
                if(d>28){
                d=1;
                m++;
                }
            }else{
                if(d>29){
                d=1;
                m++;
                }              
            }
        }else if ( (m % 2) == 0){ // ２月以外の偶数月の場合
            if ( d>30 ){
            d = 1;
            m++;
            if ( m>12 ){
                y++;
            }
            }      
        }else{  //　奇数月の場合
            if ( d>31 ){
            d = 1;
            m++;          
            }
        }
    }
    jst_time.date.year = y;
    jst_time.date.month = m;
    jst_time.date.date = d;
    jst_time.time.hours = h;
    jst_time.time.minutes = utc_time.time.minutes;
    jst_time.time.seconds = utc_time.time.seconds;
    return jst_time;
}