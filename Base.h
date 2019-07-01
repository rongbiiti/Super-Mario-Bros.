#ifndef __BASE_H__
#define	__BASE_H__
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
#define MAX_GRAVITY 6			// 重力最大値
#define HITBOX	32				// マリオの当たり判定
#define BROKENRENGA_MAX 8		//壊れたレンガの最大表示数


/***********************************************
 * 列挙体の宣言
 ***********************************************/
enum GAME_MODE {
	GAME_TITLE,
	GAME_INIT,
	GAME_PREMAIN,		// メインに入る直前の残機表示画面
	GAME_MAIN,
	GAME_OVER,
	END = 99
};

enum FONTSIZE {
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

enum MARIO_STATUS {
	CHIBI,		// チビマリオ
	SUPER,		// スーパーマリオ
	FIRE,		// ファイアマリオ
	STAR		// 無敵
};

enum ENEMY {
	KURIBO,			// クリボー
	NOKONOKO,		// ノコノコ
	SHELL,			// カメのコウラ
	PAKKUN_FLOWER	// パックンフラワー
};

enum ITEM {
	SUPER_KINOKO,	// スーパーキノコ
	FIRE_FLOWER,	// ファイアフラワー
	SUPER_STAR,		// スーパースター
	ONEUP_KINOKO,	// 1UPキノコ
};

enum MAP_OBJECT {
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

enum INIT_STATUS {
	FIRST,				// 1-1から
	MISS,				// ミス
	CLEAR				// コースクリア
};

/***********************************************
 * 変数の宣言
 ***********************************************/
extern int g_ScrollQuantity;
 /***********************************************
  * ステージ配列の宣言
  ***********************************************/
extern int g_InitStage[MAP_HEIGHT][MAP_WIDTH];

extern int g_Stage[MAP_HEIGHT][MAP_WIDTH];

/***********************************************
 関数のプロトタイプ宣言
 **********************************************/
extern void InputKey(void);		// 0:入力されていない 1:入力された瞬間 2以上:入力されている

extern void DrawTitle(void);		// タイトル画面描画
extern void GameInit(void);		// 初期化処理
extern void StageInit(void);		// ステージ初期化処理
extern void DrawPreMain(void);		// メイン直前のマリオ残機数表示画面

extern void DrawMain(void);		// ゲームメイン
extern void PlayerControll(void);	// プレイヤー制御
extern void PlayerFall(void);		// プレイヤー落下
extern int IsBlock(int x, int y);	// その座標にブロックがあるか判定する
extern int IsHitBlockR(void);		// 右側ブロックとの当たり判定
extern int IsHitBlockL(void);		// 左側ブロックとの当たり判定
extern int IsHitGround(void);		// 地面との当たり判定
extern int IsHitCeil(void);		// 天井との当たり判定
extern void HitBlockMemo(int x, int y);	// 当たった天井のブロックの座標を記憶しておき、ブロックアニメフラグを立てる
extern int IsIntoHole(int y);		// 穴に落ちたか判定
extern void FixPlayerPosition(void);// プレイヤーが画面端から出ないようにする
extern bool IsPushDashKey(void);	// ダッシュキー押してるか判断
extern bool IsPushCrouchKey(void);	// しゃがみキー押してるか判断
extern bool IsPushJumpKey(void);	// ジャンプキー押してるか判断
extern void PlayerJump(void);		// ジャンプ処理

extern void DrawPlayer(void);		// プレイヤー描画
extern void PlayerAnimControll(void);	// プレイヤーアニメション制御

extern int IsItemHitBlockR(int x, int y);	// アイテムと右側ブロックとの当たり判定
extern int IsItemHitBlockL(int x, int y);	// アイテムと左側ブロックとの当たり判定
extern int IsItemHitGround(int x, int y);	// アイテムと地面との当たり判定

extern void DrawStage(void);		// ステージ描画
extern void InitBrokenRenga(int x, int y);	// 壊れたレンガのアニメ開始位置の初期化
extern void DrawBrokenRenga(void);	// 壊れたレンガの描画
extern void StageScroll(void);		// ステージのスクロール
extern void DrawUI(void);			// 画面上部のUI描画
extern void DrawDebugInfo(void);	// デバッグ用情報表示

extern void DrawGameOver(void);	// ゲームオーバー画面描画

extern int LoadImages(void);		// 画像データロード
extern int LoadSounds(void);		// 音データロード
extern void CreateFontData(void);	// フォントデータ作成



#endif // !__BASE_H__
