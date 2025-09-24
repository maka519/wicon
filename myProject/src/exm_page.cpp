//その他ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button exa;
LGFX_Button exb;
LGFX_Button exc;
LGFX_Button exd;
LGFX_Button exe;
LGFX_Button exexec_btn;
LGFX_Button excancel_btn;

int ex_prebtn = -1;

//体調ページ描画
void DrawExmScreen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("その他");
    
    //ボタン
    exa.drawButton();
    exb.drawButton();
    exc.drawButton();
    exd.drawButton();
    exe.drawButton();
    excancel_btn.drawButton();
    exexec_btn.drawButton();

    //変数初期化
    ex_prebtn = -1;
}

void DrawExmScreen2() {
    //ページ処理
    m5::touch_point_t touchPoint;

    //送信処理
    //TODO:送信設定を行うUIを表示

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(excancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(ex_prebtn != 1) excancel_btn.drawButton(true);
            ex_prebtn = 1;
        }
        else if(exexec_btn.contains(touchPoint.x, touchPoint.y)){
            //送るボタン押した
            if(ex_prebtn != 2) exexec_btn.drawButton(true);
            ex_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(ex_prebtn == 1){
            //内容ページへ
            ClearScreen();
            Page = txm_page;
        } else if(ex_prebtn == 2){
            //Todo：送信実行
            
            //トップページへ
            ClearScreen();
            Page = top_page;
        }
    }

}