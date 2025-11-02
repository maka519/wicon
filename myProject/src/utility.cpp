//ユーティリティ関数
#include <M5Unified.h>
#include <Arduino.h>
#include <ctime>
#include <M5UnitENV.h>

#include "global_config.h"

SHT3X sht3x;

String PANID;    //通信するネットワークのPANID
String OWNID;    //自身のOWNID
String USERS[maxUsers];   //ユーザID(OWNID),ユーザ名　の文字列を要素とした配列で定義すする
String MESSAGES[maxMessages];    //メッセージID,メッセージ,カテゴリーID　の文字列を要素とした配列で定義する
String CATEGORIES[maxCategories];    //カテゴリーID,カテゴリ名　の文字列を要素とした配列で定義する

int USERS_LENGTH;       //ユーザ数
int MESSAGES_LENGTH;    //メッセージ数
int CATEGORIES_LENGTH;  //カテゴリー数

int old_bl = -1;

//環境センサ関連
float temp, humi;   //温度、湿度
int pirCnt;       //人感センサカウンタ
int wbgt;          //熱中症危険度
int pirValue;       //人感センサ

//熱中症関係
struct WBGTEntry {
  int humi;
  int temp;
  int wbgt;
};
WBGTEntry wbgtTable[] = {
  {20,40,29},{25,40,30},{30,40,31},{35,40,32},{40,40,33},{45,40,34},{50,40,35},{55,40,35},{60,40,36},{65,40,37},{70,40,38},{75,40,39},{80,40,40},{85,40,41},{90,40,42},{95,40,43},{100,40,44},
  {20,39,28},{25,39,29},{30,39,30},{35,39,31},{40,39,32},{45,39,33},{50,39,34},{55,39,35},{60,39,35},{65,39,36},{70,39,37},{75,39,38},{80,39,39},{85,39,40},{90,39,41},{95,39,42},{100,39,43},
  {20,38,28},{25,38,28},{30,38,29},{35,38,30},{40,38,31},{45,38,32},{50,38,33},{55,38,34},{60,38,35},{65,38,35},{70,38,36},{75,38,37},{80,38,38},{85,38,39},{90,38,40},{95,38,41},{100,38,42},
  {20,37,27},{25,37,28},{30,37,29},{35,37,29},{40,37,30},{45,37,31},{50,37,32},{55,37,33},{60,37,34},{65,37,35},{70,37,35},{75,37,36},{80,37,37},{85,37,38},{90,37,39},{95,37,40},{100,37,41},
  {20,36,26},{25,36,27},{30,36,28},{35,36,29},{40,36,29},{45,36,30},{50,36,31},{55,36,32},{60,36,33},{65,36,34},{70,36,34},{75,36,35},{80,36,36},{85,36,37},{90,36,38},{95,36,39},{100,36,39},
  {20,35,25},{25,35,26},{30,35,27},{35,35,28},{40,35,29},{45,35,29},{50,35,30},{55,35,31},{60,35,32},{65,35,33},{70,35,33},{75,35,34},{80,35,35},{85,35,36},{90,35,37},{95,35,38},{100,35,38},
  {20,34,25},{25,34,25},{30,34,26},{35,34,27},{40,34,28},{45,34,29},{50,34,29},{55,34,30},{60,34,31},{65,34,32},{70,34,33},{75,34,33},{80,34,34},{85,34,35},{90,34,36},{95,34,37},{100,34,37},
  {20,33,24},{25,33,25},{30,33,25},{35,33,26},{40,33,27},{45,33,28},{50,33,28},{55,33,29},{60,33,30},{65,33,31},{70,33,32},{75,33,32},{80,33,33},{85,33,34},{90,33,35},{95,33,35},{100,33,36},
  {20,32,23},{25,32,24},{30,32,25},{35,32,25},{40,32,26},{45,32,27},{50,32,28},{55,32,28},{60,32,29},{65,32,30},{70,32,31},{75,32,31},{80,32,32},{85,32,33},{90,32,34},{95,32,34},{100,32,35},
  {20,31,22},{25,31,23},{30,31,24},{35,31,24},{40,31,25},{45,31,26},{50,31,27},{55,31,27},{60,31,28},{65,31,29},{70,31,30},{75,31,30},{80,31,31},{85,31,32},{90,31,33},{95,31,33},{100,31,34},
  {20,30,21},{25,30,22},{30,30,23},{35,30,24},{40,30,24},{45,30,25},{50,30,26},{55,30,27},{60,30,27},{65,30,28},{70,30,29},{75,30,29},{80,30,30},{85,30,31},{90,30,32},{95,30,32},{100,30,33},
  {20,29,21},{25,29,21},{30,29,22},{35,29,23},{40,29,24},{45,29,24},{50,29,25},{55,29,26},{60,29,26},{65,29,27},{70,29,28},{75,29,29},{80,29,29},{85,29,30},{90,29,31},{95,29,31},{100,29,32},
  {20,28,20},{25,28,21},{30,28,21},{35,28,22},{40,28,23},{45,28,23},{50,28,24},{55,28,25},{60,28,25},{65,28,26},{70,28,27},{75,28,28},{80,28,28},{85,28,29},{90,28,30},{95,28,30},{100,28,31},
  {20,27,19},{25,27,20},{30,27,21},{35,27,21},{40,27,22},{45,27,23},{50,27,23},{55,27,24},{60,27,25},{65,27,25},{70,27,26},{75,27,27},{80,27,27},{85,27,28},{90,27,29},{95,27,29},{100,27,30},
  {20,26,18},{25,26,19},{30,26,20},{35,26,20},{40,26,21},{45,26,22},{50,26,22},{55,26,23},{60,26,24},{65,26,24},{70,26,25},{75,26,26},{80,26,26},{85,26,27},{90,26,28},{95,26,28},{100,26,29},
  {20,25,18},{25,25,18},{30,25,19},{35,25,20},{40,25,20},{45,25,21},{50,25,22},{55,25,22},{60,25,23},{65,25,23},{70,25,24},{75,25,25},{80,25,25},{85,25,26},{90,25,27},{95,25,27},{100,25,28},
  {20,24,17},{25,24,18},{30,24,18},{35,24,19},{40,24,19},{45,24,20},{50,24,21},{55,24,21},{60,24,22},{65,24,22},{70,24,23},{75,24,24},{80,24,24},{85,24,25},{90,24,26},{95,24,26},{100,24,27},
  {20,23,16},{25,23,17},{30,23,17},{35,23,18},{40,23,19},{45,23,19},{50,23,20},{55,23,20},{60,23,22},{65,23,22},{70,23,22},{75,23,23},{80,23,23},{85,23,24},{90,23,25},{95,23,25},{100,23,26},
  {20,22,15},{25,22,16},{30,22,17},{35,22,17},{40,21,18},{45,22,18},{50,22,19},{55,22,19},{60,22,21},{65,22,21},{70,22,21},{75,22,22},{80,22,22},{85,22,23},{90,22,24},{95,22,24},{100,22,25},
  {20,21,15},{25,21,15},{30,21,16},{35,21,16},{40,20,17},{45,21,17},{50,21,18},{55,21,19},{60,21,20},{65,21,20},{70,21,20},{75,21,21},{80,21,21},{85,21,22},{90,21,23},{95,21,23},{100,21,24}
};
const int wbgtTableSize = sizeof(wbgtTable) / sizeof(wbgtTable[0]);


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

//熱中症危険度取得
int lookupWBGT(float temp, float humi) {
  int bestWBGT = wbgtTable[0].wbgt;
  int minDiff = 9999;
 
  for (int i = 0; i < wbgtTableSize; i++) {
    int diff = abs(wbgtTable[i].temp - (int)temp) + abs(wbgtTable[i].humi - (int)humi);
    if (diff < minDiff) {
      minDiff = diff;
      bestWBGT = wbgtTable[i].wbgt;
    }
  }
  return bestWBGT;
}

//環境センサ初期化
void InitEnv() {
    if (!sht3x.begin(&Wire, SHT3X_I2C_ADDR, 21, 22, 400000U)) {
        //Serial.println("SHT30 not found");
        M5.Display.setCursor(60, 300);
        M5.Display.print("SHT30 not found");
        while (1) delay(1);
    }
    pirCnt = 0;
}

//環境センサ情報更新
void updateEnv() {
    sht3x.update();
    temp = sht3x.cTemp;
    humi = sht3x.humidity;
    pirValue = digitalRead(PIR_pin);
    if(pirValue == 1) {
        pirCnt++;
        if(pirCnt >= 255) pirCnt = 255;
    }
    wbgt = lookupWBGT(temp, humi);
}