//================================================
//
// ui���� [ui.cpp]
// Author : �����D�I
//
//================================================

//================================================
// �C���N���[�h
//================================================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "keyboard.h"

//================================================
//�ÓI�����o�ϐ��錾
//================================================
LPDIRECT3DTEXTURE9 CUi::m_apTexture[UI_TYPE] = {};
int CUi::m_nUi = 0;

//================================================
//�N���G�C�g����
//================================================
CUi* CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UITYPE type)
{
	if (m_nUi <= 20)
	{
		// �I�u�W�F�N�g�𐶐�
		CUi* pUi = new CUi;

		// ����������
		pUi->Init(pos, size, type);

		m_nUi++;

		return pUi;
	}

	return NULL;
}

//================================================
//���[�h����
//================================================
HRESULT CUi::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���

	//���C�t�Q�[�W�o�[
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/LifeBar.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UITTYPE_LIFE]);

	//�^�C���̉�
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Time002.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UITYPE_TIME]);

	//�v���C���[����
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Player001.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UITYPE_PLAYER]);

	//�G�l�~�[����
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Enemy001.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UITYPE_ENEMY]);

	//�q�b�g������
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Hit.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UITYPE_HIT]);

	//��������
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Win.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UITYPE_WIN]);

	//��������
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/Lose1.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UITYPE_LOSE]);

	//�W��
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/Lockon001.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UITYPE_STANDARD]);


	//WINMARKFRAME
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/WinMarkFrame.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UYTYPE_WINMARKFRAME]);

	//WINMARK
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/WinMark.png", //�t�@�C���̓ǂݍ���
		&m_apTexture[UYTYPE_WINMARK]); 

		//WINMARK
		D3DXCreateTextureFromFile(pDevice,
			"Data/TEXTURE/Draw001.png", //�t�@�C���̓ǂݍ���
			&m_apTexture[UITYPE_DRAW]);

	return S_OK;
}

//================================================
//���[�h�̏I��
//================================================
void CUi::Unload(void)
{
	for (int nCount = 0; nCount < UI_TYPE; nCount++)
	{
		//�e�N�X�`���̊J��
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}

}

//================================================
//�R���X�g���N�^
//================================================
CUi::CUi()
{
	m_nPattern = 0;								// �p�^�[����
	m_nCounter = 0;								// �J�E���^	
	m_pos = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ���W
	m_move = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �ړ���
	m_size = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �T�C�Y
	m_type = UITYPE_NONE;						// �^�C�v
}

//================================================
//�f�X�g���N�^
//================================================
CUi::~CUi()
{
	m_nUi--;
}

//================================================
//����������
//================================================
HRESULT CUi::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, UITYPE type)
{
	m_pos = pos;	//�ʒu���
	m_size = size;	//�T�C�Y
	m_type = type;	//�^�C�v

	CScene2d::Init(pos,size);	//CScene2d�̏�����

	CScene2d::BindTexture(m_apTexture[type]);

	//�ʒu�ݒ�
	CScene2d::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	//�T�C�Y�ݒ�
	CScene2d::SetPolygonSize(D3DXVECTOR3(m_size.x, m_size.y, m_size.z));

	//�J���[�ݒ�
	CScene2d::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//================================================
//�I������
//================================================
void CUi::Uninit(void)
{
	CScene2d::Uninit();
	m_nUi--;
}

//================================================
//�X�V����
//================================================
void CUi::Update(void)
{
	CScene2d::Update();

	//Hit!!���ł��Ƃ�
	UiHit();

	// �L�[�{�[�h�X�V
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	//if (pKeyboard->GetTrigger(DIK_T))
	//{
	//	Create(D3DXVECTOR3(UI_RESULT_POS_LEFT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_WIN);
	//	Create(D3DXVECTOR3(UI_RESULT_POS_RIGHT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_LOSE);
	//}

	//if (pKeyboard->GetTrigger(DIK_Y))
	//{
	//	Create(D3DXVECTOR3(UI_RESULT_POS_RIGHT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_WIN);
	//	Create(D3DXVECTOR3(UI_RESULT_POS_LEFT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_LOSE);
	//}

	if (m_type == UITYPE_WIN || m_type == UITYPE_LOSE)
	{
		for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
		{
			bool bWinLose = CGame::GetPlayer(nCount)->GetSetWinLose();

			// �t���O�������Ȃ�����
			if (bWinLose == false)
			{
				Uninit();
				break;
			}
		}
	}

	CManager::MODE_TYPE mode = CManager::GetMode();

	if (mode == CManager::MODE_TYPE_TITLE)
	{
		// �I������
		Uninit();

		return;
	}

	//if (pKeyboard->GetTrigger(DIK_M))
	//{
	//	Create(D3DXVECTOR3(UI_WINMARK_POS_LEFT1_X, UI_WINMARK_POS_Y, 0.0f), D3DXVECTOR3(UI_WINMARK_SIZE_X, UI_WINMARK_SIZE_Y, 0.0f), CUi::UYTYPE_WINMARK);
	//	Create(D3DXVECTOR3(UI_WINMARK_POS_LEFT2_X, UI_WINMARK_POS_Y, 0.0f), D3DXVECTOR3(UI_WINMARK_SIZE_X, UI_WINMARK_SIZE_Y, 0.0f), CUi::UYTYPE_WINMARK);
	//	Create(D3DXVECTOR3(UI_WINMARK_POS_RIGHT1_X, UI_WINMARK_POS_Y, 0.0f), D3DXVECTOR3(UI_WINMARK_SIZE_X, UI_WINMARK_SIZE_Y, 0.0f), CUi::UYTYPE_WINMARK);
	//	Create(D3DXVECTOR3(UI_WINMARK_POS_RIGHT2_X, UI_WINMARK_POS_Y, 0.0f), D3DXVECTOR3(UI_WINMARK_SIZE_X, UI_WINMARK_SIZE_Y, 0.0f), CUi::UYTYPE_WINMARK);

	//}
}

//================================================
//�`�揈��
//================================================
void CUi::Draw(void)
{
	CScene2d::Draw();
}

//================================================
//�q�b�g�������ɏo��UI�̏���
//================================================
void CUi::UiHit(void)
{
	D3DXCOLOR col = GetCol();

	if (m_type == UITYPE_HIT)
	{
		m_nPattern++;
		m_nCounter++;

		if (m_nCounter == 3)
		{
			col.a = 1.0f;
		}
		else if (m_nCounter == 5)
		{
			col.a = 0.0f;
			m_nCounter = 0.0f;
		}

		if (m_nPattern == 30)
		{
			m_nPattern = 0;
			Uninit();
			return;
		}
	}

	SetCol(col);
}