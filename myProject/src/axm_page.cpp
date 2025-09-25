//体調ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button axa;
LGFX_Button axb;
LGFX_Button axc;
LGFX_Button axexec_btn;//戻る
LGFX_Button axcancel_btn;//送る

int ax_prebtn = -1;

//体調ページ描画
void DrawAxmScreen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("体調");

    //ボタン
    axa.drawButton();
    axb.drawButton();
    axc.drawButton();
    axcancel_btn.drawButton();
    axexec_btn.drawButton();

    //変数初期化
    ax_prebtn = -1;
}

void DrawAxmScreen2() {
    //ページ処理
    m5::touch_point_t touchPoint;

    //送信処理
    //TODO:送信設定を行うUIを表示

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(axcancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(ax_prebtn != 1) axcancel_btn.drawButton(true);
            ax_prebtn = 1;
        }
        else if(axexec_btn.contains(touchPoint.x, touchPoint.y)){
            //送るボタン押した
            if(ax_prebtn != 2) axexec_btn.drawButton(true);
            ax_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(ax_prebtn == 1){
            //内容ページへ
            ClearScreen();
            Page = txm_page;
        } else if(ax_prebtn == 2){
            //Todo：送信実行
            
            //トップページへ
            ClearScreen();
            Page = top_page;
        }
    }

}

/*void DrawTopScreen3() {
    //内容画面処理
    m5::touch_point_t touchPoint;
    int umes;

    //環境情報
    //TODO:環境情報を取得し、表示を更新する

    //メッセージ
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(txm_btn.contains(touchPoint.x, touchPoint.y)){
            //送るボタン押した
            if(ax_prebtn != 1) axcancel_btn.drawButton(true);
            ax_prebtn = 1;
        }
        else if(rxm_btn.contains(touchPoint.x, touchPoint.y)){
            //見るボタン押した
            if(ax_prebtn != 2) rxm_btn.drawButton(true);
            ax_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(ax_prebtn == 1){
            //メッセージ送信ページへ
            ClearScreen();
            Page = txm_page;
        } else if(ax_prebtn == 2){
            //メッセージ受信ページへ
            ClearScreen();
            Page = rxm_page;
        }
    }
    umes = getUnreadMesNum();
    
}*/