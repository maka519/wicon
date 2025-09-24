//受信メッセージ表示ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button rxrep_btn;
LGFX_Button rxnext_btn;
LGFX_Button rxprev_btn;
LGFX_Button rxcancel_btn;

int rx_prebtn;
int sel_rcv = 0;

//受信メッセージ表示ページ描画
void DrawRxmScreen() {
    //ページ初期化

    //ページレイアウト
    //フレーム描画1
    //受信メッセージ
    display.fillRect(3,200,Disw-6,600,TFT_BLACK);
    delay(300);
    //フレーム描画2
    //受信メッセージ
    display.fillRect(5,202,Disw-6,595,TFT_WHITE);
    delay(300);

    //テキスト    
    display.setCursor(3, 150);
    display.print("受信メッセージ");
    display.setCursor(5, 202);
    //display.println("TODO: ここにメッセージを表示する");
    if(num_rcv == 0){
        //受信メッセージなし
         display.println("受信メッセージはありません。");
    } else {
        sel_rcv = mbw_ptr - 1;
        if(sel_rcv < 0) sel_rcv = max_rcv-1;
        dispRcvMes(sel_rcv);
    }
    //ボタン
    rxcancel_btn.drawButton();
    rxrep_btn.drawButton();
    rxnext_btn.drawButton();
    rxprev_btn.drawButton();

    //変数初期化
    rx_prebtn = -1;
}

void DrawRxmScreen2() {
    //ページ処理
    m5::touch_point_t touchPoint;

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(rxcancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(rx_prebtn != 1) rxcancel_btn.drawButton(true);
            rx_prebtn = 1;
        }
        else if(rxrep_btn.contains(touchPoint.x, touchPoint.y)){
            //返信ボタン押した
            if(rx_prebtn != 2) rxrep_btn.drawButton(true);
            rx_prebtn = 2;
        }
        else if(rxnext_btn.contains(touchPoint.x, touchPoint.y)){
            //次へボタン押した
            if(rx_prebtn != 3) rxnext_btn.drawButton(true);
            rx_prebtn = 3;
        }
        else if(rxprev_btn.contains(touchPoint.x, touchPoint.y)){
            //前へボタン押した
            if(rx_prebtn != 4) rxprev_btn.drawButton(true);
            rx_prebtn = 4;
        }
    } else {
        //ボタン押してない
        if(rx_prebtn == 1){
            //トップページへ
            ClearScreen();
            Page = top_page;
        } else if(rx_prebtn == 2){
            //返信処理
            ClearScreen();
            Page = rxm_page_res1;
        } else if(rx_prebtn == 3){
            //次へ処理
            sel_rcv++;
            if(sel_rcv >= num_rcv)  sel_rcv = 0;
            display.setCursor(5, 202);
            dispRcvMes(sel_rcv);
            rx_prebtn = -1;
            rxnext_btn.drawButton();
        } else if(rx_prebtn == 4){
            //前へ処理
            sel_rcv--;
            if(sel_rcv < 0)  sel_rcv = num_rcv-1;
            display.setCursor(5, 202);    
            dispRcvMes(sel_rcv);            
            rx_prebtn = -1;
            rxprev_btn.drawButton();
        }
    }

}

//受信メッセージを表示
void dispRcvMes(int idx){
    int i;
    String mes;
    display.fillRect(5,202,Disw-6,495,TFT_WHITE);
    delay(300);  //クリアするまで待つ
    display.println("送信者："+getUserName(mesBuff[idx].from_user_id));
    display.print("宛先：");
    for(i=0; i < mesBuff[idx].to_num; i++){
        display.print(getUserName(mesBuff[idx].to_user_id[i])+" ");
    }
    display.println("");
    display.setTextSize(1);
    display.print("受信日時：");
    DispSelTime(mesBuff[idx].rcv_datetime);
    display.println("");
    display.setTextSize(2);    
    getMessage(mesBuff[idx].con_id_1, &mes);
    display.print("メッセージ："+mes);
}

//未読メッセージ数を取得する
int getUnreadMesNum() {
    int i, num;
    num = 0;
    for(i=0; i<num_rcv; i++){
        if(!mesBuff[i].is_read) num++;
    }
    return num;
}