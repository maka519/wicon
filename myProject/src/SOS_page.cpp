//SOSページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

unsigned long SOScnt = 0;     //SOSボタン押下カウンタ
LGFX_Button sos_btn;
LGFX_Button sos_cancel_btn;
LGFX_Button sos_done_btn;

int dflg = 1;
int pcnt = 0;
int SOSmode = -1;
int pre_SOSmode;

//SOSボタン押下ページ描画
void DrawSOSScreen() {
    //ページ初期化
    display.setTextSize(4);
    display.setCursor(25, 200);
    display.println("ＳＯＳ発信");
    display.setTextSize(2);
    display.setCursor(1, 300);
    display.println("５秒間ＳＯＳボタンを押し続けて下さい");
    display.setTextSize(4);
    display.setCursor(100, 400);
    display.print("0");
    display.setCursor(200, 400);
    display.print("秒");
    display.setTextSize(2);
}
void DrawSOSScreen2() {
    if((millis()-SOScnt)>1000){
        //経過時間のみ更新
        pcnt++;
        display.setTextSize(4);
        display.setCursor(100, 400);
        display.print(String(pcnt));
        display.setTextSize(2);
        SOScnt = millis();
    }
}

//SOS送信ページ描画
void DrawTxSOSScreen() {
    //画面初期化
    display.setTextSize(4);
    display.setCursor(25, 200);
    display.println("ＳＯＳ発信");
    display.setTextSize(2);
    display.setCursor(1, 300);
    display.println("ＳＯＳ信号発信中．．．");
    sos_cancel_btn.drawButton();
    sos_done_btn.drawButton();
    displayImage("img/SOS1.png", Disw/2-300,  600);
    pre_SOSmode = -1;
}
void DrawTxSOSScreen2() {
    //SOS送信画面処理
    if(SOSmode == 1){
    //キャンセル
        display.setTextSize(4);
        display.setCursor(25, 200);
        display.println("ＳＯＳ中止");
        display.setTextSize(2);
        display.setCursor(1, 300);
        display.println("発信キャンセルしました");
    }
    else if(SOSmode == 2){
        //発信終了
        display.setTextSize(4);
        display.setCursor(25, 200);
        display.println("ＳＯＳ停止");
        display.setTextSize(2);
        display.setCursor(1, 300);
        display.println("　発信を停止しました　");
    } else {
        //発信中
        if((millis()-SOScnt) > 1000) {
            //if(dflg == 1)   display.setTextColor(TFT_BLACK, TFT_WHITE);
            //else            display.setTextColor(TFT_WHITE, TFT_BLACK);
            //delay(200);
            dflg = -dflg;
            SOScnt = millis();
            display.setTextSize(4);
            display.setCursor(25, 200);
            display.println("ＳＯＳ発信");
            display.setTextSize(2);
            display.setCursor(1, 300);
            display.println("ＳＯＳ信号発信中．．．");
        }        
    }
}

//SOS処理
bool SOS() {
    m5::touch_point_t touchPoint;

    if(Page == txSOS_page){
        //SOS発信中
        if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
            if(sos_cancel_btn.contains(touchPoint.x, touchPoint.y)){
                //タッチ座標がSOSキャンセルボタン内
                if(SOSmode != 1){
                    sos_cancel_btn.drawButton(true);
                }
                SOSmode = 1;
                return true;            
            }
            else if(sos_done_btn.contains(touchPoint.x, touchPoint.y)){
                //タッチ座標がSOS終了ボタン内
                if(SOSmode != 2){
                    sos_done_btn.drawButton(true);
                }
                SOSmode = 2;
                return true;            
            }
        }
        else{
            //タッチしてない
            if(SOSmode == 1){
                //SOSキャンセル
                //todo SOSキャンセルメッセージをブロードキャストで送信

                //トップページに戻る
                SOScnt = 0;
                DrawHeader();
                ClearScreen();
                Page = top_page;
            } else if(SOSmode == 2){
                //SOS終了
                //todo SOS終了メッセージをブロードキャストで送信

                //トップページに戻る
                SOScnt = 0;
                DrawHeader();
                ClearScreen();
                Page = top_page;
            } else {
                //SOS発信中
                //todo SOS信号メッセージをブロードキャストで送信（5分周期）

            }
            display.setTextColor(TFT_BLACK, TFT_WHITE);
            return false;         
        }
            
    }
    else {
        //SOSボタンチェック
        if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
            //タッチ座標取得
            if(sos_btn.contains(touchPoint.x, touchPoint.y)){
                //タッチ座標がSOSボタン内
                if(SOScnt == 0){
                    //SOS押下開始
                    SOScnt = millis();
                    sos_btn.drawButton(true);
                    ClearScreen();
                    Page = SOS_page;
                    pcnt = 0;
                }
                else if(pcnt >= 5){
                    //5秒以上押されたのでSOS処理実施
                    ClearScreen();
                    Page = txSOS_page;
                    SOSmode = -1;
                    SOScnt = millis();
                }
                else{
                    //SOS押下中

                }; 
                return true;            
            }
        }
        else{
            //タッチしてない
            if(SOScnt != 0){
                SOScnt = 0;
                DrawHeader();
                //トップ画面に戻る
                ClearScreen();
                Page = top_page;
            }
            return false;         
        }
    }
    return false;
}