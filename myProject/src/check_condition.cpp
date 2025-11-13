//体調確認ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button very_good;//
LGFX_Button good;//
LGFX_Button normal;//
LGFX_Button sick;//
LGFX_Button very_sick;//
LGFX_Button dizzy;
LGFX_Button ather;
//LGFX_Button conditioncancel_btn;//戻る

int condition_prebtn = -1;

//内容ページ描画
void Drawcheck_conditionScreen() {

    ClearScreen();
    display.display();
    delay(100);
    //テキスト    
    display.setCursor(3, 150);
    display.print("今の体調はどうですか？");
    
    //ボタン
    very_good.drawButton();
    good.drawButton();
    normal.drawButton();
    sick.drawButton();
    very_sick.drawButton();
    dizzy.drawButton();
    ather.drawButton();
    //conditioncancel_btn.drawButton();//戻る
    
    const int button_height = 90;
    const int text_y_offset = button_height / 2 - display.fontHeight() / 2; // ボタン中央にY座標を合わせるためのオフセット
    const int center_x = Disw / 2; // ボタンの中心X座標 (画面幅の半分)

    // 1. 元気です (Y=250)
    const char* text1 = "元気です";
    display.setCursor(center_x - display.textWidth(text1) / 2, 250 - text_y_offset);
    display.print(text1);

    // 2. 熱があります (Y=350)
    const char* text2 = "熱があります";
    display.setCursor(center_x - display.textWidth(text2) / 2, 350 - text_y_offset);
    display.print(text2);

    // 3. 頭痛がひどい (Y=450)
    const char* text3 = "頭痛がひどい";
    display.setCursor(center_x - display.textWidth(text3) / 2, 450 - text_y_offset);
    display.print(text3);

    // 4. 腹痛がひどい (Y=550)
    const char* text4 = "腹痛がひどい";
    display.setCursor(center_x - display.textWidth(text4) / 2, 550 - text_y_offset);
    display.print(text4);

    // 5.
    const char* text5 = "吐き気がする";
    display.setCursor(center_x - display.textWidth(text5) / 2, 650 - text_y_offset);
    display.print(text5);

    // 6.
    const char* text6 = "めまいがする";
    display.setCursor(center_x - display.textWidth(text6) / 2, 750 - text_y_offset);
    display.print(text6);

    // 7.
    const char* text7 = "その他体調不良";
    display.setCursor(center_x - display.textWidth(text7) / 2, 850 - text_y_offset);
    display.print(text7);

    //変数初期化
    condition_prebtn = -1;

    display.display();
}

void Drawcheck_conditionScreen2() {
    //ページ処理
    m5::touch_point_t touchPoint;

    //送信処理
    //TODO:送信設定を行うUIを表示

    //ボタン
    if(M5.Lcd.getTouchRaw(&touchPoint, 1)){
        /*if(normalancel_btn.contains(touchPoint.x, touchPoint.y)){
            //戻るボタン押した
            if(condition_prebtn != 1) normalancel_btn.drawButton(true);
            condition_prebtn = 1;
        }*/
        if(very_good.contains(touchPoint.x, touchPoint.y)){
            
            if(condition_prebtn != 1) very_good.drawButton(true);
            condition_prebtn = 1;
        }
        else if(good.contains(touchPoint.x, touchPoint.y)){
            
            if(condition_prebtn != 2) good.drawButton(true);
            condition_prebtn = 2;
        }
         else if(normal.contains(touchPoint.x, touchPoint.y)){
            
            if(condition_prebtn != 3) normal.drawButton(true);
            condition_prebtn = 3;
        }
        else if(sick.contains(touchPoint.x, touchPoint.y)){
            
            if(condition_prebtn != 4) sick.drawButton(true);
            condition_prebtn = 4;
        }  
        else if(very_sick.contains(touchPoint.x, touchPoint.y)){
            
            if(condition_prebtn != 5) very_sick.drawButton(true);
            condition_prebtn = 5;
        }  
        else if(dizzy.contains(touchPoint.x, touchPoint.y)){
            
            if(condition_prebtn != 6) dizzy.drawButton(true);
            condition_prebtn = 6;
        }
        else if(ather.contains(touchPoint.x, touchPoint.y)){
            
            if(condition_prebtn != 7) ather.drawButton(true);
            condition_prebtn = 7;
        }   
    } else {
        //ボタン押してない
        if(condition_prebtn == 1){
            today_condition_check  = true;
            SendCondition(1);
            //トップページへ
            ClearScreen();
            Page = top_page;
        } else if(condition_prebtn == 2){
            today_condition_check  = true;
            SendCondition(2);
            //トップページへ
            ClearScreen();
            Page = top_page;
        } else if(condition_prebtn == 3){
            today_condition_check  = true;
            SendCondition(3);
            ClearScreen();
            Page = top_page;
        } else if(condition_prebtn == 4){
            today_condition_check  = true;
            SendCondition(4);
            ClearScreen();
            Page = top_page;
        } else if(condition_prebtn == 5){
            today_condition_check  = true;
            SendCondition(5);
            ClearScreen();
            Page = top_page;
        } else if(condition_prebtn == 6){
            today_condition_check  = true;
            SendCondition(6);
            ClearScreen();
            Page = top_page;
        } else if(condition_prebtn == 7){
            today_condition_check  = true;
            SendCondition(7);
            ClearScreen();
            Page = top_page;
        }
    }

} 