//最終課題「ブラック・ジャック」
#include <stdio.h>
#include <handy.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define BLACKJUCK 21
#define WINDOWWIDTH 1200
#define WINDOWHIGHT 800
#define CARDSIZE 0.15 //カードの倍率
#define NUMBER 7      //合計で引いても七枚とする（おそらく）
#define MYCARDX 500   //自身のカードのx軸上の初期位置
#define MYCARDY 400   //自身のカードのy軸上の初期位置
#define DCARDX 500    //ディーラのカードのx軸上の初期位置
#define DCARDY 700    //ディーラのカードのy軸上の初期位置
#define CARDDIFFER 50 //カードごとにずらす位相差

//背景
void backgrounddisplay()      //void
{
    int card_ground;
    int card_back;
    card_back = HgImageLoad("img/tranpuura.jpg");
    card_ground = HgImageLoad("img/backcard.jpg");
    HgSetFillColor(HG_DGREEN);
    HgSetColor(HG_BROWN);
    HgSetWidth(50);
    HgCircleFill(WINDOWWIDTH / 2, WINDOWHIGHT * 19 / 20, 600, 1);
    //トランプ背景
    HgImagePut(120, 120, card_ground, 0.45, -0.3);
    //山札
    HgImagePut(1000, 700, card_back, 0.1, 0);
    //説明文
    HgText(1000, 100, "ブラック・ジャック\nヒットの場合は「ｈ」を入力\nスタンドの場合は「ｓ」を入力\n");
    return ;
}

int handdecide(void)
{ //ディーラと自身の手札を乱数で決定
    int hand;
    int i;
    HgSleep(0.5);
    srand((unsigned)time(NULL));
    i = rand() % 5;
    if (i < 4)
    {
        srand((unsigned)time(NULL)); //真の乱数を定義するため
        hand = rand() % 9;
        hand = hand + 2;
    }
    else
    {
        srand((unsigned)time(NULL)); //真の乱数を定義するため
        hand = rand() % 13;
        hand += 1;
    }

    //printf("%d,%d",mhand[i],dhand[i]);          //デバック用
    return hand;
}

int displaycard(int nowhand, int l /*何枚目のカードなのか*/, int dm /*dmが1なら自身 0ならディーラ*/)
{
    int i;    //カードの番号
    int x, y; //カードの表示位置
    //カード情報と宣言
    int dia_card[15];
    dia_card[1] = HgImageLoad("img/dia1.png");
    dia_card[2] = HgImageLoad("img/dia2.png");
    dia_card[3] = HgImageLoad("img/dia3.png");
    dia_card[4] = HgImageLoad("img/dia4.png");
    dia_card[5] = HgImageLoad("img/dia5.png");
    dia_card[6] = HgImageLoad("img/dia6.png");
    dia_card[7] = HgImageLoad("img/dia7.png");
    dia_card[8] = HgImageLoad("img/dia8.png");
    dia_card[9] = HgImageLoad("img/dia9.png");
    dia_card[10] = HgImageLoad("img/dia10.png");
    dia_card[11] = HgImageLoad("img/dia11.png");
    dia_card[12] = HgImageLoad("img/dia12.png");
    dia_card[13] = HgImageLoad("img/dia13.png");
    
    //l枚目のカードの位置についての情報を決定

    //省略形
    if(dm == 0){
        for(i = 1;i < 14;i++){
             if(nowhand == i){
            HgImagePut(DCARDX + CARDDIFFER * l, DCARDY, dia_card[i], CARDSIZE, 0);
        }
        }
       

    }else if(dm == 1){
        for(i = 1;i < 14;i++){
            if(nowhand == i){
                HgImagePut(MYCARDX + CARDDIFFER * l, MYCARDY, dia_card[i], CARDSIZE, 0);
            }  
        }

    }
    return 0;
}

//手札の合計を計算する関数  (11,12,13 == 10   1 == 1,11)
int handsum(int hand1, int hand2, int hand3, int hand4, int hand5)
{
    int cphand[6];
    int hand[6] = {0, hand1, hand2, hand3, hand4, hand5};
    int sum; //handの合計
    int asum = 0;
    int bsum = 0; //仮の合計
    int i;
    int l = 0; //エースの数
    //エースと10以上の数の処理
    for (i = 1; i < 6; i++)
    {
        if (hand[i] != 1 && hand[i] <= 10)
        {
            cphand[i] = hand[i];
        }
        else if (hand[i] > 10)
        {
            cphand[i] = 10;
        }
        else if (hand[i] == 1)
        {
            //1or11の計算(自身（より21に近い方を採用）)（ディーラの場合は）
            l = l + 1;
            cphand[i] = 1;
        }
    }

    //和を求める処理
    for (i = 1; i < 6; i++)
    {
        asum = asum + cphand[i]; //エースが1の判定
        if (l > 0)
        {
            bsum = asum + (l * 10); //エースが１１の判定
        }
    }
    if ((BLACKJUCK - asum) < (BLACKJUCK - bsum))
    {
        sum = asum;
    }
    else
    {
        sum = bsum;
    }
    return sum;
}

int dcardjudgement(int dhandsum, int myhandsum)
{            //ディーラがスタンドするかヒットするかを決める判断
    int hit; //hit 1 stand 0
    if (dhandsum == myhandsum)
    {
        hit = 0;
    }
    else if (dhandsum - myhandsum < 0 && dhandsum < BLACKJUCK)
    {
        hit = 1;
    }
    else
    {
        hit = 0;
    }

    return hit;
}

//買ったときの演出
int windisplay()
{
    HgClear();
    HgSetFont(HG_HB,200);
    HgText(400, 300, "WIN");
    return 0;
}
//負けたときの演出
int losedisplay()
{
    HgClear();
    HgSetFont(HG_HB,200);
    HgText(300, 300, "LOSE");
    return 0;
}
//EVENの時の演出
int evendisplay()
{
    HgClear();
    HgSetFont(HG_HB,200);
    HgText(300, 300, "EVEN");
    return 0;
}

//ブラック・ジャックになったときの演出
int blackjuckdisplay()
{
    HgClear();
    HgSetFont(HG_HB,200);
    HgText(400, 300, "WIN\nBLACKJUCK");
    return 0;
}

int burstdisplay(){
    HgClear();
    HgText(600, 400, "BURST");
    return 0;
}

//メイン関数
int main()
{                            //宣言
    int i;                   //何枚目のカードなのか判定
    int dhandsum = 0;        //ディーラの手札の合計
    int myhandsum = 0;       //自身の手札の合計
    int dhand[NUMBER] = {};  //ディーラの手札
    int myhand[NUMBER] = {}; //自身の手札
    int judge;               //勝敗のフラグ　１が自身の価値　　０が自身の負け
    int hit;                 //クリックの代理のフラグ(hit 1なら引く　0ならスタンド)
    int mykey;               //自身がスタンドするのかヒットするのかを決めるキー入力
    int card_ground;
    int card_back; //トランプの裏
    //int dia_card1, dia_card2, dia_card3, dia_card4, dia_card5, dia_card6, dia_card7, dia_card8, dia_card9, dia_card10, dia_card11, dia_card12, dia_card13;
    int lose = 0, win = 0, even = 0,blackjuck = 0;
    int burst = 0;
    //デバック用（後に消去）
    /*dia_card1 = HgImageLoad("img/dia1.png");
    dia_card2 = HgImageLoad("img/dia2.png");
    dia_card3 = HgImageLoad("img/dia3.png");
    dia_card4 = HgImageLoad("img/dia4.png");
    dia_card5 = HgImageLoad("img/dia5.png");
    dia_card6 = HgImageLoad("img/dia6.png");
    dia_card7 = HgImageLoad("img/dia7.png");
    dia_card8 = HgImageLoad("img/dia8.png");
    dia_card9 = HgImageLoad("img/dia9.png");
    dia_card10 = HgImageLoad("img/dia10.png");
    dia_card11 = HgImageLoad("img/dia11.png");
    dia_card12 = HgImageLoad("img/dia12.png");
    dia_card13 = HgImageLoad("img/dia13.png");*/
    //カードのイラストを読み込む
    card_back = HgImageLoad("img/tranpuura.jpg");
    card_ground = HgImageLoad("img/backcard.jpg");

    /*デバック用
    myhand = handdecide;
    printf("自身の手 %d",myhand);
    dhand = handdecide;
    printf("ディーラの手 %d",dhand);
   
    */

    HgOpen(WINDOWWIDTH, WINDOWHIGHT);

    /* //背景（ディーラの机とか）
    //ディーラーテーブル
    HgSetFillColor(HG_DGREEN);
    HgSetColor(HG_BROWN);
    HgSetWidth(50);
    HgCircleFill(WINDOWWIDTH / 2, WINDOWHIGHT * 19 / 20, 600, 1);
    //トランプ背景
    HgImagePut(120,120,card_ground,0.45,-0.3);
    //山札
    HgImagePut(1000, 700, card_back, 0.1, 0);
    //説明文
    HgText(1000,100,"ブラック・ジャック\nヒットの場合は「ｈ」を入力\nスタンドの場合は「ｓ」を入力\n");*/
    backgrounddisplay();

    //デバック用（自身のカード置き場）
    /*HgImagePut(MYCARDX,MYCARDY,dia_card5,CARDSIZE,0);
    HgImagePut(MYCARDX + CARDDIFFER,MYCARDY,dia_card11,CARDSIZE,0);
    HgImagePut(MYCARDX + CARDDIFFER * 2,MYCARDY,dia_card1,CARDSIZE,0);
    HgImagePut(MYCARDX + CARDDIFFER * 3,MYCARDY,dia_card13,CARDSIZE,0);
    HgImagePut(DCARDX,DCARDY,dia_card5,CARDSIZE,0);
    */

    //最初自身に二枚のカードを配る
    myhand[1] = handdecide();
    HgSleep(1); //乱数を一緒にしないために時間
    myhand[2] = handdecide();
    myhandsum = myhand[1] + myhand[2];
    displaycard(myhand[1], 1, 1);
    displaycard(myhand[2], 2, 1);

    //ディーラのカードを二枚配る
    dhand[1] = handdecide();
    HgSleep(1);
    dhand[2] = handdecide();
    dhandsum = dhand[1] + dhand[2];
    displaycard(dhand[1], 1, 0);
    HgImagePut(DCARDX + CARDDIFFER * 2, DCARDY, card_back, 0.115, 0); //二枚目のみ裏で表示
    //printf("%d,%d",myhand[0],myhand[1]);   //デバック用（正常】
    //hをおしたらhit   sを押したらスタンド  イベント判定

    //自身がヒットし続ける場合の処理とスタンドの処理
    printf("%d", mykey);
    for (i = 3, hit = 0; mykey != 's'; i++)
    {                //スタンドの処理が来るまでループ
        mykey = 'a'; // 初期化

        while (mykey != ' ') //hかｓが入力されるまで繰り返す
        {
            mykey = HgGetChar(); //キー入力
            hit = 0;
            if (mykey == 's') //stand
            {
                mykey = ' ';
                hit = 0;
            }
            else if (mykey == 'h') //hit
            {
                mykey = ' ';
                hit = 1;
            }
        }
        //追加のカードを引く動作
        if (hit == 1)
        {
            myhand[i] = handdecide(); //引く手札を決める
            //手札の合計を計算する関数
            //printf("  %d ",myhand[i]);//デバック用   //乱数を重複サせないようにする方法
            //ヒットしたのでカードを表示する
            displaycard(myhand[i], i, 1);
            HgSleep(0.5);
        }
        else
        {
            mykey = 's';
        }
    }

    //二枚目を裏から表に
    displaycard(dhand[2], 2, 0);

    //自身の合計を求める
    myhandsum = handsum(myhand[1], myhand[2], myhand[3], myhand[4], myhand[5]);
    //バーストした場合の表示
    if (myhandsum > 21)
    {
        burst = 1; //バーストした
        lose = 1;  //自身の負け
    }


    if (lose == 0)//バーストしてない場合のみ実行
    {

        //ここから先はディーラの表示
        for (i = 3; i < 6; i++)
        {
            dhandsum = handsum(dhand[1], dhand[2], dhand[3], dhand[4], dhand[5]);
            hit = 0;
            hit = dcardjudgement(dhandsum, myhandsum);
            /*デバック用
            printf(" 1::%d,", dhand[1]);
            printf(" 2::%d,", dhand[2]);
            printf(" 3::%d,", dhand[3]);
            printf(" 4::%d,", dhand[4]);
            printf(" 5::%d,", dhand[5]);   */
            //hit = 1;//デバック用
            //ヒットの処理
            if (hit == 1)
            {
                dhand[i] = handdecide();
                displaycard(dhand[i], i, 0);
            }
            if (hit == 0)
            {
                break;
            }
        }
    }
    


    //ディーラがバーストしたかどうか判断
    if(dhandsum > 21 && myhandsum <= 21){
        win = 1;
    }

    //デバック用
    //printf("%d,%d", dhandsum, myhandsum); //＊＊＊＊＊＊＊ディーラのヒット判定のアルゴリズムの見直し＊＊＊＊＊＊＊＊＊

    //勝敗の判定
    if(burst == 1) {
        lose = 1;
    }else if(myhandsum == dhandsum){
        even = 1;
    }else if(myhandsum == 21 ){
        blackjuck =1;
    } else if(myhandsum > dhandsum){//21に近い方
        win = 1;
    }else {
        lose = 1;
    }
    /*デバック用
    printf("%d\n",even);
    printf("%d\n",win);
    printf("%d\n",lose);
    printf("%d\n",blackjuck);
    printf("%d\n",burst);*/


    HgSleep(3);//勝敗演出

    //勝ち・負け・引き分け・バーストの出力
    if(even == 1){
        evendisplay();
    }else if(win == 1){
        windisplay();
    }else if(lose == 1){
        losedisplay();
    }else if(blackjuck == 1){
        blackjuckdisplay();
    }else if(burst == 1){
        burstdisplay();
    }
    

    

    HgGetChar();
    HgClose();
    return 0;
}
