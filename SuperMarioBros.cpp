/********************************************************************
** �`�[������F�X�[�p�[�}���I�u���U�[�Y�@����J�n�F2019/06/07
** ���G�g�i�y�������G�A���l�D�m�A�����Ė��A���c�H���j
********************************************************************/
#include "DxLib.h"
#include <stdlib.h>
#include <stdio.h>

/***********************************************
 * �萔�̐錾
 ***********************************************/
#define DEBUG 1					// 0���f�o�b�OOFF	1���f�o�b�OON
#define BLOCKSIZE 32			// 1�}�X�̑傫��
#define FONTDATA_COUNT 9		// �t�H���g�f�[�^�̐�
#define SCREEN_WIDTH 512		// ��ʂ̕�
#define SCREEN_HEIGHT 384		// ��ʂ̍���
#define DRAW_WINDOW_WIDTH 960	// �`�掞�̉�ʂ̕��B
#define DRAW_WINDOW_HEIGHT 720	// �`�掞�̉�ʂ̍���
#define START_LIFE 3			// �������C�t
#define START_TIME 400			// �����c�莞��
#define NORMAL_SPEED 3			// �ړ����x
#define DASH_SPEED 7			// �_�b�V�����x
#define NORMAL_JUMPPOWER 16		// �W�����v���x
#define DASH_JUMPPOWER 19		// �_�b�V���W�����v���x
#define GRAVITY 2				// �d��
#define MAX_GRAVITY 11			// �d�͍ő�l

/***********************************************
 * �񋓑̂̐錾
 ***********************************************/
typedef enum GAME_MODE{
	GAME_TITLE,
	GAME_INIT,
	GAME_PREMAIN,		// ���C���ɓ��钼�O�̎c�@�\�����
	GAME_MAIN,
	GAME_OVER,
	END = 99
};

typedef enum FONTSIZE{
	Size16,				// �t�H���g�f�[�^�̔z��̓Y�����w��p�̗񋓒萔�̐錾
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
	CHIBI,		// �`�r�}���I
	SUPER,		// �X�[�p�[�}���I
	FIRE,		// �t�@�C�A�}���I
	STAR		// ���G
};

typedef enum ENEMY{
	KURIBO,			// �N���{�[
	NOKONOKO,		// �m�R�m�R
	SHELL,			// �J���̃R�E��
	PAKKUN_FLOWER	// �p�b�N���t�����[
};

typedef enum ITEM{
	SUPER_KINOKO,	// �X�[�p�[�L�m�R
	FIRE_FLOWER,	// �t�@�C�A�t�����[
	SUPER_STAR,		// �X�[�p�[�X�^�[
	ONEUP_KINOKO,	// 1UP�L�m�R
};

typedef enum MAP_OBJECT{
	GROUND = 1,				// �n�ʃu���b�N
	RENGA_BLOCK,			// �����K�u���b�N
	TEN_COIN_BLOCK,			// 10�R�C���u���b�N
	HATENA_BLOCK,			// �n�e�i�u���b�N
	HIDDEN_BLOCK,			// �B���u���b�N
	EMPTY_BLOCK,			// �J���u���b�N
	HARD_BLOCK,				// �d���u���b�N
	COIN,					// �R�C��
	PIPE,					// �y��
	MIDDLE_POINT,			// ���ԃ|�C���g
	GOAL_POLE,				// �S�[���|�[��
	SPAWN_KURIBO,			// �N���{�[�o���ʒu
	SPAWN_NOKONOKO,			// �m�R�m�R�o���ʒu
	SPAWN_PAKKUN_FLOWER		// �p�b�N���t�����[�o���ʒu
};

typedef enum INIT_STATUS{
	FIRST,				// 1-1����
	MISS,				// �~�X
	CLEAR				// �R�[�X�N���A
};

/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int Buf[ 256 ] ;				// �L�[�{�[�h���͏��擾�̂��߂̕ϐ�
int FontSize[ FONTDATA_COUNT ];	// �t�H���g�f�[�^��ۑ����Ă����z��

int g_GameState = GAME_INIT;

int g_Score;
int g_Coin;
int g_Time;

int g_InitStatus;				// ���������ňꕔ�̕ϐ������������邩���f���邽�߂̕ϐ��B0:����A1:�~�X�A2:�N���A�B
bool g_IsThroughMiddlePoint;	// ���Ԓn�_��ʉ߂�����

/***********************************************
 * �\���̂̐錾
 ***********************************************/
struct PLAYER {
	int life;
	int x,y;		// ���W x,y
	bool angle;		// �����Ă�������B ��0��	��1�E
	bool isjump;	// �W�����v��:1
	int jumppower;	// �W�����v��
	int jumpcnt;
	bool isinv;		// ���G�t���O(0:�ʏ��� ,1:���G���)�B�G�ƏՓ˂����Ƃ��Ɏg��
	int invcnt;		// ���G��Ԃ̎c�莞��
};
struct PLAYER g_Player;

/***********************************************
 * �摜�f�[�^�p�ϐ��̐錾
 ***********************************************/
int g_Mapchip[84];		// �}�b�v�`�b�v�摜�i�[�p�z��

/***********************************************
 * ���f�[�^�p�ϐ��̐錾
 ***********************************************/


/***********************************************
 * �X�e�[�W�z��̐錾
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
 �֐��̃v���g�^�C�v�錾
 **********************************************/
void InputKey(void);		// 0:���͂���Ă��Ȃ� 1:���͂��ꂽ�u�� 2�ȏ�:���͂���Ă���

void DrawTitle(void);		// �^�C�g����ʕ`��
void GameInit(void);		// ����������
void DrawPreMain(void);		// ���C�����O�̃}���I�c�@���\�����

void DrawMain(void);		// �Q�[�����C��
void PlayerControll(void);	// �v���C���[����
bool IsPushDashKey(void);	// �_�b�V���L�[�����Ă邩���f
bool IsPushJumpKey(void);	// �W�����v�L�[�����Ă邩���f
void PlayerJump(void);		// �W�����v����
void DrawPlayer(void);		// �v���C���[�`��
void DrawStage(void);		// �X�e�[�W�`��
void DrawUI(void);			// ��ʏ㕔��UI�`��
void DrawDebugInfo(void);	// �f�o�b�O�p���\��

void DrawGameOver(void);	// �Q�[���I�[�o�[��ʕ`��

int LoadImages(void);		// �摜�f�[�^���[�h
int LoadSounds(void);		// ���f�[�^���[�h
void CreateFontData(void);	// �t�H���g�f�[�^�쐬

/***********************************************
 * �v���O�����̊J�n
 ***********************************************/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	SetMainWindowText( "Super Mario Bros." );

	SetGraphMode( DRAW_WINDOW_WIDTH , DRAW_WINDOW_HEIGHT , 32 );

	ChangeWindowMode( TRUE );

	if ( DxLib_Init() == -1 ) return -1;

	int offscreen_handle = MakeScreen( SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);

	SetDrawScreen( offscreen_handle );

	if ( LoadImages() == -1 ) return -1; // �摜�Ǎ��݊֐����Ăяo��
	if ( LoadSounds() == -1 ) return -1; // �T�E���h�Ǎ��݊֐����Ăяo��
	CreateFontData();

	// �Q�[�����[�v
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
	if( Buf[ KEY_INPUT_RETURN ] == 1){
		g_GameState = GAME_INIT;
	}

}

/***********************************************
 ����������
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
 ���C�����O�̃}���I�c�@���\�����
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
 �Q�[�����C��
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
 �v���C���[����
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
 �_�b�V���L�[�����Ă邩���f
 **********************************************/
bool IsPushDashKey(void)
{
	if( Buf[ KEY_INPUT_LSHIFT ] != 0){
		return TRUE;
	}
	return FALSE;
}

/***********************************************
 �W�����v�L�[�����Ă邩���f
 **********************************************/
bool IsPushJumpKey(void)
{
	if( Buf[ KEY_INPUT_SPACE ] != 0){
		return TRUE;
	}
	return FALSE;
}

/***********************************************
 �W�����v����
 **********************************************/
void PlayerJump(void)
{
	if( g_Player.isjump == TRUE){
		g_Player.y -= g_Player.jumppower;
		if( g_Player.jumppower > -MAX_GRAVITY){
			//�㏸�����W�����v�L�[�����Ă�Ԃ͉��H�ʂ�2����1�ɂ���
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
 �v���C���[�`��
 **********************************************/
void DrawPlayer(void)
{
	DrawCircle( BLOCKSIZE / 2 + g_Player.x, BLOCKSIZE / 2 + g_Player.y, BLOCKSIZE / 2, 0xDD3333);
}

/***********************************************
 �X�e�[�W�`��
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

		DrawFormatStringToHandle( 5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "���V�t�g�L�[:�_�b�V��");
		DrawFormatStringToHandle( 5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "�X�y�[�X�L�[:�W�����v");

		DrawFormatStringToHandle( 5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.x:%d", g_Player.x);
		DrawFormatStringToHandle( 5, 5 + i++ * 16, 0xFFFFFF, FontSize[Size16], "g_Player.y:%d", g_Player.y);

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

	if( LoadDivGraph("Image/backimage.png", 84, 12, 7, 32, 32, g_Mapchip) == -1) return -1;

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
	FontSize[Size16] = CreateFontToHandle( NULL, 16, -1, DX_FONTTYPE_ANTIALIASING_EDGE);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
	FontSize[Size20] = CreateFontToHandle( NULL, 20, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size25] = CreateFontToHandle( NULL, 25, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size30] = CreateFontToHandle( NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size35] = CreateFontToHandle( NULL, 35, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size40] = CreateFontToHandle( NULL, 40, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size48] = CreateFontToHandle( NULL, 48, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size50] = CreateFontToHandle( NULL, 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontSize[Size200] = CreateFontToHandle( NULL, 200, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

}