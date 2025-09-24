//入浴ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button cxa;
//LGFX_Button cxb;
//LGFX_Button cxc;
LGFX_Button cxexec_btn;
LGFX_Button cxcancel_btn;

int cx_prebtn = -1;

//体調ページ描画
void DrawCxmScreen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("入浴");
   
    //ボタン
    cxa.drawButton();
    //cxb.drawButton();
    //cxc.drawButton();
    cxcancel_btn.drawButton();
    cxexec_btn.drawButton();

    //変数初期化
    cx_prebtn = -1;
}

void DrawCxmScreen2() {
    //ページ処理
    m5::touch_point_t touchPoint;

    //送信処理
    //TODO:送信設定を行うUIを表示

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(cxcancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(cx_prebtn != 1) cxcancel_btn.drawButton(true);
            cx_prebtn = 1;
        }
        else if(cxexec_btn.contains(touchPoint.x, touchPoint.y)){
            //送るボタン押した
            if(cx_prebtn != 2) cxexec_btn.drawButton(true);
            cx_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(cx_prebtn == 1){
            //内容ページへ
            ClearScreen();
            Page = txm_page;
        } else if(cx_prebtn == 2){
            //Todo：送信実行
            
            //トップページへ
            ClearScreen();
            Page = top_page;
        }
    }

}