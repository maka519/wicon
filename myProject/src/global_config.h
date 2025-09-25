//グローバル設定用ヘッダーファイル
//プロジェクト全体で共有する以下の内容を定義する
//パラメータ(#define)
//グローバル変数（型宣言)
//グローバル関数（プロトタイプ宣言）
//実際のグローバル変数、関数の定義は各cppファイル内で行い、このファイル内ではその宣言と、どのcppファイルに定義がされているかをコメントで書くこと。
//このファイルを全てのcppファイルにインクルードしておけば、各ファイル内でグローバル変数、関数、パラメータが使用可能となる。

//＊＊＊＊パラメータ定義＊＊＊＊
//LoRa関係
//環境に合わせて編集すること
#define payloadSize 60      //データペイロードサイズ
#define maxUsers    100     //ユーザ最大数
#define maxMessages 100     //メッセージ最大数
#define maxCategories 100   //カテゴリー最大数
//受信バッファ関係
#define max_rcv 5     //メッセージ保存件数
//ページID
#define top_page    1   //トップページ
#define SOS_page    2   //SOSページ
#define txSOS_page    3   //SOS送信ページ
#define rxm_page   4   //受信メッセージ表示ページ
#define txm_page    5   //メッセージ送信ページ
#define axm_page    6   //体調メッセージ表示ページ
#define bxm_page    7   //会話メッセージ表示ページ
#define cxm_page    8   //入浴メッセージ表示ページ
#define dxm_page    9   //仕事メッセージ表示ページ
#define exm_page    10   //その他メッセージ表示ページ
#define rxm_page_res1    11   //受信メッセージの返信（お試し）

//＊＊＊＊構造体定義＊＊＊＊
//受信メッセージバッファ
struct RcvMesBuff {
    String  from_user_id;   //送信元ユーザID
    String    from_seq;       //送信元シーケンス番号
    String  rep_user_id;    //返信先ユーザID
    String   rep_seq;        //返信先シーケンス番号
    int     con_id_1;       //メッセージID
    int     con_id_2;       //(いまのところ)未使用
    int     to_num;         //送信先ユーザ数
    String  to_user_id[4];  //送信先ユーザIDリスト
    m5::rtc_datetime_t  rcv_datetime;   //受信時間（RTC）
    unsigned long   rcv_millis; //受信時間（システム時間）
    bool     is_read;        //未読：false　既読：true
};


//＊＊＊＊グローバル変数宣言＊＊＊＊
//main.cpp
extern int Page;    //現在表示しているページIDs


//utility.cpp
extern String PANID;    //通信するネットワークのPANID
extern String OWNID;    //自身のOWNID
extern String USERS[];   //ユーザID(OWNID),ユーザ名　の文字列を要素とした配列で定義すする
extern String MESSAGES[];    //メッセージID,メッセージ,カテゴリID　の文字列を要素とした配列で定義する
extern String CATEGORIES[]; //カテゴリーID,カテゴリー名 の文字列を要素とした配列を定義する

//comm.cpp
extern char  LoRaRx[payloadSize+12], LoRaTx[payloadSize+12];   //LoRa通信データ格納用    
extern char *recdata;  //LoRa受信ペイロード先頭ポインタ
extern int rcv_type; //受信データタイプ 0:マルチホップデータ、1:時刻データ（マルチホップ含む）、2:その他
extern int maxRssi, minRssi;   
extern String RcvMess, From, To; //受信メッセージ格納用
extern RcvMesBuff  mesBuff[max_rcv];  //受信メッセージバッファ　最新max_rcv件まで保存
extern int         num_rcv;  //受信メッセージ数
extern int         mbw_ptr;  //受信メッセージ書き込みポインタ
extern int         frt;      //時間データ受信頻度

//top_page.cpp
extern int32_t Disw;    //スクリーン幅
extern int32_t Dish;    //スクリーン高さ
extern M5GFX display;
extern LGFX_Button txm_btn;
extern LGFX_Button rxm_btn;

//SOS_page.cpp
extern unsigned long SOScnt;     //SOSボタン押下カウンタ
extern LGFX_Button sos_btn;
extern LGFX_Button sos_cancel_btn;
extern LGFX_Button sos_done_btn;

//txm_page.cpp
extern LGFX_Button txa;
extern LGFX_Button txb;
extern LGFX_Button txc;
extern LGFX_Button txd;
extern LGFX_Button txe;
extern LGFX_Button txexec_btn;
extern LGFX_Button txcancel_btn;

//axm_page.cpp
extern LGFX_Button axa;
extern LGFX_Button axb;
extern LGFX_Button axc;
extern LGFX_Button axexec_btn;
extern LGFX_Button axcancel_btn;

//bxm_page.cpp
extern LGFX_Button bxa;
extern LGFX_Button bxb;
extern LGFX_Button bxc;
extern LGFX_Button bxexec_btn;
extern LGFX_Button bxcancel_btn;

//cxm_page.cpp
extern LGFX_Button cxa;
//extern LGFX_Button cxb;
//extern LGFX_Button cxc;
extern LGFX_Button cxexec_btn;
extern LGFX_Button cxcancel_btn;

extern LGFX_Button rx_res_1;
extern LGFX_Button rx_res_2;
extern LGFX_Button rx_res_3;
extern LGFX_Button rx_res_4;
extern LGFX_Button rx_rescancel_btn;
//dxm_page.cpp
extern LGFX_Button dxa;
extern LGFX_Button dxb;
extern LGFX_Button dxc;
extern LGFX_Button dxd;
extern LGFX_Button dxexec_btn;
extern LGFX_Button dxcancel_btn;

//exm_page.cpp
extern LGFX_Button exa;
extern LGFX_Button exb;
extern LGFX_Button exc;
extern LGFX_Button exd;
extern LGFX_Button exe;
extern LGFX_Button exexec_btn;
extern LGFX_Button excancel_btn;

//rxm_page.cpp
extern LGFX_Button rxrep_btn;
extern LGFX_Button rxnext_btn;
extern LGFX_Button rxprev_btn;
extern LGFX_Button rxcancel_btn;

//rxm_page_res1.cpp
extern LGFX_Button rxm_res1;
extern LGFX_Button rxm_res2;
extern LGFX_Button rxm_res3;
extern LGFX_Button rxm_res4;
extern LGFX_Button rxm_res_cancel_btn;

//＊＊＊＊グローバル関数宣言＊＊＊＊
//comm.cpp
extern void InitLoRa();
extern char* RxLoRa();
extern void TxLoRa();
extern void SendMessage(String *toid, int tonum, int mesid, int mesid2, String rpyid, int rpyseq);
extern bool ReceiveMessage();
extern void CheckRxLoRa();

//utility.cpp
extern void InitData();
extern String getUserName(String userid);
extern int getMessage(int messageid, String *mstr);
extern String getCategory(int categoryid);
extern void SetMessageFont();
extern void InitRTC();
extern void SetRTC(m5::rtc_datetime_t td);
extern void DispTime();
extern void DispSelTime(m5::rtc_datetime_t stime);
extern void CheckBattery(bool IsForcedDisplay);
extern void DisplayFrt(int frt);
extern m5::rtc_datetime_t UTC2JST(m5::rtc_datetime_t utc_time);

//SD.cpp
extern void InitSD();
extern int ReadSDData(String *array, String filename, int upper);
extern bool SetConfigFromSD();
extern void displayImage(String imgfn, int x,  int y);

//top_page.cpp
extern void InitScreen();
extern void DrawHeader();
extern void DrawTopScreen();
extern void DrawTopScreen2();
extern void ClearScreen();

//SOS_page.cpp
extern void DrawSOSScreen();
extern void DrawTxSOSScreen();
extern void DrawSOSScreen2();
extern void DrawTxSOSScreen2();
extern bool SOS();

//rxm_page.cpp
extern void DrawRxmScreen();
extern void DrawRxmScreen2();
extern void dispRcvMes(int idx);
extern int getUnreadMesNum();

//txm_page.cpp
extern void DrawTxmScreen();
extern void DrawTxmScreen2();

//axm_page.cpp
extern void DrawAxmScreen();
extern void DrawAxmScreen2();

//bxm_page.cpp
extern void DrawBxmScreen();
extern void DrawBxmScreen2();

//cxm_page.cpp
extern void DrawCxmScreen();
extern void DrawCxmScreen2();

//dxm_page.cpp
extern void DrawDxmScreen();
extern void DrawDxmScreen2();

//exm_page.cpp
extern void DrawExmScreen();
extern void DrawExmScreen2();

//rxm_page_res1.cpp
extern void Drawrxm_res1Screen();
extern void Drawrxm_res1Screen2();