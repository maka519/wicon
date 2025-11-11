//体調確認ページ
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Button very_good;//とてもいい
LGFX_Button good;//いい
LGFX_Button normal;//普通
LGFX_Button sick;//悪い
LGFX_Button very_sick;//とても悪い
//LGFX_Button conditioncancel_btn;//戻る

int condition_prebtn = -1;

//内容ページ描画
void Drawcheck_conditionScreen() {
    //テキスト    
    display.setCursor(3, 150);
    display.print("今の気分は？");
    
    //ボタン
    very_good.drawButton();
    good.drawButton();
    normal.drawButton();
    sick.drawButton();
    very_sick.drawButton();
    //conditioncancel_btn.drawButton();//戻る

    //変数初期化
    condition_prebtn = -1;
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
        }
    }

} 