//トップページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>
#include <M5UnitENV.h>

#include "global_config.h"

SHT3X sht3x;
int32_t Disw;    //スクリーン幅(M5paper: 540)
int32_t Dish;    //スクリーン高さ(M5paper: 960)
M5GFX display;

LGFX_Button txm_btn;
LGFX_Button rxm_btn;
LGFX_Button bath_btn;


int top_prebtn = -1;
int pre_umes;

//熱中症関係
struct WBGTEntry {
  int temp;
  int humi;
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

//画面初期化
//各ページのボタンの初期化もここで行う
void InitScreen() {
    display.begin();
    
    if(display.isEPD())
    {
        display.setColorDepth(8);//電子ペーパー製品は最大8ビットのビット深度をサポートします。
        display.setEpdMode(epd_fastest);
    }
    else
    {
        display.setColorDepth(16);
    }

    display.clear(TFT_WHITE);
    delay(1000);
    Disw = display.width();
    Dish = display.height();

    //各ページのボタン初期化
    //ヘッダー
    sos_btn.initButton(&display, 5+Disw/4, 50, Disw/2, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "SOS", 2, 2);
    //topページ
    txm_btn.initButton(&display, Disw/2, 525, Disw-20, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "送る", 2, 2);
    rxm_btn.initButton(&display, Disw/2, 625, Disw-20, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "見る", 2, 2);
    bath_btn.initButton(&display, Disw/2, 825, Disw-20, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "お風呂", 2, 2);

    if (!sht3x.begin(&Wire, SHT3X_I2C_ADDR, 21, 22, 400000U)) {
   Serial.println("SHT30 not found");
   M5.Display.setCursor(60, 300);
   M5.Display.print("SHT30 not found");
   while (1) delay(1);
 }

    //SOSページ
    sos_cancel_btn.initButton(&display, 5+Disw/4, 400, Disw/3, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "間違い", 2, 2);
    sos_done_btn.initButton(&display, Disw-(5+Disw/4), 400, Disw/3, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "発信終", 2, 2);

    //内容ページ
    txa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "体調", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    txb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "会話", 2, 2);
    txc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "入浴", 2, 2);
    txd.initButton(&display, Disw/2, 550, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "仕事", 2, 2);
    txe.initButton(&display, Disw/2, 650, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "その他", 2, 2);
    txcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);

    //体調ページ
    axa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "体調が", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    axb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "体が痛", 2, 2);
    axc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "助けて", 2, 2);
    axcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);
    
    //会話ページ
    bxa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "元気で", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    bxb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "一緒に", 2, 2);
    bxc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "買い物", 2, 2);
    bxcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);

    //入浴ページ
    cxa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "お風呂", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    //cxb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "会話", 2, 2);
    //cxc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "入浴", 2, 2);
    cxcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);

    //仕事ページ
    dxa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "楽しい", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    dxb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "つらい", 2, 2);
    dxc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "相談し", 2, 2);
    dxd.initButton(&display, Disw/2, 550, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "悩みが", 2, 2);
    dxcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);

    //その他ページ
    exa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "なくし", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    exb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "ひま", 2, 2);
    exc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "病院に", 2, 2);
    exd.initButton(&display, Disw/2, 550, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "買い物", 2, 2);
    exe.initButton(&display, Disw/2, 650, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "助けて", 2, 2);
    excancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);

    //受信メッセージ表示ページ
    rxcancel_btn.initButton(&display, Disw/4, 900, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);
    rxrep_btn.initButton(&display, Disw-(Disw/4), 900, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "返信", 2, 2);
    rxprev_btn.initButton(&display, Disw/4, 750, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "前へ", 2, 2);
    rxnext_btn.initButton(&display, Disw-(Disw/4), 750, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "次へ", 2, 2);

    //受信メッセージ返信ページ
    rxm_res1.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "わかりました", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    rxm_res2.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "返信１", 2, 2);
    rxm_res3.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "返信２", 2, 2);
    rxm_res4.initButton(&display, Disw/2, 550, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "返信３", 2, 2);
    rxm_res_cancel_btn.initButton(&display, Disw/4, 900, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);

    //お風呂ページ
    take_a_bath.initButton(&display, Disw/2, 500, Disw, 540, TFT_BLACK, 0xeeee, TFT_BLACK, "入る", 2, 2);
    bath_cancel_btn.initButton(&display, Disw/4, 900, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);

    //お風呂中ページ
    finish_bath.initButton(&display, Disw/2, 500, Disw, 540, TFT_BLACK, 0xeeee, TFT_BLACK, "", 2, 2);

    /*
    //テスト
    display.setCursor(10, 40);
    display.println("Screen Width: "+String(Disw));
    display.println("Screen Height: "+String(Dish));
    display.println("M5.Lcd.getCursorY: "+String(M5.Lcd.getCursorY()));
    display.println("M5.Lcd.getCursorY: "+String(M5.Lcd.getCursorY()));
    */
}

//ヘッダー描画
void DrawHeader() {
    display.fillRect(0,0,Disw,100,TFT_WHITE);
    delay(300);
    display.drawFastHLine( 0, 99, Disw, TFT_BLACK); //エリア境界に水平線を引く
    sos_btn.drawButton();
    DisplayFrt(frt);
    CheckBattery(true);
}

//トップページ描画
void DrawTopScreen() {
    //トップ画面初期化
    //レイアウト
    //フレーム描画1
    //環境情報
    display.fillRect(3,110,Disw-6,300,TFT_BLACK);
    //メッセージ
    display.fillRect(3,420,Disw-6,350,TFT_BLACK);
    delay(200);
    //フレーム描画2
    //環境情報
    display.fillRect(5,112,Disw-10,295,TFT_WHITE);
    //メッセージ
    display.fillRect(5,422,Disw-10,345,TFT_WHITE);
    delay(200);

    //テキスト
    //環境情報
    //display.setCursor(5,112);
    //display.print("環境情報");

    //メッセージ
    display.setCursor(5,422);
    display.print("メッセージ");
    display.setCursor(Disw-250,680);
    display.print("未読：　　");   
    
    //ボタン
    txm_btn.drawButton();
    rxm_btn.drawButton();
    bath_btn.drawButton();

    //変数初期化
    top_prebtn = -1;
    pre_umes = -1;

    /*動作確認用（本番は消す）
    display.setTextSize(1);
    display.setCursor(3,800);
    display.println("動作確認用");
    display.println("ユーザ名取得："+getUserName("ca02"));
    String mes;
    int cid;
    cid = getMessage(2, &mes);
    display.println("メッセージ取得："+mes);
    display.println("カテゴリー取得："+getCategory(cid));
    display.setTextSize(2);*/
}

void DrawTopScreen2() {
    //トップ画面処理
    m5::touch_point_t touchPoint;
    int umes;

    //環境情報
    //TODO:環境情報を取得し、表示を更新する
    M5.update();
    sht3x.update();
    float temp, humi;
    unsigned long titi = 0;
    unsigned long now = millis();
    
    temp = sht3x.cTemp;
    humi = sht3x.humidity;
    int wbgt = lookupWBGT(temp, humi);
    int pirValue = digitalRead(32);

    Serial.printf("temp:%6.2f C\n", temp);
    Serial.printf("humi:%6.2f %%\n", humi);
    Serial.printf("PIR: %d\n", pirValue);
    Serial.printf("WBGT: %d\n\n", wbgt);

    display.setCursor(5, 112);
    display.printf("温度:%6.2f C", temp);

    display.setCursor(5, 162);
    display.printf("湿度:%6.2f %%", humi);

    display.setCursor(5, 212);
    if (wbgt <= 20) {
        display.print("熱中症:ほぼ安全");
        displayImage("img/WBGT1.png", 350, 125);
    }
    else if (21 < wbgt && wbgt <= 25) {
        display.print("熱中症:注意　　");
        displayImage("img/WBGT2.png", 350, 125);
    }
    else if (26 <= wbgt && wbgt <= 28) {
        display.print("熱中症:警戒　　");
        displayImage("img/WBGT3.png", 350, 125);
    }
    else if (29 <= wbgt && wbgt <= 30) {
        display.print("熱中症:厳重警戒");
        displayImage("img/WBGT4.png", 350, 125);
    }
    else if (31 <= wbgt) {
        display.print("熱中症:危険　　");
        displayImage("img/WBGT5.png", 350, 125);
    }

    display.setCursor(5, 262);
    if (pirValue == 0) {
        if (titi == 0) {
            titi = now;
        }
        display.print("感知してません");
        if (now - titi >= 5000) {
            display.setCursor(5, 312);
            display.print("5秒経ちました");
        }
    }
    else {
        titi = 0;
        display.setCursor(5, 312);
        display.print("                 "); // 前の文字を消す
        display.setCursor(5, 262);
        display.print("感知しました　");
    }
    


    //メッセージ
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(txm_btn.contains(touchPoint.x, touchPoint.y)){
            //送るボタン押した
            if(top_prebtn != 1) txm_btn.drawButton(true);
            top_prebtn = 1;
        }
        else if(rxm_btn.contains(touchPoint.x, touchPoint.y)){
            //見るボタン押した
            if(top_prebtn != 2) rxm_btn.drawButton(true);
            top_prebtn = 2;
        }
        //お風呂
        else if(bath_btn.contains(touchPoint.x, touchPoint.y)){
            //お風呂ボタンを押した
            if(top_prebtn != 12) bath_btn.drawButton(true);
                top_prebtn = 12;

        }
    } else {
        //ボタン押してない
        if(top_prebtn == 1){
            //メッセージ送信ページへ
            ClearScreen();
            Page = txm_page;
        } else if(top_prebtn == 2){
            //メッセージ受信ページへ
            ClearScreen();
            Page = rxm_page;
        } else if(top_prebtn == 12){
            //お風呂ページへ
            ClearScreen();
            Page = bath_page;
        }
    }
    umes = getUnreadMesNum();
    if(umes != pre_umes){
        display.setCursor(Disw-250,680);
        if(umes == 0)   display.print("未読：なし");
        else            display.print("未読：あり");             
        pre_umes = umes;
    }
}

//メインページクリア
void ClearScreen() {
    delay(500); //描画途中の処理が終わるのを待つ
    display.fillRect(0,100,Disw,Dish-100,TFT_WHITE); 
}
