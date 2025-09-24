//トップページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

int32_t Disw;    //スクリーン幅(M5paper: 540)
int32_t Dish;    //スクリーン高さ(M5paper: 960)
M5GFX display;

LGFX_Button txm_btn;
LGFX_Button rxm_btn;

int top_prebtn = -1;
int pre_umes;


//画面初期化
//各ページのボタンの初期化もここで行う
void InitScreen() {
    display.begin();
    
    if(display.isEPD())
    {
        display.setColorDepth(8);//電子ペーパー製品は最大8ビットのビット深度をサポートします。
        display.setEpdMode(epd_fastest);
    }
    else
    {
        display.setColorDepth(16);
    }

    display.clear(TFT_WHITE);
    delay(1000);
    Disw = display.width();
    Dish = display.height();

    //各ページのボタン初期化
    //ヘッダー
    sos_btn.initButton(&display, 5+Disw/4, 50, Disw/2, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "SOS", 2, 2);
    //topページ
    txm_btn.initButton(&display, Disw/2, 525, Disw-20, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "送る", 2, 2);
    rxm_btn.initButton(&display, Disw/2, 625, Disw-20, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "見る", 2, 2);

    //SOSページ
    sos_cancel_btn.initButton(&display, 5+Disw/4, 400, Disw/3, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "間違い", 2, 2);
    sos_done_btn.initButton(&display, Disw-(5+Disw/4), 400, Disw/3, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "発信終", 2, 2);

    //内容ページ
    txa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "体調", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    txb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "会話", 2, 2);
    txc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "入浴", 2, 2);
    txd.initButton(&display, Disw/2, 550, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "仕事", 2, 2);
    txe.initButton(&display, Disw/2, 650, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "その他", 2, 2);
    txcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);
    txexec_btn.initButton(&display, Disw-(Disw/4), 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "送る", 2, 2);

    //体調ページ
    axa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "体調が悪い", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    axb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "体が痛い", 2, 2);
    axc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "助けてほしい", 2, 2);
    axcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);
    axexec_btn.initButton(&display, Disw-(Disw/4), 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "送る", 2, 2);

    //会話ページ
    bxa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "元気ですか", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    bxb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "一緒に通院したい", 2, 2);
    bxc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "買い物に行きたい", 2, 2);
    bxcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);
    bxexec_btn.initButton(&display, Disw-(Disw/4), 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "送る", 2, 2);

    //入浴ページ
    cxa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "お風呂に入ります", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    //cxb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "会話", 2, 2);
    //cxc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "入浴", 2, 2);
    cxcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);
    cxexec_btn.initButton(&display, Disw-(Disw/4), 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "送る", 2, 2);

    //仕事ページ
    dxa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "楽しい", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    dxb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "つらい", 2, 2);
    dxc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "相談したい", 2, 2);
    dxd.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "悩みがある", 2, 2);
    dxcancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);
    dxexec_btn.initButton(&display, Disw-(Disw/4), 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "送る", 2, 2);

    //その他ページ
    exa.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "なくしもの", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    exb.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "ひま", 2, 2);
    exc.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "病院に行きたい", 2, 2);
    exd.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "買い物に行きたい", 2, 2);
    exe.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "助けてほしい", 2, 2);
    excancel_btn.initButton(&display, Disw/4, 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);
    exexec_btn.initButton(&display, Disw-(Disw/4), 800, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "送る", 2, 2);

    //受信メッセージ表示ページ
    rxcancel_btn.initButton(&display, Disw/4, 900, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);
    rxrep_btn.initButton(&display, Disw-(Disw/4), 900, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "返信", 2, 2);
    rxprev_btn.initButton(&display, Disw/4, 750, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "前へ", 2, 2);
    rxnext_btn.initButton(&display, Disw-(Disw/4), 750, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "次へ", 2, 2);

    //受信メッセージ返信ページ
    rxm_res1.initButton(&display, Disw/2, 250, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "わかりました", 2, 2);//(図形の場所x軸、y軸、　図形の形横幅、　縦幅)
    rxm_res2.initButton(&display, Disw/2, 350, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "返信１", 2, 2);
    rxm_res3.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "返信２", 2, 2);
    rxm_res4.initButton(&display, Disw/2, 450, Disw, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "返信３", 2, 2);
    rxm_res_cancel_btn.initButton(&display, Disw/4, 900, Disw/5, 90, TFT_BLACK, 0xeeee, TFT_BLACK, "戻る", 2, 2);

    /*
    //テスト
    display.setCursor(10, 40);
    display.println("Screen Width: "+String(Disw));
    display.println("Screen Height: "+String(Dish));
    display.println("M5.Lcd.getCursorY: "+String(M5.Lcd.getCursorY()));
    display.println("M5.Lcd.getCursorY: "+String(M5.Lcd.getCursorY()));
    */
}

//ヘッダー描画
void DrawHeader() {
    display.fillRect(0,0,Disw,100,TFT_WHITE);
    delay(300);
    display.drawFastHLine( 0, 99, Disw, TFT_BLACK); //エリア境界に水平線を引く
    sos_btn.drawButton();
    DisplayFrt(frt);
    CheckBattery(true);
}

//トップページ描画
void DrawTopScreen() {
    //トップ画面初期化
    //レイアウト
    //フレーム描画1
    //環境情報
    display.fillRect(3,110,Disw-6,300,TFT_BLACK);
    //メッセージ
    display.fillRect(3,420,Disw-6,350,TFT_BLACK);
    delay(200);
    //フレーム描画2
    //環境情報
    display.fillRect(5,112,Disw-10,295,TFT_WHITE);
    //メッセージ
    display.fillRect(5,422,Disw-10,345,TFT_WHITE);
    delay(200);

    //テキスト
    //環境情報
    display.setCursor(5,112);
    display.print("TODO:このエリアに環境情報を表示する");
    //メッセージ
    display.setCursor(5,422);
    display.print("メッセージ");
    display.setCursor(Disw-250,680);
    display.print("未読：　　");   
    
    //ボタン
    txm_btn.drawButton();
    rxm_btn.drawButton();

    //変数初期化
    top_prebtn = -1;
    pre_umes = -1;

    //動作確認用（本番は消す）
    display.setTextSize(1);
    display.setCursor(3,800);
    display.println("動作確認用");
    display.println("ユーザ名取得："+getUserName("ca02"));
    String mes;
    int cid;
    cid = getMessage(2, &mes);
    display.println("メッセージ取得："+mes);
    display.println("カテゴリー取得："+getCategory(cid));
    display.setTextSize(2);
}

void DrawTopScreen2() {
    //トップ画面処理
    m5::touch_point_t touchPoint;
    int umes;

    //環境情報
    //TODO:環境情報を取得し、表示を更新する

    //メッセージ
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        if(txm_btn.contains(touchPoint.x, touchPoint.y)){
            //送るボタン押した
            if(top_prebtn != 1) txm_btn.drawButton(true);
            top_prebtn = 1;
        }
        else if(rxm_btn.contains(touchPoint.x, touchPoint.y)){
            //見るボタン押した
            if(top_prebtn != 2) rxm_btn.drawButton(true);
            top_prebtn = 2;
        }
    } else {
        //ボタン押してない
        if(top_prebtn == 1){
            //メッセージ送信ページへ
            ClearScreen();
            Page = txm_page;
        } else if(top_prebtn == 2){
            //メッセージ受信ページへ
            ClearScreen();
            Page = rxm_page;
        }
    }
    umes = getUnreadMesNum();
    if(umes != pre_umes){
        display.setCursor(Disw-250,680);
        if(umes == 0)   display.print("未読：なし");
        else            display.print("未読：あり");             
        pre_umes = umes;
    }
}

//メインページクリア
void ClearScreen() {
    delay(500); //描画途中の処理が終わるのを待つ
    display.fillRect(0,100,Disw,Dish-100,TFT_WHITE); 
}
