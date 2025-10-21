//お風呂ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button take_a_bath;
LGFX_Button bath_cancel_btn;

int bath_prebtn;

void DrawbathScreen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("お風呂に入りますか？");

    //ボタン
    take_a_bath.drawButton();
    bath_cancel_btn.drawButton();

    //変数初期化
    bath_prebtn = -1;
}

void DrawbathScreen2(){
    //ページ処理
    m5::touch_point_t touchPoint;

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(bath_cancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(bath_prebtn != 1) bath_cancel_btn.drawButton(true);
            bath_prebtn = 1;
        }
        else if(take_a_bath.contains(touchPoint.x, touchPoint.y)){
            //お風呂に入るボタン押した
            if(bath_prebtn != 2) take_a_bath.drawButton(true);
            bath_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(bath_prebtn == 1){
            //トップページへ
            ClearScreen();
            Page = top_page;
        } else if(bath_prebtn == 2){
            //お風呂中ページへ
            ClearScreen();
            Page = bath_now_page;
        }
        
    }

}