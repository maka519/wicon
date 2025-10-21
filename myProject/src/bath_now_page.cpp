//お風呂中ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button finish_bath;

int bath_now_prebtn;

void Drawbath_nowScreen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("お風呂に入っています");

    display.setCursor(Disw/10, 500);
    display.print("お風呂から上がった");

    //ボタン
    finish_bath.drawButton();

    //変数初期化
    bath_now_prebtn = -1;
}

void Drawbath_nowScreen2(){
    //ページ処理
    m5::touch_point_t touchPoint;

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(finish_bath.contains(touchPoint.x, touchPoint.y)){
            //お風呂上がったボタン押した
            if(bath_now_prebtn != 1) bath_cancel_btn.drawButton(true);
            bath_now_prebtn = 1;
        }
    } else {
        //ボタン押してない
        if(bath_now_prebtn == 1){
            //トップページへ
            ClearScreen();
            Page = top_page;
        }
        
    }

}