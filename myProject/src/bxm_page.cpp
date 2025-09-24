//会話ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button bxa;
LGFX_Button bxb;
LGFX_Button bxc;
LGFX_Button bxexec_btn;
LGFX_Button bxcancel_btn;

int bx_prebtn = -1;

//体調ページ描画
void DrawBxmScreen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("会話");
    
    //ボタン
    bxa.drawButton();
    bxb.drawButton();
    bxc.drawButton();
    bxcancel_btn.drawButton();
    bxexec_btn.drawButton();

    //変数初期化
    bx_prebtn = -1;
}

void DrawBxmScreen2() {
    //ページ処理
    m5::touch_point_t touchPoint;

    //送信処理
    //TODO:送信設定を行うUIを表示

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(bxcancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(bx_prebtn != 1) bxcancel_btn.drawButton(true);
            bx_prebtn = 1;
        }
        else if(bxexec_btn.contains(touchPoint.x, touchPoint.y)){
            //送るボタン押した
            if(bx_prebtn != 2) bxexec_btn.drawButton(true);
            bx_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(bx_prebtn == 1){
            //内容ページへ
            ClearScreen();
            Page = txm_page;
        } else if(bx_prebtn == 2){
            //Todo：送信実行
            
            //トップページへ
            ClearScreen();
            Page = top_page;
        }
    }

}