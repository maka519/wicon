//LoRaレシーバー
//M5Paper用

#define DEBUG_FLG (0)

#include <M5Unified.h>
#include <Arduino.h>

#include "global_config.h"

int Page;    //現在表示しているページID
int OldPage;

int lastday_condition_check = -1;
bool today_condition_check = false;

unsigned long        kp_sTime;   //キープアライブ周期開始時間

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);  //M5コアを初期化
  SetMessageFont(); //フォントをメッセージ表示用に設定
  Serial2.begin(9600, SERIAL_8N1, Port_LoRaRx, Port_LoRaTx);//RX_PIN, TX_PIN  LoRa通信
  pinMode(PIR_pin, INPUT);  //PIR接続を入力に設定
  
  InitEnv();  //環境センサ初期化
  InitSD();   //SDカードマウント
  InitLoRa(); //LoRa通信初期化
  InitData(); //データ初期化
  InitScreen(); //画面初期化
  DrawHeader();  //ヘッダー表示

  updateEnv();  //環境センサデータ更新
  SendEnv();  //環境情報送信
  kp_sTime = millis();

// 日付チェック変数を初期化
  m5::rtc_datetime_t dt_now;
  M5.Rtc.getDateTime(&dt_now); // LoRaで設定された現在時刻を取得
  lastday_condition_check = dt_now.date.date;

  //トップページ表示
  Page = top_page;
  OldPage = -1;
}

void loop() {
  
  M5.update();
  
  m5::rtc_datetime_t dt; 
  M5.Rtc.getDateTime(&dt);

//日付が変わったらリセット
  if (dt.date.date != lastday_condition_check) {
    today_condition_check = false; 
    lastday_condition_check = dt.date.date;
  }

  if ( (dt.time.hours >= 6) && (!today_condition_check) && (Page != check_condition_page) ) 
  {
    Page = check_condition_page;
  }

  //ページ制御
  if(Page != OldPage){
    //ページ切替
    switch(Page){
      case top_page:  DrawTopScreen();  break;
      case SOS_page:  DrawSOSScreen();  break;
      case txSOS_page:  DrawTxSOSScreen();  break;
      case rxm_page: DrawRxmScreen(); break;
      case txm_page: DrawTxmScreen(); break;
      case axm_page: DrawAxmScreen(); break;
      case bxm_page: DrawBxmScreen(); break;
      case cxm_page: DrawCxmScreen(); break;
      case dxm_page: DrawDxmScreen(); break;
      case exm_page: DrawExmScreen(); break;
      case rxm_page_res1: Drawrxm_res1Screen(); break;
      case bath_page: DrawbathScreen(); break;
      case bath_now_page: Drawbath_nowScreen(); break;
      case check_condition_page: Drawcheck_conditionScreen(); break;

    }
    OldPage = Page;
  }
  else {
      //ページ処理
    switch(Page){
      case top_page:  DrawTopScreen2();  break;
      case SOS_page:  DrawSOSScreen2();  break;
      case txSOS_page:  DrawTxSOSScreen2();  break;
      case rxm_page: DrawRxmScreen2(); break;
      case txm_page: DrawTxmScreen2(); break;
      case axm_page: DrawAxmScreen2(); break;
      case bxm_page: DrawBxmScreen2(); break;
      case cxm_page: DrawCxmScreen2(); break;
      case dxm_page: DrawDxmScreen2(); break;
      case exm_page: DrawExmScreen2(); break;
      case rxm_page_res1: Drawrxm_res1Screen2(); break;
      case bath_page: DrawbathScreen2(); break;
      case bath_now_page: Drawbath_nowScreen2(); break;
      case check_condition_page: Drawcheck_conditionScreen2(); break;

    }  

  }

  CheckBattery(false); //バッテリーチェック
  SOS();  //SOS処理
  CheckRxLoRa();  //LoRa受信処理
  DispTime(); //時間表示
  updateEnv();  //環境センサデータ更新
  
  if((millis()-kp_sTime) >= KPPeriod){
      SendEnv();  //環境情報送信
    kp_sTime = millis();
  }
  
  delay(500);
}
