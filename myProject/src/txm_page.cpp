//メッセージ送信ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button txexec_btn;
LGFX_Button txcancel_btn;

int tx_prebtn = -1;

//メッセージ送信ページ描画
void DrawTxmScreen() {
    //ページ初期化

    //ページレイアウト
    //フレーム描画1
    //送信設定
    display.fillRect(3,200,Disw-6,400,TFT_BLACK);
    delay(300);
    //フレーム描画2
    //送信設定
    display.fillRect(5,202,Disw-6,395,TFT_WHITE);
    delay(300);

    //テキスト    
    display.setCursor(3, 150);
    display.print("メッセージ送信");
    display.setCursor(5, 202);
    display.println("TODO: ここに送信処理を表示する");

    //ボタン
    txcancel_btn.drawButton();
    txexec_btn.drawButton();

    //変数初期化
    tx_prebtn = -1;
}

void DrawTxmScreen2() {
    //ページ処理
    m5::touch_point_t touchPoint;

    //送信処理
    //TODO:送信設定を行うUIを表示

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(txcancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(tx_prebtn != 1) txcancel_btn.drawButton(true);
            tx_prebtn = 1;
        }
        else if(txexec_btn.contains(touchPoint.x, touchPoint.y)){
            //送るボタン押した
            if(tx_prebtn != 2) txexec_btn.drawButton(true);
            tx_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(tx_prebtn == 1){
            //トップページへ
            ClearScreen();
            Page = top_page;
        } else if(tx_prebtn == 2){
            //Todo：送信実行
            
            //トップページへ
            ClearScreen();
            Page = top_page;
        }
    }

}