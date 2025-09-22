//LoRaレシーバー
//M5Paper用

#define DEBUG_FLG (0)

#include <M5Unified.h>
#include <Arduino.h>

#include "global_config.h"

int Page;    //現在表示しているページID
int OldPage;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);  //M5コアを初期化
  SetMessageFont(); //フォントをメッセージ表示用に設定
  Serial2.begin(9600, SERIAL_8N1, 25, 32);
  M5.Rtc.begin();

  InitSD();   //SDカードマウント
  InitLoRa(); //LoRa通信初期化
  InitData(); //データ初期化
  InitScreen(); //画面初期化
  DrawHeader();  //ヘッダー表示

  //トップページ表示
  Page = top_page;
  OldPage = -1;
}

void loop() {
  
  M5.update();
  
  //ページ制御
  if(Page != OldPage){
    //ページ切替
    switch(Page){
      case top_page:  DrawTopScreen();  break;
      case SOS_page:  DrawSOSScreen();  break;
      case txSOS_page:  DrawTxSOSScreen();  break;
      case rxm_page: DrawRxmScreen(); break;
      case txm_page: DrawTxmScreen(); break;

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

    }  

  }

  CheckBattery(false); //バッテリーチェック
  SOS();  //SOS処理
  CheckRxLoRa();  //LoRa受信処理
  DispTime(); //時間表示

  delay(500);
}
