#include "DxLib.h"
#include <stdlib.h>
#include <stdio.h>
#include "Base.h"

extern const float NORMAL_ACCEL = 0.1f;
extern const float DASH_ACCEL = 0.15f;
/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int Buf[256];				// �L�[�{�[�h���͏��擾�̂��߂̕ϐ�
int FontSize[FONTDATA_COUNT];	// �t�H���g�f�[�^��ۑ����Ă����z��

int g_GameState = GAME_INIT;

int g_ScrollQuantity;

int g_Score;
int g_Coin;
int g_Time;

int g_HitBlockX;	// ���������V��̃u���b�N��X���W
int g_HitBlockY;	// ���������V��̃u���b�N��Y���W
int g_BlockAnimCnt;	// ���������u���b�N���㉺�ɓ����Ƃ��̕ϐ�
int g_BlockAnimFlg;// �u���b�N���㉺�ɓ����Ƃ��̃t���O�B�����ĂȂ�0�A�㏸��1�A���~��2�B

int g_InitStatus;				// ���������ňꕔ�̕ϐ������������邩���f���邽�߂̕ϐ��B0:����A1:�~�X�A2:�N���A�B
bool g_IsThroughMiddlePoint;	// ���Ԓn�_��ʉ߂�����

/***********************************************
 * �\���̂̐錾
 ***********************************************/
struct PLAYER {
	int life;
	int status;
	int x, y;		// ���W x,y
	int px, py;		// �ړ��O���W
	float mx;
	bool angle;		// �����Ă�������B ��1��	��0�E
	bool isjump;	// �W�����v��:1
	int jumppower;	// �W�����v��
	bool jumpOK;
	bool isground;
	bool isinv;		// ���G�t���O(0:�ʏ��� ,1:���G���)�B�G�ƏՓ˂����Ƃ��Ɏg��
	int invcnt;		// ���G��Ԃ̎c�莞��
	int animnum;	// �A�j���[�V�����̔ԍ�
	bool issuper;	// �f�J�}���I��
	bool iscrouch;	// �f�J�}���I���ɂ��Ⴊ��ł��邩	
};																	
struct PLAYER g_Player;

struct BROKENRENGA {
	bool flg;
	int x, y;
	int initY;
	int jumppowerX;
	int jumppowerY;
	int animcnt;
	double angle;
};
struct BROKENRENGA g_BrokenRenga[BROKENRENGA_MAX];

/***********************************************
 * �摜�f�[�^�p�ϐ��̐錾
 ***********************************************/
int g_Mapchip[84];		// �}�b�v�`�b�v�摜�i�[�p�z��
int g_MarioImage[3][15];		// �}���I�L�����`�b�v�摜
int g_BrokenRengaImage[4];	//��ꂽ�����K�̉摜

/***********************************************
 * ���f�[�^�p�ϐ��̐錾
 ***********************************************/


 /***********************************************
  * �X�e�[�W�z��̐錾
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
											{-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,64,65,-1,-1,-1,-1,-1,-1,-1,-1,76,77,-1,-1,-1,-1,-1,-1,-1,76,77,-1,-1,14,15,16,-1,-1,-1,-1,-1,76,77,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,14,15,16,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,8,-1,-1,8,8,8,-1,-1,14,15,16,-1,8,8,8,8,-1,-1,8,8,8,-1,-1,-1,17,-1,64,65,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,64,65,-1,8,8,8,8,8,8,8,8,-1,-1,14,15,16,-1,-1,-1,34,-1,-1,-1,45,45,47,45,45,-1,-1,17,-1,-1},
											{14,15,16,-1,-1,-1,-1,-1,-1,-1,-1,-1,21,22,22,22,23,14,15,16,-1,-1,-1,-1,21,22,23,-1,-1,76,77,-1,-1,-1,-1,-1,-1,-1,-1,76,77,-1,-1,21,22,22,23,-1,76,77,-1,14,15,15,15,16,-1,-1,-1,-1,76,77,-1,21,22,22,22,23,14,15,16,-1,-1,-1,-1,-1,21,22,23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,21,22,22,23,-1,-1,-1,14,15,15,15,16,-1,-1,-1,-1,-1,-1,-1,21,22,22,22,23,14,15,16,-1,-1,-1,-1,-1,21,22,23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,8,8,8,22,22,8,8,8,8,14,15,15,15,8,8,8,8,8,-1,-1,8,8,8,8,23,14,15,16,76,77,-1,-1,-1,-1,21,22,23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,76,77,8,8,8,8,8,8,8,8,8,-1,14,15,15,15,16,-1,-1,8,-1,-1,-1,45,45,59,45,45,23,14,15,16,-1},
											{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},
											{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,-1,-1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7} };

int g_Stage[MAP_HEIGHT][MAP_WIDTH];


/***********************************************
 * �v���O�����̊J�n
 ***********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("Super Mario Bros.");

	SetGraphMode(DRAW_WINDOW_WIDTH, DRAW_WINDOW_HEIGHT, 32);

	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1) return -1;

	int offscreen_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);

	SetDrawScreen(offscreen_handle);

	if (LoadImages() == -1) return -1; // �摜�Ǎ��݊֐����Ăяo��
	if (LoadSounds() == -1) return -1; // �T�E���h�Ǎ��݊֐����Ăяo��
	CreateFontData();

	// �Q�[�����[�v
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
 * 0:���͂���Ă��Ȃ� 1:���͂��ꂽ�u�� 2�ȏ�:���͂���Ă���
 ***********************************************/
void InputKey(void)
{
	static char Key[256];
	GetHitKeyStateAll(Key);
	for (int i = 0; i < 256; i++) {
		if (Key[i]) Buf[i]++;		// ���͂��ꂽ�L�[�͉�����Ă���Ԃǂ�ǂ񐔎��������Ă����B
		else        Buf[i] = 0;		// �����ƈ�C��0�ɖ߂�
	}
}

/***********************************************
 �^�C�g����ʕ`��
 **********************************************/
void DrawTitle(void)
{
	if (Buf[KEY_INPUT_RETURN] == 1) {
		g_GameState = GAME_INIT;
	}

}

/***********************************************
 ����������
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

	for (int i = 0; i < BROKENRENGA_MAX; i++) {
		g_BrokenRenga[i].flg = FALSE;
		g_BrokenRenga[i].x = 0;
		g_BrokenRenga[i].y = 0;
		g_BrokenRenga[i].initY = 0;
		g_BrokenRenga[i].jumppowerX = 0;
		g_BrokenRenga[i].jumppowerY = 0;
		g_BrokenRenga[i].animcnt = 0;
		g_BrokenRenga[i].angle = 0;
	}

	StageInit();

	g_GameState = GAME_PREMAIN;
}

/***********************************************
 �X�e�[�W����������
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
 ���C�����O�̃}���I�c�@���\�����
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
 �Q�[�����C��
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
 �v���C���[����
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
 �v���C���[����
 **********************************************/
void PlayerFall(void)
{
	if (IsHitGround() == FALSE && g_Player.isjump == FALSE) {
		if (g_Player.jumppower > -MAX_GRAVITY) {
			g_Player.jumppower -= GRAVITY;
		}
		g_Player.y -= g_Player.jumppower;
		/*if (IsHitCeil() == TRUE && g_Player.jumppower > 0) {
			g_Player.isjump = FALSE;
			g_Player.jumppower = 0;
		}*/

		if (IsHitGround() == TRUE) {
			g_Player.y = (g_Player.y / BLOCKSIZE * BLOCKSIZE) + (BLOCKSIZE / 2) - 1;
			g_Player.isjump = FALSE;
		}
	}

}
/***********************************************
 ���̍��W���u���b�N�����肷��B
 �����F���肳�������L�����N�^�[��X�AY���W�B
 �Ԃ�l�F�u���b�N��������1�A�Ȃɂ��Ȃ����R�C���Ȃǂ̃A�C�e���Ȃ�0�B
 **********************************************/
int IsBlock(int x, int y)
{
	if ((g_Stage[y][x] >= 0 && g_Stage[y][x] <= 8) ||
		(g_Stage[y][x] >= 36 && g_Stage[y][x] <= 41) ||
		(g_Stage[y][x] >= 60 && g_Stage[y][x] <= 65) ||
		(g_Stage[y][x] >= 72 && g_Stage[y][x] <= 77)) {
		return 1;
	}
	return 0;
}
/***********************************************
 �u���b�N�Ƃ̓����蔻��
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
		g_Player.mx = 0;
		return 1;
	}

	int p_bottom = g_Player.y + HITBOX / 2;
	int m_bottom = p_bottom / BLOCKSIZE;

	if (IsBlock(w_x, m_bottom)) {
		if ((g_Player.x + HITBOX / 2 + 1) > (w_x * BLOCKSIZE - g_ScrollQuantity)) {
			g_Player.x -= (g_Player.x + HITBOX / 2 + 1) - (w_x * BLOCKSIZE - g_ScrollQuantity);
		}
		g_Player.mx = 0;
		return 1;
	}

	return 0;
}

/***********************************************
 �����u���b�N�Ƃ̓����蔻��
 **********************************************/
int IsHitBlockL(void)
{
	int p_left = g_Player.x - HITBOX / 2 - 1;
	int w_x = (p_left + g_ScrollQuantity) / BLOCKSIZE;
	int p_upper = (g_Player.y - HITBOX / 4) - (g_Player.issuper * BLOCKSIZE) + (g_Player.iscrouch * BLOCKSIZE);
	int m_upper = p_upper / BLOCKSIZE;

	if (IsBlock(w_x, m_upper)) {
		if (p_left > (w_x * BLOCKSIZE - g_ScrollQuantity)) {
			g_Player.x += (w_x * BLOCKSIZE - g_ScrollQuantity) - p_left+32;
		}
		g_Player.mx = 0;
		return 1;
	}

	int p_bottom = g_Player.y + HITBOX / 2;
	int m_bottom = p_bottom / BLOCKSIZE;

	if (IsBlock(w_x, m_bottom)) {
		if (p_left > (w_x * BLOCKSIZE - g_ScrollQuantity)) {
			g_Player.x += (w_x * BLOCKSIZE - g_ScrollQuantity) - p_left+32;
		}
		g_Player.mx = 0;
		return 1;
	}

	return 0;
}

/***********************************************
 �n�ʂƂ̓����蔻��
 **********************************************/
int IsHitGround(void)
{
	int p_bottom = g_Player.y + HITBOX / 2 + 1;
	int m_bottom = p_bottom / BLOCKSIZE;
	int p_left = g_Player.x - HITBOX / 2 + 1 - (int)g_Player.mx*2;
	int m_left = (p_left + g_ScrollQuantity) / BLOCKSIZE;

	if (IsBlock(m_left, m_bottom)) {
		if (p_bottom > m_bottom * BLOCKSIZE+-g_Player.jumppower/2) {
			g_Player.y -= (p_bottom - m_bottom * BLOCKSIZE)/2+2;
		}

		return 1;
	}

	int p_right = g_Player.x + HITBOX / 2 - 1;
	int m_right = (p_right + g_ScrollQuantity) / BLOCKSIZE;

	if (IsBlock(m_right, m_bottom)) {
		if (p_bottom > m_bottom * BLOCKSIZE+-g_Player.jumppower/2) {
			g_Player.y -= (p_bottom - m_bottom * BLOCKSIZE)/2+2;
		}

		return 1;
	}

	return 0;
}

/***********************************************
 �V��Ƃ̓����蔻��
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
				if (g_Stage[m_upper][m_left] == 5 && g_Player.issuper == TRUE) {
					InitBrokenRenga(m_left, m_upper);
				}
				else if (g_Stage[m_upper][m_left] == 1) {
					g_Stage[m_upper][m_left] = 0;
				}
				return 1;
			}
			g_Player.x += (m_left * BLOCKSIZE + 32 - (g_Player.x + g_ScrollQuantity)) - 16 + (int)g_Player.mx * 2;
			return 0;
		}
		HitBlockMemo(m_left, m_upper);
		if (g_Stage[m_upper][m_left] == 5 && g_Player.issuper == TRUE) {
			InitBrokenRenga(m_left, m_upper);
		}
		else if (g_Stage[m_upper][m_left] == 1) {
			g_Stage[m_upper][m_left] = 0;
		}
		return 1;
	}


	if (IsBlock(m_right, m_upper)) {
		if (g_Player.x + g_ScrollQuantity - 2 + (int)g_Player.mx <= m_right * BLOCKSIZE) {
			p_right = g_Player.x - (((g_Player.x + g_ScrollQuantity) - m_right * BLOCKSIZE) + 5 + (int)g_Player.mx * 2);
			m_right = (p_right + g_ScrollQuantity) / BLOCKSIZE;
			if (IsBlock(m_right, m_upper)) {
				HitBlockMemo(m_right, m_upper);
				if (g_Stage[m_upper][m_right] == 5 && g_Player.issuper == TRUE) {
					InitBrokenRenga(m_right, m_upper);
				}
				else if (g_Stage[m_upper][m_right] == 1) {
					g_Stage[m_upper][m_right] = 0;
				}
				return 1;
			}
			g_Player.x -= ((g_Player.x + g_ScrollQuantity) - m_right * BLOCKSIZE) - 16 + (int)g_Player.mx * 2;
			return 0;
		}
		HitBlockMemo(m_right, m_upper);
		if (g_Stage[m_upper][m_right] == 5 && g_Player.issuper == TRUE) {
			InitBrokenRenga(m_right, m_upper);
		}
		else if (g_Stage[m_upper][m_right] == 1) {
			g_Stage[m_upper][m_right] = 0;
		}
		return 1;
	}

	return 0;
}

/***********************************************
 �@�����u���b�N�̏ꏊ���L��
 **********************************************/
void HitBlockMemo(int x, int y)
{
	g_HitBlockX = x;
	g_HitBlockY = y;
	if ( g_Stage[g_HitBlockY][g_HitBlockX] == 1 || 
		(g_Stage[g_HitBlockY][g_HitBlockX] == 5 && g_Player.issuper == FALSE) ) {
		g_BlockAnimFlg = 1;
		g_BlockAnimCnt = 0;
	}
	
}
/***********************************************
 ���ɗ�����������
 �����FY���W
 �Ԃ�l�F�����Ă�����1
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
 �A�C�e���ƉE���u���b�N�̓����蔻��
 �����F�A�C�e����X�AY���W
 �Ԃ�l�F�u���b�N�ƏՓ˂��Ă�����1�A�łȂ����0
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
 �A�C�e���ƍ����u���b�N�̓����蔻��
 �����F�A�C�e����X�AY���W
 �Ԃ�l�F�u���b�N�ƏՓ˂��Ă�����1�A�łȂ����0
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
 �A�C�e���ƒn�ʂ̓����蔻��
 �����F�A�C�e����X�AY���W
 �Ԃ�l�F�u���b�N�ƏՓ˂��Ă�����1�A�łȂ����0
 **********************************************/
int IsItemHitGround(int x, int y)
{
	int p_bottom = y + HITBOX / 2 + 1;
	int m_bottom = p_bottom / BLOCKSIZE;
	int p_left = x - HITBOX / 2 + 1;
	int m_left = (p_left + g_ScrollQuantity) / BLOCKSIZE;

	if (IsBlock(m_left, m_bottom)) {
		if (p_bottom > m_bottom * BLOCKSIZE-3) {
			y -= p_bottom - m_bottom * BLOCKSIZE;
		}

		return 1;
	}

	int p_right = x + HITBOX / 2 - 1;
	int m_right = (p_right + g_ScrollQuantity) / BLOCKSIZE;

	if (IsBlock(m_right, m_bottom)) {
		if (p_bottom > m_bottom * BLOCKSIZE-10) {
			y -= p_bottom - m_bottom * BLOCKSIZE;
		}

		return 1;
	}

	return 0;
}

/***********************************************
 �v���C���[����ʒ[����o�Ȃ��悤�ɂ���
 **********************************************/
void FixPlayerPosition(void)
{
	if (g_Player.x - HITBOX / 2 <= 0) g_Player.x = 0 + HITBOX / 2;
	if (g_Player.x + HITBOX / 2 >= SCREEN_WIDTH) g_Player.x = SCREEN_WIDTH - HITBOX / 2;
}
/***********************************************
 �_�b�V���L�[�����Ă邩���f
 **********************************************/
bool IsPushDashKey(void)
{
	if (Buf[KEY_INPUT_LSHIFT] != 0) {
		return TRUE;
	}
	return FALSE;
}

/***********************************************
 ���Ⴊ�݃L�[�����Ă邩���f
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
 �W�����v�L�[�����Ă邩���f
 **********************************************/
bool IsPushJumpKey(void)
{
	if (Buf[KEY_INPUT_SPACE] != 0) {
		return TRUE;
	}
	return FALSE;
}

/***********************************************
 �W�����v����
 **********************************************/
void PlayerJump(void)
{
	if (g_Player.isjump == TRUE) {
		if (g_Player.jumppower > -MAX_GRAVITY) {
			//�㏸�����W�����v�L�[�����Ă�Ԃ͉��H�ʂ�2����1�ɂ���
			if (IsPushJumpKey() == TRUE && g_Player.jumppower > 0) {
				g_Player.jumppower -= (GRAVITY - (GRAVITY / 2));
			}
			else {
				g_Player.jumppower -= GRAVITY;
			}
		}

		g_Player.y -= g_Player.jumppower;
		if (IsHitCeil() == TRUE && g_Player.jumppower > 0) {
			g_Player.jumppower = -5;
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
 �v���C���[�`��
 **********************************************/
void DrawPlayer(void)
{
	PlayerAnimControll();
	DrawRotaGraph(g_Player.x, g_Player.y - g_Player.issuper * BLOCKSIZE/2, 1.0f, 0, g_MarioImage[g_Player.status][g_Player.animnum], TRUE, g_Player.angle);
}

/***********************************************
 �v���C���[�A�j���[�V��������
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
		if( Buf[ KEY_INPUT_RIGHT ] != 0 || Buf [ KEY_INPUT_LEFT ] != 0){
			anim += 2;
		}else{
		g_Player.animnum = 0;
		anim = 0;
		}
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
 �X�e�[�W�`��
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
	DrawBrokenRenga();
}
/***********************************************
 ��ꂽ�����K�̃A�j���J�n�ʒu�̏�����
 **********************************************/
void InitBrokenRenga(int x, int y)
{
	bool flg = false;
	for (int i = 0; i < BROKENRENGA_MAX; i++) {
		if (g_BrokenRenga[i].flg == FALSE) {
			g_BrokenRenga[i].flg = TRUE;
			g_BrokenRenga[i].x = x * BLOCKSIZE;
			g_BrokenRenga[i].y = y * BLOCKSIZE;
			g_BrokenRenga[i].initY = y * BLOCKSIZE;
			g_BrokenRenga[i].jumppowerX = 0;
			g_BrokenRenga[i].jumppowerY = 8;
			g_Stage[y][x] = -1;
			g_BrokenRenga[i].animcnt = 0;
			g_BrokenRenga[i].angle = 0;
			flg = true;
		}
		if (flg == true) break;
	}
}

/***********************************************
 ��ꂽ�����K�̕`��
 **********************************************/
void DrawBrokenRenga(void)
{
	for( int i=0; i<BROKENRENGA_MAX; i++){
		if (g_BrokenRenga[i].flg == TRUE) {
			DrawRotaGraph(g_BrokenRenga[i].x - g_ScrollQuantity - g_BrokenRenga[i].jumppowerX, g_BrokenRenga[i].y - 32, 1, g_BrokenRenga[i].angle, g_BrokenRengaImage[0], TRUE);
			DrawRotaGraph(g_BrokenRenga[i].x - g_ScrollQuantity + BLOCKSIZE / 2 + g_BrokenRenga[i].jumppowerX, g_BrokenRenga[i].y - 32, 1, g_BrokenRenga[i].angle, g_BrokenRengaImage[1], TRUE);
			DrawRotaGraph(g_BrokenRenga[i].x - g_ScrollQuantity - g_BrokenRenga[i].jumppowerX, g_BrokenRenga[i].y + BLOCKSIZE / 2, 1, g_BrokenRenga[i].angle, g_BrokenRengaImage[2], TRUE);
			DrawRotaGraph(g_BrokenRenga[i].x - g_ScrollQuantity + BLOCKSIZE / 2 + g_BrokenRenga[i].jumppowerX, g_BrokenRenga[i].y + BLOCKSIZE / 2, 1, g_BrokenRenga[i].angle, g_BrokenRengaImage[3], TRUE);

			g_BrokenRenga[i].y -= g_BrokenRenga[i].jumppowerY;
			g_BrokenRenga[i].jumppowerX += 2;

			if (g_BrokenRenga[i].jumppowerY > -MAX_GRAVITY) {
				--g_BrokenRenga[i].jumppowerY;
			}
			if (g_BrokenRenga[i].y - g_BrokenRenga[i].initY > SCREEN_HEIGHT) {
				g_BrokenRenga[i].flg = FALSE;
			}
			if (++g_BrokenRenga[i].animcnt > 30) {
				g_BrokenRenga[i].angle = (180 * 3.14) / 180;
			}
			else if (g_BrokenRenga[i].animcnt > 60) {
				g_BrokenRenga[i].animcnt = 0;
				g_BrokenRenga[i].angle = 0;
			}
		}
	}
}
/***********************************************
 �X�e�[�W�̃X�N���[��
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
 ��ʏ㕔��UI�`��
 **********************************************/
void DrawUI(void)
{

}

/***********************************************
 �f�o�b�O�p���\��
 **********************************************/
void DrawDebugInfo(void)
{
	bool IsDrawEnd = 0;
	int i = 0;
	while (IsDrawEnd == 0)

	{
		//DrawFormat...1�����R�s���� " " �̒��g��ς��邾���Ō�������������܂��B

		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "���V�t�g�L�[:�_�b�V��");
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "�X�y�[�X�L�[:�W�����v");

		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.x:%d", g_Player.x);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.y:%d", g_Player.y);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "�X�N���[��:%d", g_ScrollQuantity);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "X:%d", (g_Player.x + g_ScrollQuantity) / BLOCKSIZE);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "Y:%d", (g_Player.y + (HITBOX / 2)) / BLOCKSIZE);
		DrawFormatStringToHandle(5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.mx:%.3f", g_Player.mx);

		IsDrawEnd = 1;
	}
}

/***********************************************
 �Q�[���I�[�o�[��ʕ`��
 **********************************************/
void DrawGameOver(void)
{

}

/***********************************************
 �摜�f�[�^���[�h ���摜�t�H���_�̖��O�́uImage�v�ł��B
 **********************************************/
int LoadImages(void)
{
	//if((g_PlayerImage[Wait] = LoadGraph("Image/player normal.png")) == -1) return -1;

	if (LoadDivGraph("Image/backimage.png", 84, 12, 7, 32, 32, g_Mapchip) == -1) return -1;
	if (LoadDivGraph("Image/BrokenRenga.png", 4, 2, 2, 16, 16, g_BrokenRengaImage) == -1) return -1;
	if (LoadDivGraph("Image/mario_chara.png", 15, 5, 3, 32, 32, g_MarioImage[0]) == -1) return -1;
	if (LoadDivGraph("Image/super_mario_chara.png", 15, 5, 3, 32, 64, g_MarioImage[1]) == -1) return -1;
	if (LoadDivGraph("Image/fire_mario_chara.png", 15, 5, 3, 32, 64, g_MarioImage[2]) == -1) return -1;

	return 0;
}

/***********************************************
 ���f�[�^���[�h �����t�H���_�̖��O�́uSound�v�ł��B
 **********************************************/
int LoadSounds(void)
{
	//if((s_Desision = LoadSoundMem("Sound/kettei_kin.mp3")) == -1) return -1;

	return 0;
}

/***********************************************
 �t�H���g�f�[�^�쐬
 **********************************************/
void CreateFontData(void)
{
	FontSize[Size10] = CreateFontToHandle(NULL, 10, -1, DX_FONTTYPE_ANTIALIASING_EDGE);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
	FontSize[Size16] = CreateFontToHandle(NULL, 16, -1, DX_FONTTYPE_ANTIALIASING_EDGE);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
	FontSize[Size20] = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size25] = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size30] = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size35] = CreateFontToHandle(NULL, 35, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size40] = CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size48] = CreateFontToHandle(NULL, 48, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size100] = CreateFontToHandle(NULL, 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size200] = CreateFontToHandle(NULL, 200, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

}