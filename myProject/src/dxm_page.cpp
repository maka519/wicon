//仕事ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button dxa;
LGFX_Button dxb;
LGFX_Button dxc;
LGFX_Button dxd;
LGFX_Button dxexec_btn;
LGFX_Button dxcancel_btn;

int dx_prebtn = -1;

//体調ページ描画
void DrawDxmScreen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("仕事");
    
    //ボタン
    dxa.drawButton();
    dxb.drawButton();
    dxc.drawButton();
    dxd.drawButton();
    dxcancel_btn.drawButton();
    dxexec_btn.drawButton();

    //変数初期化
    dx_prebtn = -1;
}

void DrawDxmScreen2() {
    //ページ処理
    m5::touch_point_t touchPoint;

    //送信処理
    //TODO:送信設定を行うUIを表示

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(dxcancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(dx_prebtn != 1) dxcancel_btn.drawButton(true);
            dx_prebtn = 1;
        }
        else if(dxexec_btn.contains(touchPoint.x, touchPoint.y)){
            //送るボタン押した
            if(dx_prebtn != 2) dxexec_btn.drawButton(true);
            dx_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(dx_prebtn == 1){
            //内容ページへ
            ClearScreen();
            Page = txm_page;
        } else if(dx_prebtn == 2){
            //Todo：送信実行
            
            //トップページへ
            ClearScreen();
            Page = top_page;
        }
    }

}