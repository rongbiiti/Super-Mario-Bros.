/********************************************************************
** チーム制作：スーパーマリオブラザーズ　制作開始：2019/06/07
** 佳秀組（冨名腰佳秀、長浜颯士、呉屋翔麻、平田稀里）
********************************************************************/
#include "DxLib.h"
#include <stdlib.h>
#include <stdio.h>

/***********************************************
 * 定数の宣言
 ***********************************************/
#define DEBUG 1					// 0→デバッグOFF	1→デバッグON
#define BLOCKSIZE 32			// 1マスの大きさ
#define FONTDATA_COUNT 9		// フォントデータの数
#define SCREEN_WIDTH 512		// 画面の幅
#define SCREEN_HEIGHT 384		// 画面の高さ
#define DRAW_WINDOW_WIDTH 960	// 描画時の画面の幅。
#define DRAW_WINDOW_HEIGHT 720	// 描画時の画面の高さ
#define START_LIFE 3			// 初期ライフ
#define START_TIME 400			// 初期残り時間
#define NORMAL_SPEED 3			// 移動速度
#define DASH_SPEED 7			// ダッシュ速度
#define NORMAL_JUMPPOWER 16		// ジャンプ強度
#define DASH_JUMPPOWER 19		// ダッシュジャンプ強度
#define GRAVITY 2				// 重力
#define MAX_GRAVITY 11			// 重力最大値

/***********************************************
 * 列挙体の宣言
 ***********************************************/
typedef enum GAME_MODE{
	GAME_TITLE,
	GAME_INIT,
	GAME_PREMAIN,		// メインに入る直前の残機表示画面
	GAME_MAIN,
	GAME_OVER,
	END = 99
};

typedef enum FONTSIZE{
	Size16,				// フォントデータの配列の添え字指定用の列挙定数の宣言
	Size20,
	Size25,
	Size30,
	Size35,
	Size40,
	Size48,
	Size50,
	Size200
};

typedef enum MARIO_STATUS{
	CHIBI,		// チビマリオ
	SUPER,		// スーパーマリオ
	FIRE,		// ファイアマリオ
	STAR		// 無敵
};

typedef enum ENEMY{
	KURIBO,			// クリボー
	NOKONOKO,		// ノコノコ
	SHELL,			// カメのコウラ
	PAKKUN_FLOWER	// パックンフラワー
};

typedef enum ITEM{
	SUPER_KINOKO,	// スーパーキノコ
	FIRE_FLOWER,	// ファイアフラワー
	SUPER_STAR,		// スーパースター
	ONEUP_KINOKO,	// 1UPキノコ
};

typedef enum MAP_OBJECT{
	GROUND = 1,				// 地面ブロック
	RENGA_BLOCK,			// レンガブロック
	TEN_COIN_BLOCK,			// 10コインブロック
	HATENA_BLOCK,			// ハテナブロック
	HIDDEN_BLOCK,			// 隠しブロック
	EMPTY_BLOCK,			// カラブロック
	HARD_BLOCK,				// 硬いブロック
	COIN,					// コイン
	PIPE,					// 土管
	MIDDLE_POINT,			// 中間ポイント
	GOAL_POLE,				// ゴールポール
	SPAWN_KURIBO,			// クリボー出現位置
	SPAWN_NOKONOKO,			// ノコノコ出現位置
	SPAWN_PAKKUN_FLOWER		// パックンフラワー出現位置
};

typedef enum INIT_STATUS{
	FIRST,				// 1-1から
	MISS,				// ミス
	CLEAR				// コースクリア
};

/***********************************************
 * 変数の宣言
 ***********************************************/
int Buf[ 256 ] ;				// キーボード入力情報取得のための変数
int FontSize[ FONTDATA_COUNT ];	// フォントデータを保存しておく配列

int g_GameState = GAME_INIT;

int g_Score;
int g_Coin;
int g_Time;

int g_InitStatus;				// 初期処理で一部の変数を初期化するか判断するための変数。0:初回、1:ミス、2:クリア。
bool g_IsThroughMiddlePoint;	// 中間地点を通過したか

/***********************************************
 * 構造体の宣言
 ***********************************************/
struct PLAYER {
	int life;
	int x,y;		// 座標 x,y
	bool angle;		// 向いている方向。 左0←	→1右
	bool isjump;	// ジャンプ中:1
	int jumppower;	// ジャンプ力
	int jumpcnt;
	bool isinv;		// 無敵フラグ(0:通常状態 ,1:無敵状態)。敵と衝突したときに使う
	int invcnt;		// 無敵状態の残り時間
};
struct PLAYER g_Player;

/***********************************************
 * 画像データ用変数の宣言
 ***********************************************/
int g_Mapchip[84];		// マップチップ画像格納用配列

/***********************************************
 * 音データ用変数の宣言
 ***********************************************/


/***********************************************
 * ステージ配列の宣言
 ***********************************************/
int g_InitStage[12][100] = {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,5,1,5,1,5,-1,-1,-1,-1,-1,-1,36,37,38,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,48,49,50,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,14,15,16,-1,21,22,23,-1,-1,-1,-1,-1,-1,-1,21,22,23,-1,-1,-1,-1,-1,-1,-1,21,22,23,48,49,50,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};


int g_Stage[12][100];
/***********************************************
 関数のプロトタイプ宣言
 **********************************************/
void InputKey(void);		// 0:入力されていない 1:入力された瞬間 2以上:入力されている

void DrawTitle(void);		// タイトル画面描画
void GameInit(void);		// 初期化処理
void DrawPreMain(void);		// メイン直前のマリオ残機数表示画面

void DrawMain(void);		// ゲームメイン
void PlayerControll(void);	// プレイヤー制御
bool IsPushDashKey(void);	// ダッシュキー押してるか判断
bool IsPushJumpKey(void);	// ジャンプキー押してるか判断
void PlayerJump(void);		// ジャンプ処理
void DrawPlayer(void);		// プレイヤー描画
void DrawStage(void);		// ステージ描画
void DrawUI(void);			// 画面上部のUI描画
void DrawDebugInfo(void);	// デバッグ用情報表示

void DrawGameOver(void);	// ゲームオーバー画面描画

int LoadImages(void);		// 画像データロード
int LoadSounds(void);		// 音データロード
void CreateFontData(void);	// フォントデータ作成

/***********************************************
 * プログラムの開始
 ***********************************************/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	SetMainWindowText( "Super Mario Bros." );

	SetGraphMode( DRAW_WINDOW_WIDTH , DRAW_WINDOW_HEIGHT , 32 );

	ChangeWindowMode( TRUE );

	if ( DxLib_Init() == -1 ) return -1;

	int offscreen_handle = MakeScreen( SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);

	SetDrawScreen( offscreen_handle );

	if ( LoadImages() == -1 ) return -1; // 画像読込み関数を呼び出し
	if ( LoadSounds() == -1 ) return -1; // サウンド読込み関数を呼び出し
	CreateFontData();

	// ゲームループ
	while ( ProcessMessage() == 0 && g_GameState != END && ( Buf[ KEY_INPUT_ESCAPE ] == 0 )){

		SetDrawScreen(offscreen_handle);
		ClearDrawScreen();
		InputKey();

		switch (g_GameState){
			case GAME_TITLE:
				DrawTitle();
			break;
			case GAME_INIT:
				GameInit();
			break;
			case GAME_PREMAIN:
				DrawPreMain();
			break;
			case GAME_MAIN:
				DrawMain();
			break;
			case GAME_OVER:
				DrawGameOver();
			break;
		}

		SetDrawScreen( DX_SCREEN_BACK );
		DrawExtendGraph( 0, 0, DRAW_WINDOW_WIDTH, DRAW_WINDOW_HEIGHT, offscreen_handle, FALSE);
		ScreenFlip();

	}

	if ( DxLib_Init() == -1 ) return -1;

	DxLib_End();

	return 0;
}

/***********************************************
 * 0:入力されていない 1:入力された瞬間 2以上:入力されている
 ***********************************************/
void InputKey(void)
{
	static char Key[256];
    GetHitKeyStateAll(Key);
    for (int i = 0; i < 256; i++) {
        if (Key[i]) Buf[i]++;		// 入力されたキーは押されている間どんどん数字が増えていく。
        else        Buf[i] = 0;		// 離すと一気に0に戻る
    }
}

/***********************************************
 タイトル画面描画
 **********************************************/
void DrawTitle(void)
{
	if( Buf[ KEY_INPUT_RETURN ] == 1){
		g_GameState = GAME_INIT;
	}

}

/***********************************************
 初期化処理
 **********************************************/
void GameInit(void)
{
	g_Player.x = 64;
	g_Player.y = 288;
	g_Player.angle = 1;
	g_Player.isinv = FALSE;
	g_Player.invcnt = 0;

	switch (g_InitStatus)
	{
	case FIRST:
		g_Player.life = START_LIFE;
		g_IsThroughMiddlePoint = FALSE;
		g_Score = 0;
		g_Coin = 0;
		g_Time = START_TIME;
		break;
	case MISS:
		--g_Player.life;
		break;
	case CLEAR:
		g_IsThroughMiddlePoint = FALSE;
		break;
	}

	if( g_IsThroughMiddlePoint == TRUE){
		g_Time = START_TIME / 2;
	}

	for(int i=0; i<12; i++){
		for( int j=0; j<100; j++){
			g_Stage[i][j] = g_InitStage[i][j];
		}
	}

	g_GameState = GAME_PREMAIN;
}

/***********************************************
 メイン直前のマリオ残機数表示画面
 **********************************************/
void DrawPreMain(void)
{
	static int WaitTime = 0;

	DrawUI();

	if( ++WaitTime >= 150){
		g_GameState = GAME_MAIN;
		WaitTime = 0;
	}
}

/***********************************************
 ゲームメイン
 **********************************************/
void DrawMain(void)
{
	PlayerControll();

	DrawStage();
	DrawPlayer();
	DrawUI();
	if( DEBUG == TRUE) DrawDebugInfo();
}

/***********************************************
 プレイヤー制御
 **********************************************/
void PlayerControll(void)
{
	if( Buf[ KEY_INPUT_LEFT ] != 0){
		if( IsPushDashKey() == TRUE){
			g_Player.x -= DASH_SPEED;
		}else{
			g_Player.x -= NORMAL_SPEED;
		}
		
	}else if( Buf[ KEY_INPUT_RIGHT ] != 0){
		if( IsPushDashKey() == TRUE){
			g_Player.x += DASH_SPEED;
		}else{
			g_Player.x += NORMAL_SPEED;
		}
	}

	PlayerJump();
}

/***********************************************
 ダッシュキー押してるか判断
 **********************************************/
bool IsPushDashKey(void)
{
	if( Buf[ KEY_INPUT_LSHIFT ] != 0){
		return TRUE;
	}
	return FALSE;
}

/***********************************************
 ジャンプキー押してるか判断
 **********************************************/
bool IsPushJumpKey(void)
{
	if( Buf[ KEY_INPUT_SPACE ] != 0){
		return TRUE;
	}
	return FALSE;
}

/***********************************************
 ジャンプ処理
 **********************************************/
void PlayerJump(void)
{
	if( g_Player.isjump == TRUE){
		g_Player.y -= g_Player.jumppower;
		if( g_Player.jumppower > -MAX_GRAVITY){
			//上昇中かつジャンプキー押してる間は加工量を2分の1にする
			if( IsPushJumpKey() == TRUE && g_Player.jumppower > 0){
				g_Player.jumppower -= (GRAVITY - (GRAVITY / 2));
			}else{
				g_Player.jumppower -= GRAVITY;
			}
		}
		if( g_Player.y > 288){
			g_Player.y = 288;
			g_Player.isjump = FALSE;
		}
	}

	if( g_Player.isjump == FALSE && IsPushJumpKey() == TRUE){
		g_Player.isjump = TRUE;
		g_Player.jumppower = IsPushDashKey() ? DASH_JUMPPOWER : NORMAL_JUMPPOWER;
	}
}


/***********************************************
 プレイヤー描画
 **********************************************/
void DrawPlayer(void)
{
	DrawCircle( BLOCKSIZE / 2 + g_Player.x, BLOCKSIZE / 2 + g_Player.y, BLOCKSIZE / 2, 0xDD3333);
}

/***********************************************
 ステージ描画
 **********************************************/
void DrawStage(void)
{
	DrawBox( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x00bfff, TRUE);

	for( int i=0; i<12; i++){
		for( int j=0; j<100; j++){
			DrawGraph( j * BLOCKSIZE, i * BLOCKSIZE, g_Mapchip[g_Stage[i][j]], TRUE);
		}
	}
}

/***********************************************
 画面上部のUI描画
 **********************************************/
void DrawUI(void)
{

}

/***********************************************
 デバッグ用情報表示
 **********************************************/
void DrawDebugInfo(void)
{
	bool IsDrawEnd = 0;
	int i = 0;
	while (IsDrawEnd == 0)
	{
		//DrawFormat...1文をコピって " " の中身を変えるだけで見たい情報を見れます。

		DrawFormatStringToHandle( 5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "左シフトキー:ダッシュ");
		DrawFormatStringToHandle( 5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "スペースキー:ジャンプ");

		DrawFormatStringToHandle( 5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.x:%d", g_Player.x);
		DrawFormatStringToHandle( 5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.y:%d", g_Player.y);

		IsDrawEnd = 1;
	}
}

/***********************************************
 ゲームオーバー画面描画
 **********************************************/
void DrawGameOver(void)
{

}

/***********************************************
 画像データロード ※画像フォルダの名前は「Image」です。
 **********************************************/
int LoadImages(void)
{
	//if((g_PlayerImage[Wait] = LoadGraph("Image/player normal.png")) == -1) return -1;

	if( LoadDivGraph("Image/backimage.png", 84, 12, 7, 32, 32, g_Mapchip) == -1) return -1;

	return 0;
}

/***********************************************
 音データロード ※音フォルダの名前は「Sound」です。
 **********************************************/
int LoadSounds(void)
{
	//if((s_Desision = LoadSoundMem("Sound/kettei_kin.mp3")) == -1) return -1;

	return 0;
}

/***********************************************
 フォントデータ作成
 **********************************************/
void CreateFontData(void)
{
	FontSize[Size16] = CreateFontToHandle( NULL, 16, -1, DX_FONTTYPE_ANTIALIASING_EDGE);	//フォントデータをあらかじめ作成しておく
	FontSize[Size20] = CreateFontToHandle( NULL, 20, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size25] = CreateFontToHandle( NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size30] = CreateFontToHandle( NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size35] = CreateFontToHandle( NULL, 35, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size40] = CreateFontToHandle( NULL, 40, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size48] = CreateFontToHandle( NULL, 48, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size50] = CreateFontToHandle( NULL, 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size200] = CreateFontToHandle( NULL, 200, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

}