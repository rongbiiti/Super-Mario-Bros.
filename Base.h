#ifndef __BASE_H__
#define	__BASE_H__
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
#define FONTDATA_COUNT 10		// �t�H���g�f�[�^�̐�
#define SCREEN_WIDTH 640		// ��ʂ̕�
#define SCREEN_HEIGHT 512		// ��ʂ̍���
#define DRAW_WINDOW_WIDTH 1152	// �`�掞�̉�ʂ̕��B
#define DRAW_WINDOW_HEIGHT 864	// �`�掞�̉�ʂ̍���
#define MAP_WIDTH 221			// �}�b�v�̕�
#define MAP_HEIGHT 16			// �}�b�v�̍���
#define START_LIFE 3			// �������C�t
#define START_TIME 400			// �����c�莞��
#define NORMAL_SPEED 3			// �ړ����x
#define DASH_SPEED 5			// �_�b�V�����x
#define NORMAL_JUMPPOWER 18		// �W�����v���x
#define DASH_JUMPPOWER 18		// �_�b�V���W�����v���x
#define GRAVITY 2				// �d��
#define MAX_GRAVITY 6			// �d�͍ő�l
#define HITBOX	32				// �}���I�̓����蔻��
#define BROKENRENGA_MAX 8		//��ꂽ�����K�̍ő�\����


/***********************************************
 * �񋓑̂̐錾
 ***********************************************/
enum GAME_MODE {
	GAME_TITLE,
	GAME_INIT,
	GAME_PREMAIN,		// ���C���ɓ��钼�O�̎c�@�\�����
	GAME_MAIN,
	GAME_OVER,
	END = 99
};

enum FONTSIZE {
	Size10,
	Size16,				// �t�H���g�f�[�^�̔z��̓Y�����w��p�̗񋓒萔�̐錾
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
	CHIBI,		// �`�r�}���I
	SUPER,		// �X�[�p�[�}���I
	FIRE,		// �t�@�C�A�}���I
	STAR		// ���G
};

enum ENEMY {
	KURIBO,			// �N���{�[
	NOKONOKO,		// �m�R�m�R
	SHELL,			// �J���̃R�E��
	PAKKUN_FLOWER	// �p�b�N���t�����[
};

enum ITEM {
	SUPER_KINOKO,	// �X�[�p�[�L�m�R
	FIRE_FLOWER,	// �t�@�C�A�t�����[
	SUPER_STAR,		// �X�[�p�[�X�^�[
	ONEUP_KINOKO,	// 1UP�L�m�R
};

enum MAP_OBJECT {
	GROUND = 7,				// �n�ʃu���b�N
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

enum INIT_STATUS {
	FIRST,				// 1-1����
	MISS,				// �~�X
	CLEAR				// �R�[�X�N���A
};

/***********************************************
 * �ϐ��̐錾
 ***********************************************/
extern int g_ScrollQuantity;
 /***********************************************
  * �X�e�[�W�z��̐錾
  ***********************************************/
extern int g_InitStage[MAP_HEIGHT][MAP_WIDTH];

extern int g_Stage[MAP_HEIGHT][MAP_WIDTH];

/***********************************************
 �֐��̃v���g�^�C�v�錾
 **********************************************/
extern void InputKey(void);		// 0:���͂���Ă��Ȃ� 1:���͂��ꂽ�u�� 2�ȏ�:���͂���Ă���

extern void DrawTitle(void);		// �^�C�g����ʕ`��
extern void GameInit(void);		// ����������
extern void StageInit(void);		// �X�e�[�W����������
extern void DrawPreMain(void);		// ���C�����O�̃}���I�c�@���\�����

extern void DrawMain(void);		// �Q�[�����C��
extern void PlayerControll(void);	// �v���C���[����
extern void PlayerFall(void);		// �v���C���[����
extern int IsBlock(int x, int y);	// ���̍��W�Ƀu���b�N�����邩���肷��
extern int IsHitBlockR(void);		// �E���u���b�N�Ƃ̓����蔻��
extern int IsHitBlockL(void);		// �����u���b�N�Ƃ̓����蔻��
extern int IsHitGround(void);		// �n�ʂƂ̓����蔻��
extern int IsHitCeil(void);		// �V��Ƃ̓����蔻��
extern void HitBlockMemo(int x, int y);	// ���������V��̃u���b�N�̍��W���L�����Ă����A�u���b�N�A�j���t���O�𗧂Ă�
extern int IsIntoHole(int y);		// ���ɗ�����������
extern void FixPlayerPosition(void);// �v���C���[����ʒ[����o�Ȃ��悤�ɂ���
extern bool IsPushDashKey(void);	// �_�b�V���L�[�����Ă邩���f
extern bool IsPushCrouchKey(void);	// ���Ⴊ�݃L�[�����Ă邩���f
extern bool IsPushJumpKey(void);	// �W�����v�L�[�����Ă邩���f
extern void PlayerJump(void);		// �W�����v����

extern void DrawPlayer(void);		// �v���C���[�`��
extern void PlayerAnimControll(void);	// �v���C���[�A�j���V��������

extern int IsItemHitBlockR(int x, int y);	// �A�C�e���ƉE���u���b�N�Ƃ̓����蔻��
extern int IsItemHitBlockL(int x, int y);	// �A�C�e���ƍ����u���b�N�Ƃ̓����蔻��
extern int IsItemHitGround(int x, int y);	// �A�C�e���ƒn�ʂƂ̓����蔻��

extern void DrawStage(void);		// �X�e�[�W�`��
extern void InitBrokenRenga(int x, int y);	// ��ꂽ�����K�̃A�j���J�n�ʒu�̏�����
extern void DrawBrokenRenga(void);	// ��ꂽ�����K�̕`��
extern void StageScroll(void);		// �X�e�[�W�̃X�N���[��
extern void DrawUI(void);			// ��ʏ㕔��UI�`��
extern void DrawDebugInfo(void);	// �f�o�b�O�p���\��

extern void DrawGameOver(void);	// �Q�[���I�[�o�[��ʕ`��

extern int LoadImages(void);		// �摜�f�[�^���[�h
extern int LoadSounds(void);		// ���f�[�^���[�h
extern void CreateFontData(void);	// �t�H���g�f�[�^�쐬



#endif // !__BASE_H__
