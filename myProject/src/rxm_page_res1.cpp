//受信メッセージに対する返信表示ページ（お試し）
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button rxm_res1;
LGFX_Button rxm_res2;
LGFX_Button rxm_res3;
LGFX_Button rxm_res4;
LGFX_Button rxm_res_cancel_btn;

int rx_res_prebtn;

void Drawrxm_res1Screen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("メッセージ返信");

    //ボタン
    rxm_res1.drawButton();
    rxm_res2.drawButton();
    rxm_res3.drawButton();
    rxm_res4.drawButton();
    rxm_res_cancel_btn.drawButton();

    //変数初期化
    rx_res_prebtn = -1;
}

void Drawrxm_res1Screen2(){
    //ページ処理
    m5::touch_point_t touchPoint;

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(rxm_res_cancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(rx_res_prebtn != 1) rxm_res_cancel_btn.drawButton(true);
            rx_res_prebtn = 1;
        }
        else if(rxm_res1.contains(touchPoint.x, touchPoint.y)){
            //返信１ボタン押した
            if(rx_res_prebtn != 2) rxm_res1.drawButton(true);
            rx_res_prebtn = 2;
        }
        else if(rxm_res2.contains(touchPoint.x, touchPoint.y)){
            //返信２ボタン押した
            if(rx_res_prebtn != 3) rxm_res2.drawButton(true);
            rx_res_prebtn = 3;
        }
        else if(rxm_res3.contains(touchPoint.x, touchPoint.y)){
            //返信３ボタン押した
            if(rx_res_prebtn != 4) rxm_res3.drawButton(true);
            rx_res_prebtn = 4;
        }
        else if(rxm_res4.contains(touchPoint.x, touchPoint.y)){
            //返信４ボタン押した
            if(rx_res_prebtn != 5) rxm_res4.drawButton(true);
            rx_res_prebtn = 5;
        }
    } else {
        //ボタン押してない
        if(rx_res_prebtn == 1){
            //見るページへ
            ClearScreen();
            Page = rxm_page;
        } else if(rx_res_prebtn == 2){
            //トップページへ
            ClearScreen();
            Page = top_page;
        } else if(rx_res_prebtn == 3){
            //トップページへ
            ClearScreen();
            Page = top_page;
        } else if(rx_res_prebtn == 4){
            //トップページへ
            ClearScreen();
            Page = top_page;
        } else if(rx_res_prebtn == 5){
            //トップページへ
            ClearScreen();
            Page = top_page;
        }
        
    }

}