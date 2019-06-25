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
#define FONTDATA_COUNT 10		// フォントデータの数
#define SCREEN_WIDTH 640		// 画面の幅
#define SCREEN_HEIGHT 512		// 画面の高さ
#define DRAW_WINDOW_WIDTH 1152	// 描画時の画面の幅。
#define DRAW_WINDOW_HEIGHT 864	// 描画時の画面の高さ
#define MAP_WIDTH 221			// マップの幅
#define MAP_HEIGHT 16			// マップの高さ
#define START_LIFE 3			// 初期ライフ
#define START_TIME 400			// 初期残り時間
#define NORMAL_SPEED 3			// 移動速度
#define DASH_SPEED 5			// ダッシュ速度
#define NORMAL_JUMPPOWER 18		// ジャンプ強度
#define DASH_JUMPPOWER 18		// ダッシュジャンプ強度
#define GRAVITY 2				// 重力
#define MAX_GRAVITY 7			// 重力最大値
#define HITBOX	32				// マリオの当たり判定

const float NORMAL_ACCEL = 0.23f;
const float DASH_ACCEL = 0.32f;

/***********************************************
 * 列挙体の宣言
 ***********************************************/
typedef enum GAME_MODE {
	GAME_TITLE,
	GAME_INIT,
	GAME_PREMAIN,		// メインに入る直前の残機表示画面
	GAME_MAIN,
	GAME_OVER,
	END = 99
};

typedef enum FONTSIZE {
	Size10,
	Size16,				// フォントデータの配列の添え字指定用の列挙定数の宣言
	Size20,
	Size25,
	Size30,
	Size35,
	Size40,
	Size48,
	Size100,
	Size200
};

typedef enum MARIO_STATUS {
	CHIBI,		// チビマリオ
	SUPER,		// スーパーマリオ
	FIRE,		// ファイアマリオ
	STAR		// 無敵
};

typedef enum ENEMY {
	KURIBO,			// クリボー
	NOKONOKO,		// ノコノコ
	SHELL,			// カメのコウラ
	PAKKUN_FLOWER	// パックンフラワー
};

typedef enum ITEM {
	SUPER_KINOKO,	// スーパーキノコ
	FIRE_FLOWER,	// ファイアフラワー
	SUPER_STAR,		// スーパースター
	ONEUP_KINOKO,	// 1UPキノコ
};

typedef enum MAP_OBJECT {
	GROUND = 7,				// 地面ブロック
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

typedef enum INIT_STATUS {
	FIRST,				// 1-1から
	MISS,				// ミス
	CLEAR				// コースクリア
};

/***********************************************
 * 変数の宣言
 ***********************************************/
int Buf[256];				// キーボード入力情報取得のための変数
int FontSize[FONTDATA_COUNT];	// フォントデータを保存しておく配列

int g_GameState = GAME_INIT;

int g_ScrollQuantity;

int g_Score;
int g_Coin;
int g_Time;

int g_HitBlockX;	// 当たった天井のブロックのX座標
int g_HitBlockY;	// 当たった天井のブロックのY座標
int g_BlockAnimCnt;	// 当たったブロックが上下に動くときの変数
int g_BlockAnimFlg;// ブロックが上下に動くときのフラグ。動いてない0、上昇中1、下降中2。

int g_InitStatus;				// 初期処理で一部の変数を初期化するか判断するための変数。0:初回、1:ミス、2:クリア。
bool g_IsThroughMiddlePoint;	// 中間地点を通過したか

/***********************************************
 * 構造体の宣言
 ***********************************************/
struct PLAYER {
	int life;
	int status;
	int x, y;		// 座標 x,y
	int px, py;		// 移動前座標
	float mx;
	bool angle;		// 向いている方向。 左1←	→0右
	bool isjump;	// ジャンプ中:1
	int jumppower;	// ジャンプ力
	bool jumpOK;
	bool isground;
	bool isinv;		// 無敵フラグ(0:通常状態 ,1:無敵状態)。敵と衝突したときに使う
	int invcnt;		// 無敵状態の残り時間
	int animnum;	// アニメーションの番号
	bool issuper;	// デカマリオか
	bool iscrouch;	// デカマリオ中にしゃがんでいるか	
};																	
struct PLAYER g_Player;

/***********************************************
 * 画像データ用変数の宣言
 ***********************************************/
int g_Mapchip[84];		// マップチップ画像格納用配列
int g_MarioImage[3][15];		// マリオキャラチップ画像

/***********************************************
 * 音データ用変数の宣言
 ***********************************************/


 /***********************************************
  * ステージ配列の宣言
  ***********************************************/
int g_InitStage[MAP_HEIGHT][MAP_WIDTH] = { {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,18,19,19,19,20,-1,-1,-1,-1,30,31,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,19,19,20,-1,-1,-1,-1,30,31,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,18,19,19,19,20,-1,-1,-1,-1,30,31,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,33,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,-1,18,19,19,19,20,-1,-1,-1,18,19,19,20,-1,-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,30,31,31,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,31,31,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,31,31,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,34,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,18,19,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1,30,31,31,31,32,-1,-1,-1,30,31,31,32,-1,-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,5,5,5,5,5,5,-1,-1,-1,5,5,5,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,5,5,-1,-1,-1,-1,5,1,1,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,-1,-1,-1,-1,-1,-1,-1,-1,34,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,30,31,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,8,-1,-1,-1,-1,-1,-1,-1,-1,34,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,8,8,-1,-1,-1,-1,-1,-1,-1,-1,34,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,68,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,8,8,8,-1,-1,-1,-1,-1,-1,-1,-1,34,-1,-1,-1,-1,42,42,42,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,5,1,5,1,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,64,65,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,64,65,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,1,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,-1,-1,-1,-1,-1,5,5,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,5,-1,-1,-1,-1,-1,-1,8,-1,-1,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,-1,-1,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,5,1,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,8,8,8,8,-1,-1,-1,-1,-1,-1,-1,-1,34,-1,-1,-1,-1,44,45,46,-1,-1,-1,-1,-1,-1},
											{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,64,65,-1,-1,-1,-1,-1,-1,-1,76,77,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,76,77,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,-1,-1,8,8,-1,-1,-1,-1,17,-1,-1,-1,8,8,8,-1,-1,8,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,8,8,8,8,8,-1,-1,-1,17,-1,-1,-1,-1,34,-1,-1,-1,42,43,43,43,42,-1,-1,-1,-1,-1},
											{-1,17,-1,-1,-1,-1,-1,64,65,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,64,65,-1,-1,-1,-1,-1,-1,-1,-1,76,77,-1,-1,-1,-1,-1,-1,-1,76,77,-1,-1,14,15,16,-1,-1,-1,-1,-1,76,77,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,14,15,16,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,8,-1,-1,8,8,8,-1,-1,14,15,16,-1,8,8,8,8,-1,-1,8,8,8,-1,-1,-1,17,-1,64,65,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,64,65,-1,8,8,8,8,8,8,8,8,-1,-1,14,15,16,-1,-1,-1,34,-1,-1,-1,45,45,47,45,45,-1,-1,17,-1,-1},
											{14,15,16,-1,-1,-1,-1,76,77,-1,-1,-1,21,22,22,22,23,14,15,16,-1,-1,-1,-1,21,22,23,-1,-1,76,77,-1,-1,-1,-1,-1,-1,-1,-1,76,77,-1,-1,21,22,22,23,-1,76,77,-1,14,15,15,15,16,-1,-1,-1,-1,76,77,-1,21,22,22,22,23,14,15,16,-1,-1,-1,-1,-1,21,22,23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,21,22,22,23,-1,-1,-1,14,15,15,15,16,-1,-1,-1,-1,-1,-1,-1,21,22,22,22,23,14,15,16,-1,-1,-1,-1,-1,21,22,23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,8,8,22,22,8,8,8,8,14,15,15,15,8,8,8,8,8,-1,-1,8,8,8,8,23,14,15,16,76,77,-1,-1,-1,-1,21,22,23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,76,77,8,8,8,8,8,8,8,8,8,-1,14,15,15,15,16,-1,-1,8,-1,-1,-1,45,45,59,45,45,23,14,15,16,-1},
											{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},
											{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7} };



int g_Stage[MAP_HEIGHT][MAP_WIDTH];
/***********************************************
 関数のプロトタイプ宣言
 **********************************************/
void InputKey(void);		// 0:入力されていない 1:入力された瞬間 2以上:入力されている

void DrawTitle(void);		// タイトル画面描画
void GameInit(void);		// 初期化処理
void StageInit(void);		// ステージ初期化処理
void DrawPreMain(void);		// メイン直前のマリオ残機数表示画面

void DrawMain(void);		// ゲームメイン
void PlayerControll(void);	// プレイヤー制御
void PlayerFall(void);		// プレイヤー落下
int IsBlock(int x, int y);	// その座標にブロックがあるか判定する
int IsHitBlockR(void);		// 右側ブロックとの当たり判定
int IsHitBlockL(void);		// 左側ブロックとの当たり判定
int IsHitGround(void);		// 地面との当たり判定
int IsHitCeil(void);		// 天井との当たり判定
void HitBlockMemo(int x, int y);	// 当たった天井のブロックの座標を記憶しておき、ブロックアニメフラグを立てる
int IsIntoHole(int y);		// 穴に落ちたか判定
void FixPlayerPosition(void);// プレイヤーが画面端から出ないようにする
bool IsPushDashKey(void);	// ダッシュキー押してるか判断
bool IsPushCrouchKey(void);	// しゃがみキー押してるか判断
bool IsPushJumpKey(void);	// ジャンプキー押してるか判断
void PlayerJump(void);		// ジャンプ処理

void DrawPlayer(void);		// プレイヤー描画
void PlayerAnimControll(void);	// プレイヤーアニメション制御

int IsItemHitBlockR(int x, int y);	// アイテムと右側ブロックとの当たり判定
int IsItemHitBlockL(int x, int y);	// アイテムと左側ブロックとの当たり判定
int IsItemHitGround(int x, int y);	// アイテムと地面との当たり判定

void DrawStage(void);		// ステージ描画
void StageScroll(void);		// ステージのスクロール
void DrawUI(void);			// 画面上部のUI描画
void DrawDebugInfo(void);	// デバッグ用情報表示

void DrawGameOver(void);	// ゲームオーバー画面描画

int LoadImages(void);		// 画像データロード
int LoadSounds(void);		// 音データロード
void CreateFontData(void);	// フォントデータ作成

/***********************************************
 * プログラムの開始
 ***********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("Super Mario Bros.");

	SetGraphMode(DRAW_WINDOW_WIDTH, DRAW_WINDOW_HEIGHT, 32);

	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1) return -1;

	int offscreen_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);

	SetDrawScreen(offscreen_handle);

	if (LoadImages() == -1) return -1; // 画像読込み関数を呼び出し
	if (LoadSounds() == -1) return -1; // サウンド読込み関数を呼び出し
	CreateFontData();

	// ゲームループ
	while (ProcessMessage() == 0 && g_GameState != END && (Buf[KEY_INPUT_ESCAPE] == 0)) {

		SetDrawScreen(offscreen_handle);
		ClearDrawScreen();
		InputKey();

		switch (g_GameState) {
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

		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, DRAW_WINDOW_WIDTH, DRAW_WINDOW_HEIGHT, offscreen_handle, FALSE);
		ScreenFlip();

	}

	if (DxLib_Init() == -1) return -1;

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
	if (Buf[KEY_INPUT_RETURN] == 1) {
		g_GameState = GAME_INIT;
	}

}

/***********************************************
 初期化処理
 **********************************************/
void GameInit(void)
{
	g_Player.x = 80;
	g_Player.mx = 0;
	g_Player.y = 431;
	g_Player.angle = 0;
	g_Player.isinv = FALSE;
	g_Player.invcnt = 0;
	g_Player.jumppower = 0;
	g_Player.jumpOK = TRUE;
	g_Player.isground = TRUE;
	g_Player.iscrouch = FALSE;

	switch (g_InitStatus)
	{
	case FIRST:
		g_Player.status = SUPER;
		g_Player.life = START_LIFE;
		g_IsThroughMiddlePoint = FALSE;
		g_Score = 0;
		g_Coin = 0;
		g_Time = START_TIME;
		g_Player.issuper = 1;
		break;
	case MISS:
		g_Player.status = CHIBI;
		--g_Player.life;
		g_Player.issuper = FALSE;
		break;
	case CLEAR:
		g_IsThroughMiddlePoint = FALSE;
		break;
	}

	if (g_IsThroughMiddlePoint == TRUE) {
		g_Time = START_TIME / 2;
	}

	g_ScrollQuantity = 0;
	g_HitBlockX = 0;
	g_HitBlockY = 0;
	g_BlockAnimFlg = 0;
	g_BlockAnimCnt = 0;

	StageInit();

	g_GameState = GAME_PREMAIN;
}

/***********************************************
 ステージ初期化処理
 **********************************************/
void StageInit(void)
{
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			g_Stage[i][j] = g_InitStage[i][j];
		}
	}
}

/***********************************************
 メイン直前のマリオ残機数表示画面
 **********************************************/
void DrawPreMain(void)
{
	static int WaitTime = 0;

	DrawUI();

	if (++WaitTime >= 0) {
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
	if (DEBUG == TRUE) DrawDebugInfo();
}

/***********************************************
 プレイヤー制御
 **********************************************/
void PlayerControll(void)
{

	g_Player.px = g_Player.x;
	g_Player.py = g_Player.y;

	if (Buf[KEY_INPUT_LEFT] != 0 && g_Player.iscrouch == FALSE) {
		if (IsPushDashKey() == TRUE) {
			g_Player.mx -= DASH_ACCEL;
			if (g_Player.mx < -DASH_SPEED) g_Player.mx = -DASH_SPEED;
		}
		else {
			g_Player.mx -= NORMAL_ACCEL;
			if (g_Player.mx < -NORMAL_SPEED) g_Player.mx = -NORMAL_SPEED;
		}
		if (IsHitBlockL() == 0) {
			g_Player.x += (int)g_Player.mx;
		}
		if (g_Player.mx > 0 && IsHitBlockR() == 1) {
			g_Player.x -= (int)g_Player.mx;
		}
		else if (g_Player.mx > 0 && IsHitBlockR() == 0) {
			StageScroll();
		}

		if (g_Player.isjump == FALSE) {
			g_Player.angle = TRUE;
		}

	}
	else if (Buf[KEY_INPUT_RIGHT] != 0 && g_Player.iscrouch == FALSE) {
		if (IsPushDashKey() == TRUE) {
			g_Player.mx += DASH_ACCEL;
			if (g_Player.mx > DASH_SPEED) g_Player.mx = DASH_SPEED;
		}
		else {
			g_Player.mx += NORMAL_ACCEL;
			if (g_Player.mx > NORMAL_SPEED) g_Player.mx = NORMAL_SPEED;
		}
		if (IsHitBlockR() == 0) {
			g_Player.x += (int)g_Player.mx;
			StageScroll();
		}
		if (g_Player.mx < 0 && IsHitBlockL() == 1) {
			g_Player.x -= (int)g_Player.mx;
		}

		if (g_Player.isjump == FALSE) {
			g_Player.angle = FALSE;
		}
	}
	else {
		if (g_Player.mx < 0) {
			g_Player.mx += NORMAL_ACCEL;
			if (IsHitBlockL() == 0) {
				g_Player.x += (int)g_Player.mx;
			}
		}
		else if (g_Player.mx > 0) {
			g_Player.mx -= NORMAL_ACCEL;
			if (IsHitBlockR() == 0) {
				g_Player.x += (int)g_Player.mx;
				StageScroll();
			}
		}
	}

	PlayerJump();
	PlayerFall();
	FixPlayerPosition();
	if( IsIntoHole(g_Player.y)){
		g_GameState = GAME_INIT;
	}
}
/***********************************************
 プレイヤー落下
 **********************************************/
void PlayerFall(void)
{
	if (IsHitGround() == FALSE && g_Player.isjump == FALSE) {
		if (g_Player.jumppower > -MAX_GRAVITY) {
			g_Player.jumppower -= GRAVITY;
		}
		g_Player.y -= g_Player.jumppower;
		if (IsHitCeil() == TRUE && g_Player.jumppower > 0) {
			g_Player.isjump = FALSE;
			g_Player.jumppower = 0;
		}

		if (IsHitGround() == TRUE) {
			g_Player.y = (g_Player.y / BLOCKSIZE * BLOCKSIZE) + (BLOCKSIZE / 2) - 1;
			g_Player.isjump = FALSE;
		}
	}

}
/***********************************************
 その座標がブロックか判定する。
 引数：判定させたいキャラクターのX、Y座標。
 返り値：ブロックだったら1、なにもないかコインなどのアイテムなら0。
 **********************************************/
int IsBlock(int x, int y)
{
	if ((g_Stage[y][x] >= 1 && g_Stage[y][x] <= 8) ||
		(g_Stage[y][x] >= 36 && g_Stage[y][x] <= 41) ||
		(g_Stage[y][x] >= 60 && g_Stage[y][x] <= 65) ||
		(g_Stage[y][x] >= 72 && g_Stage[y][x] <= 77)) {
		return 1;
	}
	return 0;
}
/***********************************************
 ブロックとの当たり判定
 **********************************************/
int IsHitBlockR(void)
{
	int p_right = g_Player.x + HITBOX / 2 + 1;
	int w_x = (p_right + g_ScrollQuantity) / BLOCKSIZE;
	int p_upper = (g_Player.y - HITBOX / 4) - (g_Player.issuper * BLOCKSIZE) + (g_Player.iscrouch * BLOCKSIZE);
	int m_upper = p_upper / BLOCKSIZE;

	if (IsBlock(w_x, m_upper)){
		if ((g_Player.x + HITBOX / 2 + 1) > (w_x * BLOCKSIZE - g_ScrollQuantity)) {
			g_Player.x -= (g_Player.x + HITBOX / 2 + 1) - (w_x * BLOCKSIZE - g_ScrollQuantity);
		}
		return 1;
	}

	int p_bottom = g_Player.y + HITBOX / 2;
	int m_bottom = p_bottom / BLOCKSIZE;

	if (IsBlock(w_x, m_bottom)) {
		if ((g_Player.x + HITBOX / 2 + 1) > (w_x * BLOCKSIZE - g_ScrollQuantity)) {
			g_Player.x -= (g_Player.x + HITBOX / 2 + 1) - (w_x * BLOCKSIZE - g_ScrollQuantity);
		}
		return 1;
	}

	return 0;
}

/***********************************************
 左側ブロックとの当たり判定
 **********************************************/
int IsHitBlockL(void)
{
	int p_left = g_Player.x - HITBOX / 2;
	int w_x = (p_left + g_ScrollQuantity) / BLOCKSIZE;
	int p_upper = g_Player.y - HITBOX / 2;
	int m_upper = p_upper / BLOCKSIZE;

	if (IsBlock(w_x, m_upper)) {

		return 1;
	}

	int p_bottom = g_Player.y + HITBOX / 2;
	int m_bottom = p_bottom / BLOCKSIZE;

	if (IsBlock(w_x, m_bottom)) {

		return 1;
	}

	return 0;
}

/***********************************************
 地面との当たり判定
 **********************************************/
int IsHitGround(void)
{
	int p_bottom = g_Player.y + HITBOX / 2 + 1;
	int m_bottom = p_bottom / BLOCKSIZE;
	int p_left = g_Player.x - HITBOX / 2 + 1 - (int)g_Player.mx*2;
	int m_left = (p_left + g_ScrollQuantity) / BLOCKSIZE;

	if (IsBlock(m_left, m_bottom)) {
		if (p_bottom > m_bottom * BLOCKSIZE-8) {
			g_Player.y -= p_bottom - m_bottom * BLOCKSIZE;
		}

		return 1;
	}

	int p_right = g_Player.x + HITBOX / 2 - 1;
	int m_right = (p_right + g_ScrollQuantity) / BLOCKSIZE;

	if (IsBlock(m_right, m_bottom)) {
		if (p_bottom > m_bottom * BLOCKSIZE) {
			g_Player.y -= p_bottom - m_bottom * BLOCKSIZE;
		}

		return 1;
	}

	return 0;
}

/***********************************************
 天井との当たり判定
 **********************************************/
int IsHitCeil(void)
{
	int p_upper = (g_Player.y - HITBOX / 2) - (g_Player.issuper * BLOCKSIZE) + (g_Player.iscrouch * BLOCKSIZE);
	int m_upper = p_upper / BLOCKSIZE;
	int p_left = g_Player.x - HITBOX / 2 + 1 - (int)g_Player.mx * 2;
	int m_left = (p_left + g_ScrollQuantity) / BLOCKSIZE;

	int p_right = g_Player.x + HITBOX / 2 - 1;
	int m_right = (p_right + g_ScrollQuantity) / BLOCKSIZE;

	if (IsBlock(m_left, m_upper)) {
		if (g_Player.x + g_ScrollQuantity - (int)g_Player.mx * 2 >= m_left * BLOCKSIZE+32) {
			p_left = g_Player.x + ((m_left * BLOCKSIZE + 32 - (g_Player.x + g_ScrollQuantity)) + 16 + (int)g_Player.mx * 2);
			m_left = (p_left + g_ScrollQuantity) / BLOCKSIZE;
			if (IsBlock(m_left, m_upper)) {
				HitBlockMemo(m_left, m_upper);
				return 1;
			}
			g_Player.x += (m_left * BLOCKSIZE + 32 - (g_Player.x + g_ScrollQuantity)) - 16 + (int)g_Player.mx * 2;
			return 0;
		}
		HitBlockMemo(m_left, m_upper);
		return 1;
	}


	if (IsBlock(m_right, m_upper)) {
		if (g_Player.x + g_ScrollQuantity - 2 + (int)g_Player.mx <= m_right * BLOCKSIZE) {
			p_right = g_Player.x - (((g_Player.x + g_ScrollQuantity) - m_right * BLOCKSIZE) + 5 + (int)g_Player.mx * 2);
			m_right = (p_right + g_ScrollQuantity) / BLOCKSIZE;
			if (IsBlock(m_right, m_upper)) {
				HitBlockMemo(m_right, m_upper);
				return 1;
			}
			g_Player.x -= ((g_Player.x + g_ScrollQuantity) - m_right * BLOCKSIZE) - 16 + (int)g_Player.mx * 2;
			return 0;
		}
		HitBlockMemo(m_right, m_upper);
		return 1;
	}

	return 0;
}

/***********************************************
 天井との当たり判定
 **********************************************/
void HitBlockMemo(int x, int y)
{
	g_HitBlockX = x;
	g_HitBlockY = y;
	g_BlockAnimFlg = 1;
	g_BlockAnimCnt = 0;
}
/***********************************************
 穴に落ちたか判定
 引数：Y座標
 返り値：落ちていたら1
 **********************************************/
int IsIntoHole(int y)
{
	if( y >= SCREEN_HEIGHT + BLOCKSIZE)
	{
		return 1;
	}

	return 0;
}
/***********************************************
 アイテムと右側ブロックの当たり判定
 引数：アイテムのX、Y座標
 返り値：ブロックと衝突していたら1、でなければ0
 **********************************************/
int IsItemHitBlockR(int x, int y)
{
	int p_right = x + HITBOX / 2 + 1;
	int w_x = (p_right + g_ScrollQuantity) / BLOCKSIZE;

	int p_bottom = y + HITBOX / 2;
	int m_bottom = p_bottom / BLOCKSIZE;

	if (IsBlock(w_x, m_bottom)) {
		return 1;
	}

	return 0;
}

/***********************************************
 アイテムと左側ブロックの当たり判定
 引数：アイテムのX、Y座標
 返り値：ブロックと衝突していたら1、でなければ0
 **********************************************/
int IsItemHitBlockL(int x, int y)
{
	int p_left = g_Player.x - HITBOX / 2;
	int w_x = (p_left + g_ScrollQuantity) / BLOCKSIZE;

	int p_bottom = y + HITBOX / 2;
	int m_bottom = p_bottom / BLOCKSIZE;

	if (IsBlock(w_x, m_bottom)) {
		return 1;
	}

	return 0;
}

/***********************************************
 アイテムと地面の当たり判定
 引数：アイテムのX、Y座標
 返り値：ブロックと衝突していたら1、でなければ0
 **********************************************/
int IsItemHitGround(int x, int y)
{
	int p_bottom = y + HITBOX / 2 + 1;
	int m_bottom = p_bottom / BLOCKSIZE;
	int p_left = x - HITBOX / 2 + 1;
	int m_left = (p_left + g_ScrollQuantity) / BLOCKSIZE;

	if (IsBlock(m_left, m_bottom)) {
		if (p_bottom > m_bottom * BLOCKSIZE-8) {
			y -= p_bottom - m_bottom * BLOCKSIZE;
		}

		return 1;
	}

	int p_right = x + HITBOX / 2 - 1;
	int m_right = (p_right + g_ScrollQuantity) / BLOCKSIZE;

	if (IsBlock(m_right, m_bottom)) {
		if (p_bottom > m_bottom * BLOCKSIZE) {
			y -= p_bottom - m_bottom * BLOCKSIZE;
		}

		return 1;
	}

	return 0;
}

/***********************************************
 プレイヤーが画面端から出ないようにする
 **********************************************/
void FixPlayerPosition(void)
{
	if (g_Player.x - HITBOX / 2 <= 0) g_Player.x = 0 + HITBOX / 2;
	if (g_Player.x + HITBOX / 2 >= SCREEN_WIDTH) g_Player.x = SCREEN_WIDTH - HITBOX / 2;
}
/***********************************************
 ダッシュキー押してるか判断
 **********************************************/
bool IsPushDashKey(void)
{
	if (Buf[KEY_INPUT_LSHIFT] != 0) {
		return TRUE;
	}
	return FALSE;
}

/***********************************************
 しゃがみキー押してるか判断
 **********************************************/
bool IsPushCrouchKey(void)
{
	if (Buf[KEY_INPUT_DOWN] != 0) {
		g_Player.iscrouch = TRUE;
		return TRUE;
	}
	g_Player.iscrouch = FALSE;
	return FALSE;
}

/***********************************************
 ジャンプキー押してるか判断
 **********************************************/
bool IsPushJumpKey(void)
{
	if (Buf[KEY_INPUT_SPACE] != 0) {
		return TRUE;
	}
	return FALSE;
}

/***********************************************
 ジャンプ処理
 **********************************************/
void PlayerJump(void)
{
	if (g_Player.isjump == TRUE) {
		if (g_Player.jumppower > -MAX_GRAVITY) {
			//上昇中かつジャンプキー押してる間は加工量を2分の1にする
			if (IsPushJumpKey() == TRUE && g_Player.jumppower > 0) {
				g_Player.jumppower -= (GRAVITY - (GRAVITY / 2));
			}
			else {
				g_Player.jumppower -= GRAVITY;
			}
		}

		g_Player.y -= g_Player.jumppower;
		if (IsHitCeil() == TRUE && g_Player.jumppower > 0) {
			g_Player.jumppower = 0;
		}

		if (IsHitGround() == TRUE && g_Player.jumppower <= 0) {
			g_Player.isjump = FALSE;
			g_Player.jumppower = 0;
		}

	}

	if (g_Player.isjump == FALSE && Buf[KEY_INPUT_SPACE] == 1 && g_Player.jumpOK == TRUE && IsHitGround()) {
		g_Player.isjump = TRUE;
		g_Player.jumpOK = FALSE;
		g_Player.jumppower = IsPushDashKey() ? DASH_JUMPPOWER : NORMAL_JUMPPOWER;
		g_Player.y -= g_Player.jumppower;
	}

	if (g_Player.isjump == FALSE && IsHitGround() == TRUE) {
		if (IsPushJumpKey() == FALSE) {
			g_Player.jumpOK = TRUE;
		}
	}
}


/***********************************************
 プレイヤー描画
 **********************************************/
void DrawPlayer(void)
{
	PlayerAnimControll();
	DrawRotaGraph(g_Player.x, g_Player.y - g_Player.issuper * BLOCKSIZE/2, 1.0f, 0, g_MarioImage[g_Player.status][g_Player.animnum], TRUE, g_Player.angle);
}

/***********************************************
 プレイヤーアニメーション制御
 **********************************************/
void PlayerAnimControll(void)
{
	static int anim;
	anim += abs((int)g_Player.mx);
	if (anim >= 44) {
		anim = 0;
	}
	g_Player.animnum = anim / 15;

	if ((int)g_Player.mx == 0) {
		g_Player.animnum = 0;
		anim = 0;
	}

	if ((g_Player.mx > 0 && Buf[KEY_INPUT_LEFT] != 0) || (g_Player.mx < 0 && Buf[KEY_INPUT_RIGHT] != 0)) {
		g_Player.animnum = 6;
		if( g_Player.issuper == TRUE){
			g_Player.animnum = 7;
		}
	}

	if (g_Player.isjump == TRUE) {
		g_Player.animnum = 4;
	}

	if( g_Player.issuper == TRUE && IsPushCrouchKey() == TRUE){
		g_Player.animnum = 6;
	}
}

/***********************************************
 ステージ描画
 **********************************************/
void DrawStage(void)
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x228fdc, TRUE);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = g_ScrollQuantity / BLOCKSIZE; j < g_ScrollQuantity / BLOCKSIZE + 21; j++) {
			if( i == g_HitBlockY && j == g_HitBlockX && g_BlockAnimFlg >= 1){
				if( g_BlockAnimFlg == 1){
					if ( --g_BlockAnimCnt == -BLOCKSIZE / 4) g_BlockAnimFlg = 2;
				}else if( g_BlockAnimFlg == 2){
					if ( ++g_BlockAnimCnt == 0) g_BlockAnimFlg = 0;
				}
				DrawGraph(j * BLOCKSIZE - g_ScrollQuantity, i * BLOCKSIZE + g_BlockAnimCnt, g_Mapchip[g_Stage[i][j]], TRUE);
			}else{
				DrawGraph(j * BLOCKSIZE - g_ScrollQuantity, i * BLOCKSIZE, g_Mapchip[g_Stage[i][j]], TRUE);
			}
		}
	}
}

/***********************************************
 ステージのスクロール
 **********************************************/
void StageScroll(void)
{
	if (g_ScrollQuantity <= (MAP_WIDTH - SCREEN_WIDTH / BLOCKSIZE) * BLOCKSIZE - 64) {
		if (g_Player.x >= 192 && g_Player.x < 240) {
			if( (int)g_Player.mx >= 0){
				g_ScrollQuantity += (int)g_Player.mx;
			}
		}
		else if (g_Player.x >= 240) {
			g_Player.x -= (int)g_Player.mx;
			if( (int)g_Player.mx >= 0){
				g_ScrollQuantity += (int)g_Player.mx;
			}
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

		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "左シフトキー:ダッシュ");
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "スペースキー:ジャンプ");

		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.x:%d", g_Player.x);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.y:%d", g_Player.y);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "スクロール:%d", g_ScrollQuantity);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "X:%d", (g_Player.x + g_ScrollQuantity) / BLOCKSIZE);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "Y:%d", (g_Player.y + (HITBOX / 2)) / BLOCKSIZE);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.mx:%.3f", g_Player.mx);

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

	if (LoadDivGraph("Image/backimage.png", 84, 12, 7, 32, 32, g_Mapchip) == -1) return -1;
	if (LoadDivGraph("Image/mario_chara.png", 15, 5, 3, 32, 32, g_MarioImage[0]) == -1) return -1;
	if (LoadDivGraph("Image/super_mario_chara.png", 15, 5, 3, 32, 64, g_MarioImage[1]) == -1) return -1;
	if (LoadDivGraph("Image/fire_mario_chara.png", 15, 5, 3, 32, 64, g_MarioImage[2]) == -1) return -1;

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
	FontSize[Size10] = CreateFontToHandle(NULL, 10, -1, DX_FONTTYPE_ANTIALIASING_EDGE);	//フォントデータをあらかじめ作成しておく
	FontSize[Size16] = CreateFontToHandle(NULL, 16, -1, DX_FONTTYPE_ANTIALIASING_EDGE);	//フォントデータをあらかじめ作成しておく
	FontSize[Size20] = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size25] = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size30] = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size35] = CreateFontToHandle(NULL, 35, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size40] = CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size48] = CreateFontToHandle(NULL, 48, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size100] = CreateFontToHandle(NULL, 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size200] = CreateFontToHandle(NULL, 200, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

}