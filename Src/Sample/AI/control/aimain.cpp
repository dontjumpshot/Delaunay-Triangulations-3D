
#include "stdafx.h"
#include "aimain.h"

using namespace ai;


// AI ���� ���� ���� �Լ�
void ai::Loop(const float deltaSeconds)
{
	cMessageManager::Get()->DispatchMsg();
}


// AI ���̺귯�� �ʱ�ȭ
void ai::Clear()
{
	cMessageManager::Release();
}
