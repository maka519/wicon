//内容ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button txa;//体調
LGFX_Button txb;//会話
LGFX_Button txc;//入浴
LGFX_Button txd;//仕事
LGFX_Button txe;//その他
LGFX_Button txexec_btn;//送る
LGFX_Button txcancel_btn;//戻る

int tx_prebtn = -1;

//内容ページ描画
void DrawTxmScreen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("内容");
    
    //ボタン
    txa.drawButton();//体調
    txb.drawButton();//会話
    txc.drawButton();//入浴
    txd.drawButton();//仕事
    txe.drawButton();//その他
    txcancel_btn.drawButton();//戻る
    txexec_btn.drawButton();//送る

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
        else if(txa.contains(touchPoint.x, touchPoint.y)){
            //体調ボタン押した
            if(tx_prebtn != 3) txa.drawButton(true);
            tx_prebtn = 3;
        }
        else if(txb.contains(touchPoint.x, touchPoint.y)){
            //会話ボタン押した
            if(tx_prebtn != 4) txb.drawButton(true);
            tx_prebtn = 4;
        }
         else if(txc.contains(touchPoint.x, touchPoint.y)){
            //入浴ボタン押した
            if(tx_prebtn != 5) txc.drawButton(true);
            tx_prebtn = 5;
        }
        else if(txd.contains(touchPoint.x, touchPoint.y)){
            //仕事ボタン押した
            if(tx_prebtn != 6) txd.drawButton(true);
            tx_prebtn = 6;
        }  
        else if(txe.contains(touchPoint.x, touchPoint.y)){
            //その他ボタン押した
            if(tx_prebtn != 7) txe.drawButton(true);
            tx_prebtn = 7;
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
        } else if(tx_prebtn == 3){
            //体調ページへ
            ClearScreen();
            Page = axm_page;
        } else if(tx_prebtn == 4){
            //会話ページへ
            ClearScreen();
            Page = bxm_page;
        } else if(tx_prebtn == 5){
            //入浴ページへ
            ClearScreen();
            Page = cxm_page;
        } else if(tx_prebtn == 6){
            //仕事ページへ
            ClearScreen();
            Page = dxm_page;
        } else if(tx_prebtn == 7){
            //その他ページへ
            ClearScreen();
            Page = exm_page;
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
            if(tx_prebtn != 1) txcancel_btn.drawButton(true);
            tx_prebtn = 1;
        }
        else if(rxm_btn.contains(touchPoint.x, touchPoint.y)){
            //見るボタン押した
            if(tx_prebtn != 2) rxm_btn.drawButton(true);
            tx_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(tx_prebtn == 1){
            //メッセージ送信ページへ
            ClearScreen();
            Page = txm_page;
        } else if(tx_prebtn == 2){
            //メッセージ受信ページへ
            ClearScreen();
            Page = rxm_page;
        } else if(tx_prebtn == 3){
            //体調ページへ
            ClearScreen();
            Page = axm_page;
        } else if(tx_prebtn == 4){
            //会話ページへ
            ClearScreen();
            Page = bxm_page;
        } else if(tx_prebtn == 5){
            //入浴ページへ
            ClearScreen();
            Page = cxm_page;
        } else if(tx_prebtn == 6){
            //仕事ページへ
            ClearScreen();
            Page = dxm_page;
        } else if(tx_prebtn == 7){
            //その他ページへ
            ClearScreen();
            Page = exm_page;
        }
    }
    umes = getUnreadMesNum();
    
}*/
