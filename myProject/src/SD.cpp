//SDカード読み書き
//シリアル通信
#include <SD.h>
#include <M5Unified.h>
#include <Arduino.h>
#include <M5GFX.h>

#include "global_config.h"

LGFX_Sprite sprite;

//SDカード初期設定
//SDカードのマウント
void InitSD(){
    // SDカードマウント待ち
    while (false == SD.begin(GPIO_NUM_4, SPI, 15000000)) {
        Serial.println("SD Wait...");
        delay(500);
    }
}

//データ読み込み
//array: 読み込みデータを格納するString配列
//filename: 読み込むファイル名
//upper: 配列最大数
//戻り値：読み込んだデータ数（行数）　エラー時は-1
int ReadSDData(String *array, String filename, int upper){
    int i;
    filename = "/"+filename;
    auto fl = SD.open(filename, FILE_READ, false);
    i = 0;
    if(fl){
        while (fl.available()) {
            array[i] = fl.readStringUntil('\n');
            //display.print(array[i]+"\n");
            i++;
            if(i>= upper) break;    //最大になったら抜ける
        }
        //display.print(String(i)+"\n");
        fl.close();
        return i;
    } else {
        display.print(filename +" was not found\n");
        return -1;
    }
}

//設定データ読み込み
//システム動作に必要な設定データを、SDカード内の"CONFIG.txt"から読み込む
bool SetConfigFromSD() {
    String tstr, vname, vval;
    int idx;
    auto fl = SD.open("/CONFIG.txt", FILE_READ, false);
    if(fl){
        while (fl.available()) {
            tstr = fl.readStringUntil('\n'); //変数名,値
            //変数名と値を抽出
            idx = tstr.indexOf(',');
            vname = tstr.substring(0, idx);
            vval = tstr.substring(idx+1);
            vname.trim();
            vval.trim();
            //display.print(vname+"\n");
            //display.print(vval+"\n");

            //対応する変数に値を代入
            if(vname == "PANID")    PANID = vval;
            else if(vname == "OWNID")    OWNID = vval;
        }
        //display.print("PANID: "+PANID+"\n");
        //display.print("OWNID: "+OWNID+"\n");
        fl.close();
        return true;
    } else {
        display.print("CONFIG.txt was not found\n");
        return false;
    }
}

//画像ファイル表示

void displayImage(String imgfn, int x,  int y){
    display.drawPngFile(SD, "/"+imgfn, x, y);
    delay(1000);
}