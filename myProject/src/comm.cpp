//シリアル通信
#include <M5Unified.h>
#include <Arduino.h>
#include <stdlib.h>

#include "global_config.h"

char  LoRaRx[payloadSize+12], LoRaTx[payloadSize+12];   //LoRa通信データ格納用    
char *recdata;  //LoRa受信ペイロード先頭ポインタ
int rcv_type; //受信データタイプ 0:マルチホップデータ、1:時刻データ（マルチホップ含む）、2:その他
int maxRssi = 0, minRssi = 0;
String RcvMess, From, To; //受信メッセージ格納用
RcvMesBuff  mesBuff[max_rcv];  //受信メッセージバッファ　最新max_rcv件まで保存
int         num_rcv;  //受信メッセージ数
int         mbw_ptr;  //受信メッセージ書き込みポインタ
int         frt;      //時間データ受信頻度

int seq = 0x2d, hop =0x2d, rflg = 0;

unsigned long   time_set = -1, frt_cnt;


//LoRa通信初期化
void InitLoRa() {
  num_rcv = 0;  //受信メッセージ０
  mbw_ptr = 0;  //書き込みポインタを０に設定
  frt = 0;  //時間データ受信頻度
  frt_cnt = millis();
}

//LoRa通信受信処理
//戻り null:受信データなし　null以外：受信データあり
//受信データありの場合
//LoRaRx: 受信データ全て（ヘッダー込み）が入っている
//rcv_typeは、受信データの種類で変数定義のコメント説明のように設定される
char* RxLoRa() {
  int LoRaRxIndex, wait_cnt, loop_flg, offset, i;
  uint8_t data;
  unsigned long tmpt;

  //display.setCursor(10, 50);
  //display.printf("Check LoRa");
  
  rcv_type = -1;
  recdata = NULL; //データ受信したらデータの先頭ポインタが入る
  if(Serial2.available() != 0){
    recdata = NULL; //データ受信したらデータの先頭ポインタが入る
    rcv_type = 2; //0:マルチホップデータ、1:時刻データ（マルチホップ含む）、2:その他
    LoRaRxIndex = 0;

    loop_flg = 1;
    while(loop_flg == 1) {  
      wait_cnt = 10000;
      loop_flg = 0;
      while(wait_cnt > 0) {
        if(Serial2.available() > 0)  wait_cnt = 0;
        else  wait_cnt--;
      }
      if(Serial2.available() > 0) {
        loop_flg = 1;
          data = (uint8_t)Serial2.read();
          if(LoRaRxIndex < payloadSize+11){
            //受信データサイズを超えるデータは破棄する
            if((data !='\n')&&(data !='\r')&&(data != '\a')) LoRaRx[LoRaRxIndex++] = data;
          }
      }
    }
    LoRaRx[LoRaRxIndex] = '\0';
    #if DEBUG_FLG
      Serial.println(LoRaRx);
    #endif
    //display.setCursor(10, 150);
    //display.print(LoRaRx);
    if(LoRaRxIndex >25){
      //受信データは25バイト以上ある

      //直前の送信処理のACKとつながっていたらその分オフセットを入れる
      offset = 0;
      i = 0;
      while(i == 0) {
        i = 1;
        if((LoRaRx[offset] == 'O')&&(LoRaRx[offset+1] == 'K')){
          offset += 2;
          i = 0;
        }
        else if((LoRaRx[offset] == 'N')&&(LoRaRx[offset+1] == 'G')){
          offset += 6;
          i = 0;
        }
      }
      

      if((LoRaRx[offset+12] == 't')&&
        (LoRaRx[offset+13] == 'i')&&
        (LoRaRx[offset+14] == 'm')&&
        (LoRaRx[offset+15] == 'e')) {
        //ゲートウェイからの時刻
        recdata = &LoRaRx[offset+12];
        rcv_type = 1;
      }
      else if((LoRaRx[offset+14] == 't')&&
        (LoRaRx[offset+15] == 'i')&&
        (LoRaRx[offset+16] == 'm')&&
        (LoRaRx[offset+17] == 'e')) {
        //ゲートウェイからの時刻のマルチホップデータ
        recdata = &LoRaRx[offset+14];
        rcv_type = 1;
      }
      else {
        //時間データ以外
        if((LoRaRx[offset+24] == '7')&&
          (LoRaRx[offset+25] == '0')) {
          //マルチホップデータ
          recdata = &LoRaRx[offset+12];
          rcv_type = 0;
        }
        else {
          //その他データ
          recdata = &LoRaRx[0];          
        }
      }
    }
    else {
      //その他データ
      recdata = &LoRaRx[0];        
    }
  
    /*
    //送信テスト
    if(LoRaRxIndex > 12) {
      String toid[1] = {"5001"};
      SendMessage(toid, 1, 1, 100, "0000", 0x2d);
    }
    */

  }

  //時間データ受信頻度更新
  tmpt = millis();
  if((tmpt-frt_cnt) > 80000 ){
    //前回更新から80秒たったら頻度減少
    frt--;
    if(frt < 0 ) frt = 0;
    DisplayFrt(frt);
    frt_cnt = tmpt;
  } else if(rcv_type == 1){
    //時間データ受信したので頻度増加
    frt++;
    if(frt > 3 ) frt = 3;
    DisplayFrt(frt);
    frt_cnt = tmpt;
  }

  return recdata;
}

//LoRa送信
//LoTa送信を実行する
//事前処理：LoRaTxにヘッダー込みで送信データを入れおく
void TxLoRa() {
    Serial2.print(LoRaTx);
    display.println(LoRaTx);
    rflg = 1;
}

//メッセージ送信
//toid: 宛先(OWNID, ffffはブロードキャスト)配列　
//tonum: 宛先配列要素数
//mesid:   送信メッセージID（送信するメッセージのID)
//mesid2:  送信メッセージID2（メッセージの付加情報があればそのID、なければ0）
//rpyid: 返信先ID（ない場合は0000)
//rpyseq: 返信先シーケンス番号（返信先ない場合は0x2d)
void SendMessage(String *toid, int tonum, int mesid, int mesid2, String rpyid, int rpyseq) {
    int i;
    //送信データ生成
    sprintf(LoRaTx,"%sffff%c%c%sffffb170%1x1%s%c%s%c%02x%02x%1x",PANID,hop,seq,OWNID,rflg,OWNID,seq,rpyid,rpyseq,mesid,mesid2,tonum);
    for(i=0; i<tonum; i++)  sprintf(LoRaTx,"%s%s",LoRaTx,toid[i]);
    sprintf(LoRaTx,"%s\r\n",LoRaTx);
    TxLoRa();
    seq++;
    if(seq >= 0x7d) seq = 0x2d;
}

//メッセージ受信
//事前処理：LoRaRxにマルチホップ受信データが入っていること
//自分宛があった場合、受信バッファ内に同じメッセージがなければ受信メッセージバッファに格納する。その場合、一番古いメッセージに上書きする　
//戻り値： バッファに書き込んだ場合：true　バッファに書き込まなかった場合:false

bool ReceiveMessage(){
  long i, num, deff;
  String tmpstr, tto, tos;
  int oflg;

  String  from_user_id;   //送信元ユーザID
  String  from_seq;       //送信元シーケンス番号
  m5::rtc_datetime_t  rcv_datetime;   //受信時間（RTC）
  unsigned long   rcv_millis; //受信時間（システム時間）
  
  //コミュニケーションノードのメッセージタイプでなければ終了
  if(!((LoRaRx[22] == 'b')&&(LoRaRx[27] == '1'))) return false;

  tmpstr = LoRaRx;
  //自分宛か確認
  num = strtol(tmpstr.substring(42,43).c_str(), NULL, 16);
  tto = "";
  for(i=0; i<num; i++){
    tos = tmpstr.substring(43+4*i,47+4*i);
    tos.toLowerCase();
    if((tos == "ffff" )||(tos == OWNID)) {
      //宛先がブロードキャストか自分なら自分宛
      tto = tos;
      i = num;
    }
  }
  //自分宛でなければ終了
  if(tto == "") return false;

  //自分宛
  from_user_id = tmpstr.substring(28,32);
  from_seq = tmpstr.substring(32,33);
  rcv_datetime = M5.Rtc.getDateTime(); 
  rcv_millis = millis();

  //同じメッセージがバッファにないかチェック
  oflg = 0;
  for(i=0; i<num_rcv; i++){
    if((mesBuff[i].from_user_id == from_user_id)&&(mesBuff[i].from_seq == from_seq)) {
      //同じ送信元ID,シーケンス番号のメッセージがある
      if(rcv_millis - mesBuff[i].rcv_millis < 60*1000){
        //受信時間の差が１分以内なので同じメッセージとみなし破棄
        oflg = 1;
      }
    }
  }
  if(oflg == 0){
    //メッセージをバッファに書き込み
    mesBuff[mbw_ptr].from_user_id = from_user_id;   //送信元ユーザID
    mesBuff[mbw_ptr].from_seq = from_seq;       //送信元シーケンス番号
    mesBuff[mbw_ptr].rep_user_id = tmpstr.substring(33,37);    //返信先ユーザID
    mesBuff[mbw_ptr].rep_seq = tmpstr.substring(37,38);        //返信先シーケンス番号
    sscanf(tmpstr.substring(38,40).c_str(), "%x", &mesBuff[mbw_ptr].con_id_1);  //メッセージID
    sscanf(tmpstr.substring(40,42).c_str(), "%x", &mesBuff[mbw_ptr].con_id_2);  //(いまのところ)未使用
    sscanf(tmpstr.substring(42,43).c_str(), "%x", &mesBuff[mbw_ptr].to_num);   //送信先ユーザ数
    for(i=0; i < mesBuff[mbw_ptr].to_num; i++){
      mesBuff[mbw_ptr].to_user_id[i] = tmpstr.substring(43+4*i,47+4*i);  //送信先ユーザIDリスト
    }    
    mesBuff[mbw_ptr].rcv_datetime = rcv_datetime;   //受信時間（RTC）
    mesBuff[mbw_ptr].rcv_millis = rcv_millis; //受信時間（システム時間）
    mesBuff[mbw_ptr].is_read = false;        //未読：false　既読：true
    mbw_ptr++;
    if(mbw_ptr >= max_rcv)  mbw_ptr = 0;
    num_rcv++;
    if(num_rcv >= max_rcv)  num_rcv = max_rcv;
    return true;
  }
  
  return false;
}

//LoRa受信チェック
void CheckRxLoRa() {
  char *rxData;
  String payload;
  m5::rtc_datetime_t stime;
  unsigned long time_det;

  rxData = RxLoRa();

  /*
  //メッセージ受信テスト
  sprintf(LoRaRx,"ff80bf9affff..ca01ffffb17011ca01.0000-01641ffff");
  rxData = &LoRaRx[12];
  rcv_type = 0;
  */

  if(rxData != NULL) {

    display.setCursor(3,800);

    //受信データあり
    switch(rcv_type){
      case 0: //マルチホップデータ
        //display.println("メッセージ受信");
        //display.println(rxData);
 
        ReceiveMessage();
        break;
      case 1: //時刻データ
        //display.println("時刻");
        //display.println(rxData);

        time_det = millis();
        if((time_set == -1)||((time_det - time_set) >= 1000*60*60*12)){
          //起動時、あるいは12時間以上時間設定していなければ時間更新
          payload = rxData;
          if(payload.substring(20,24).toInt() >= 2025){
            //疑似時間データではない
            stime.date.year = payload.substring(20,24).toInt();
            stime.date.month = payload.substring(5,7).toInt();
            stime.date.date = payload.substring(8,10).toInt();
            stime.time.hours = payload.substring(11,13).toInt();
            stime.time.minutes = payload.substring(14,16).toInt();
            stime.time.seconds = payload.substring(17,19).toInt();
            
            SetRTC(stime);
            time_set = time_det;
          }
        }

        break;
      default:  //その他
        break;
    }
  };
}