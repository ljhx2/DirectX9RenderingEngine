#include "Common.h"
#include "include\D3DRenderer.h"
#include "cScriptComponent.h"

#include "aStarAlgorithm.h"


//��� ��ũ��Ʈ�� ������ �ִ� ���� ������Ʈ
ENGINE::GameObject* g_pGameObject = NULL;
ENGINE::cScriptComponent* g_pScriptComponent = NULL;

//aSar�˰��� ����
aStarAlgorithm* g_pASTAR_Algorithm = NULL;

//�׽�Ʈ �Լ�
int testFunction1(lua_State *L)
{
	int UID_1 = lua_tonumber(L, 1);
	std::string name1 = lua_tostring(L, 2);
	int UID_2 = lua_tonumber(L, 3);
	std::string name2 = lua_tostring(L, 4);
	ENGINE::GameObject* target1 = g_pScriptComponent->FindU_IDGameObject(UID_1);
	ENGINE::GameObject* target2 = g_pScriptComponent->FindU_IDGameObject(UID_2);
	
	return 0;
}
int testFunction2(lua_State *L)
{
	int UID_1 = lua_tonumber(L, 1);
	int UID_2 = lua_tonumber(L, 2);

	/*ENGINE::GameObject* target1 = g_pScriptComponent->FindU_IDGameObject(UID_1);
	ENGINE::GameObject* target2 = g_pScriptComponent->FindU_IDGameObject(UID_2);*/

	return 0;
}

//�Է¹��� ��ġ�� �ӵ���ŭ �����δ�
int DestinationMove(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();

	D3DXVECTOR3 destPos;
	double speed;

	//������ ����
	destPos.x = lua_tonumber(L, 1);
	destPos.y = lua_tonumber(L, 2);
	destPos.z = lua_tonumber(L, 3);
	//�̵��ӵ�
	speed = lua_tonumber(L, 4);

	D3DXVECTOR3 nextPos;
	g_pASTAR_Algorithm->NextMovePoint(destPos.x, destPos.z, myPos.x, myPos.z, nextPos.x, nextPos.z);
	nextPos.y = myPos.y;

	D3DXVECTOR3 dir;

	//���⺤�� ���ϱ�
	D3DXVECTOR3 temp = nextPos.operator-=(myPos);
	D3DXVec3Normalize(&dir, &temp);

	//���⿡ �°� y��ȸ�� ��Ű��
	D3DXVECTOR3 front = g_pGameObject->GetTransform()->GetLocalFront();

	float inner = D3DXVec2Dot(&D3DXVECTOR2(front.x, front.z), &D3DXVECTOR2(dir.x, dir.z));

	float seta = acos(inner);

	//ȸ������ �̼��ϸ� �� 0.0���� ���ش�
	if (seta < 0.05){
		//���� ��ġ�� ������
		if ((front.x >= 0 && dir.x >= 0) || (front.x < 0 && dir.x < 0))
			seta = 0.0;
		else seta = 0.05;
	}
	//���� ���Ͱ� 1��и� �ϋ�
	else if (front.x >= 0 && front.z >= 0){
		if (dir.x >= 0 && dir.z >= 0){//1��и�
			if (front.x <= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2��и�
			seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3��и�
			if (-front.x >= dir.x) seta = -0.05;
			else seta = 0.05;
		}
		else{
			seta = 0.05;
		}
	}
	//���� ���Ͱ� 2��и� �ϋ�
	else if (front.x < 0 && front.z >= 0){
		if (dir.x >= 0 && dir.z >= 0){//1��и�
			seta = 0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2��и�
			if (front.x <= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3��и�
			seta = -0.05;
		}
		else{
			if (-front.x >= dir.x) seta = -0.05;
			else seta = 0.05;
		}
	}
	//���� ���Ͱ� 3��и� �ϋ�
	else if (front.x < 0 && front.z < 0){
		if (dir.x >= 0 && dir.z >= 0){//1��и�
			if (-front.x >= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2��и�
			seta = 0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3��и�
			if (front.x <= dir.x) seta = -0.05;
			else seta = 0.05;
		}
		else{
			seta = -0.05;
		}
	}
	//���� ���Ͱ� 4��и� �ϋ�
	else {
		if (dir.x >= 0 && dir.z >= 0){//1��и�
			seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2��и�
			if (-front.x >= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3��и�
			seta = 0.05;
		}
		else{
			if (front.x <= dir.x) seta = -0.05;
			else seta = 0.05;
		}
	}
	g_pGameObject->GetTransform()->Rotate_World(0, seta, 0);
	
	//�̵�
	dir.operator*=(speed);
	g_pGameObject->GetTransform()->Translation_World(dir.x, dir.y, dir.z);

	return 0;
}
// �Է¹��� G_ID�� ���� ������Ʈ�� ���� �̵��� �Ѵ�. �Է¹��� G_ID�� ���� ������Ʈ�� ���ٸ� false, �ִٸ� true
int GameObjectToMove(lua_State *L)
{
	//��ǥ�� ������Ʈ �������̵�
	int U_ID = (int)lua_tonumber(L, 1);
	//�̵��ӵ�
	double speed = lua_tonumber(L, 2);

	ENGINE::GameObject* target = g_pScriptComponent->FindU_IDGameObject(U_ID);
	//���� ������Ʈ�� ���ų� �׾ �׸��� ������ false�� �ְ� �Լ��� ������
	if (target == NULL || target->GetIsEnable() == false){
		lua_pushboolean(L, false);
		return 1;
	}

	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();
	D3DXVECTOR3 targetPos = target->GetTranslate();

	D3DXVECTOR3 destPos; 
	g_pASTAR_Algorithm->NextMovePoint(targetPos.x, targetPos.z, myPos.x, myPos.z, destPos.x, destPos.z);
	destPos.y = myPos.y;

	D3DXVECTOR3 dir;

	//���⺤�� ���ϱ�
	D3DXVECTOR3 temp = destPos.operator-=(myPos);
	D3DXVec3Normalize(&dir, &temp);

	//���⿡ �°� y��ȸ�� ��Ű��
	D3DXVECTOR3 front = g_pGameObject->GetTransform()->GetLocalFront();

	float inner = D3DXVec2Dot(&D3DXVECTOR2(front.x, front.z), &D3DXVECTOR2(dir.x, dir.z));

	float seta = acos(inner);

	//ȸ������ �̼��ϸ� �� 0.0���� ���ش�
	if (seta < 0.05){
		//���� ��ġ�� ������
		if ((front.x >= 0 && dir.x >= 0) || (front.x < 0 && dir.x < 0))
			seta = 0.0;
		else seta = 0.05;
	}
	//���� ���Ͱ� 1��и� �ϋ�
	else if (front.x >= 0 && front.z >= 0){
		if (dir.x >= 0 && dir.z >= 0){//1��и�
			if (front.x <= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2��и�
			seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3��и�
			if (-front.x >= dir.x) seta = -0.05;
			else seta = 0.05;
		}
		else{
			seta = 0.05;
		}
	}
	//���� ���Ͱ� 2��и� �ϋ�
	else if (front.x < 0 && front.z >= 0){
		if (dir.x >= 0 && dir.z >= 0){//1��и�
			seta = 0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2��и�
			if (front.x <= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3��и�
			seta = -0.05;
		}
		else{
			if (-front.x >= dir.x) seta = -0.05;
			else seta = 0.05;
		}
	}
	//���� ���Ͱ� 3��и� �ϋ�
	else if (front.x < 0 && front.z < 0){
		if (dir.x >= 0 && dir.z >= 0){//1��и�
			if (-front.x >= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2��и�
			seta = 0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3��и�
			if (front.x <= dir.x) seta = -0.05;
			else seta = 0.05;
		}
		else{
			seta = -0.05;
		}
	}
	//���� ���Ͱ� 4��и� �ϋ�
	else {
		if (dir.x >= 0 && dir.z >= 0){//1��и�
			seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2��и�
			if (-front.x >= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3��и�
			seta = 0.05;
		}
		else{
			if (front.x <= dir.x) seta = -0.05;
			else seta = 0.05;
		}
	}
	g_pGameObject->GetTransform()->Rotate_World(0, seta, 0);
	/*
	if (seta < 0.05) seta = 0.0;

	if (front.x >= 0){
		if (front.x > dir.x) seta *= -1;
	}
	else{
		if (front.x <= dir.x) seta *= -1;
	}

	g_pGameObject->GetTransform()->Rotate_World(0, 0.1, 0);
	*/
	//�̵�
	dir.operator*=(speed);
	g_pGameObject->GetTransform()->Translation_World(dir.x, dir.y, dir.z);

	lua_pushboolean(L, true);

	return 1;
}
//�Է¹��� ���͸�ŭ �̵� ��Ų��(�ӵ�x)
int GameObjectAddMove(lua_State *L)
{
	D3DXVECTOR3 pos = g_pGameObject->GetTranslate();
	D3DXVECTOR3 moveValue;

	// lua statck ��ġ �̵���x,y,z���� �̾Ƴ���.
	moveValue.x = lua_tonumber(L, 1);
	moveValue.y = lua_tonumber(L, 2);
	moveValue.z = lua_tonumber(L, 3);

	pos.operator+=(moveValue);

	g_pGameObject->SetTranslate(pos.x, pos.y, pos.z);

	return 0;
}
//�Է¹��� ���͸�ŭ ȸ�� ��Ų��
int GameObjectAddRotation(lua_State *L)
{
	D3DXVECTOR3 rot = g_pGameObject->GetRotation();
	D3DXVECTOR3 rotValue;

	// lua statck ���� ȸ�� �̵��� x,y,z���� �̾Ƴ���.
	rotValue.x = lua_tonumber(L, 1);
	rotValue.y = lua_tonumber(L, 2);
	rotValue.z = lua_tonumber(L, 3);

	rot.operator+=(rotValue);

	g_pGameObject->SetRotation(rot.x, rot.y, rot.z);

	return 0;
}
//�Է¹��� ����ŭ front�������� �̵��Ѵ�
int GameObjectFrontMove(lua_State *L)/*(double speed)*/
{
	double speed = lua_tonumber(L, 1);
	g_pGameObject->GetTransform()->Translation_Self(0, 0, speed);
	return 0;
}
//�Է¹��� ����ŭ front������ �������� ȸ���Ѵ�
int GameObjectFrontRotation(lua_State *L)/*(double rot)*/
{
	double rotValue = lua_tonumber(L, 1);
	g_pGameObject->GetTransform()->Rotate_World(0, rotValue, 0);
	return 0;
}

//�ڱⰡ ������ �ִ� �ִ�Ŭ�� ������ �����Ѵ�
int GetAniSize(lua_State *L)
{
	ENGINE::cAnimComponent* pAniComPonent = NULL;

	//�ִ� ������Ʈ�� �ִٸ� �� �����͸� �޾ƿ´�
	int componentSize = g_pGameObject->GetComponentSize();

	for (int i = 0; i < componentSize; i++)
	{
		if (g_pGameObject->GetCompoent(i)->GetType() == ENGINE::eANIMATION_TYPE)
		{
			pAniComPonent = (ENGINE::cAnimComponent*)g_pGameObject->GetCompoent(i);
		}
	}
	if (pAniComPonent == NULL){
		//�ִ� ������Ʈ�� ������ϸ� 0 ����
		lua_pushnumber(L, 0);
		return 1;
	}

	//ani_size�� �������ش�
	lua_pushnumber(L, pAniComPonent->GetAniClipNum());

	return 1;
}
//�ִϸ��̼��� �����Ѵ�
int SetAni(lua_State *L)
{
	ENGINE::cAnimComponent* pAniComPonent = NULL;

	//�ִ� ������Ʈ�� �ִٸ� �� �����͸� �޾ƿ´�
	int componentSize = g_pGameObject->GetComponentSize();

	for (int i = 0; i < componentSize; i++)
	{
		if (g_pGameObject->GetCompoent(i)->GetType() == ENGINE::eANIMATION_TYPE)
		{
			pAniComPonent = (ENGINE::cAnimComponent*)g_pGameObject->GetCompoent(i);
		}
	}
	if (pAniComPonent == NULL) return 1;

	//��ũ��Ʈ���� �Է¹��� ������ �ִ� �ε����� �����´�
	std::string aniIndex = lua_tostring(L, 1);

	pAniComPonent->Play(aniIndex);

	return 0;
}
//�ִϸ��̼� �̺�Ʈ �ݹ� �Լ� ����
int SetAniEvnetFunction(lua_State *L)
{
	//�Լ��� ȣ��� �ִ� �ε����� �����´�
	std::string aniIndex = lua_tostring(L, 1);
	//�Լ��� ȣ��� �ִ� ������
	int frame = lua_tonumber(L, 2);
	//ȣ��� �Լ� Ÿ��
	int functionType = lua_tonumber(L, 3);

	ENGINE::cAnimComponent* anim = g_pGameObject->GetComponent<ENGINE::cAnimComponent*>(ENGINE::eANIMATION_TYPE);

	if (functionType == 1)
		anim->AddAnimCallback(aniIndex, frame, ENGINE::cScriptComponent::AnimationEvent1);
	else if (functionType == 2)
		anim->AddAnimCallback(aniIndex, frame, ENGINE::cScriptComponent::AnimationEvent2);
	else if (functionType == 3)
		anim->AddAnimCallback(aniIndex, frame, ENGINE::cScriptComponent::AnimationEvent3);
	else if (functionType == 4)
		anim->AddAnimCallback(aniIndex, frame, ENGINE::cScriptComponent::AnimationEvent4);
	else if (functionType == 5)
		anim->AddAnimCallback(aniIndex, frame, ENGINE::cScriptComponent::AnimationEvent5);
	else if (functionType == 6)
		anim->AddAnimCallback(aniIndex, frame, ENGINE::cScriptComponent::AnimationEvent6);

	return 0;
}



//�ڱ� ��ġ�� x���� �����Ѵ�
int GetPosX(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();

	lua_pushnumber(L, myPos.x);
	return 1;
}
//�ڱ� ��ġ�� y���� �����Ѵ�
int GetPosY(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();

	lua_pushnumber(L, myPos.y);
	return 1;
}
//�ڱ� ��ġ�� z���� �����Ѵ�
int GetPosZ(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();

	lua_pushnumber(L, myPos.z);
	return 1;
}

//�ڱ� ��ġ�� �����Ѵ�
int SetGameObjectPos(lua_State *L)
{
	D3DXVECTOR3 pos;


	// lua statck ��ġ�� x,y,z���� �̾Ƴ���.
	pos.x = lua_tonumber(L, 1);
	pos.y = lua_tonumber(L, 2);
	pos.z = lua_tonumber(L, 3);

	g_pGameObject->SetTranslate(pos.x, pos.y, pos.z);

	return 0;
}


//Ư�� �±� ��ü�� �ڱ�� ���� ����� ������Ʈ�� id�� �޴´�
int GetTagToUID_MostNearGameObject(lua_State *L)
{
	//�Է¹��� �±� �ѹ��� �޴´�
	int nTag = (int)lua_tonumber(L, 1);
	int sightRange = lua_tonumber(L, 2);

	ENGINE::GAMEOBJECT_TAG tag = (ENGINE::GAMEOBJECT_TAG)nTag;

	//ã�� ���� ������Ʈ���� ����
	int findObjSize = 0;
	//ã�� ���� ������Ʈ u_id �迭
	unsigned int UID_Arr[100];
	g_pScriptComponent->FindTagGameObjects(tag, UID_Arr, findObjSize);

	//���� ����� �±� ��ü�� ã�´�
	double mostNearDistance = 1000000.0;
	int mostNearUID = -1;
	for (int i = 0; i < findObjSize; i++){
		ENGINE::GameObject* obj = g_pScriptComponent->FindU_IDGameObject(UID_Arr[i]);
		//�׾ �ȱ׸��� �����̸� �Ѿ��
		bool isRender = obj->GetIsEnable();
		if (isRender == false)	continue;

		//�ڽ��� ��ġ
		D3DXVECTOR3 pos1 = g_pGameObject->GetTranslate();
		//�˻��� ���� ������Ʈ ��ġ
		D3DXVECTOR3 pos2 = obj->GetTranslate();

		double distance = g_pScriptComponent->GetDistance(pos1, pos2);

		//ó�� �Է¹ްų� ���� �Է� ���� ��ü���� �� ������ �ٲپ��ش�
		if (mostNearUID == -1 || distance < mostNearDistance){
			mostNearUID = UID_Arr[i];
			mostNearDistance = distance;
		}
	}

	//���� ����� ������Ʈ�� �þ� ��Ÿ� ���̸� -1�� �Ѱ��ش�
	if (sightRange <= mostNearDistance)
		lua_pushnumber(L, -1);
	else
		lua_pushnumber(L, mostNearUID);
	return 1;
}

//Ư�� ���� ������Ʈ�� ������ �Ÿ��� ���Ѵ�
int UID_GameObjectDistance(lua_State *L)
{
	//�Է¹��� ���� ������Ʈ u_id�� �����Ѵ�
	unsigned int id = (unsigned int)lua_tonumber(L, 1);
	ENGINE::GameObject* obj = g_pScriptComponent->FindU_IDGameObject(id);

	//�ڽ��� ��ġ
	D3DXVECTOR3 pos1 = g_pGameObject->GetTranslate();
	//�˻��� ���� ������Ʈ ��ġ
	D3DXVECTOR3 pos2 = obj->GetTranslate();
	
	double distance = g_pScriptComponent->GetDistance(pos1, pos2);

	//�Ÿ� ���� �������ش�
	lua_pushnumber(L, distance);

	return 1;
}
//�Է¹��� ��ġ�� �ڱ� ��ġ���� �Ÿ��� ���Ͽ� �����Ѵ�
int SCRIPT_GetDistance(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();
	D3DXVECTOR3 pos;

	// lua statck ���� �������� ����.
	pos.x = lua_tonumber(L, 1);
	pos.y = lua_tonumber(L, 2);
	pos.z = lua_tonumber(L, 3);

	double distance = sqrt(((myPos.x - pos.x) * (myPos.x - pos.x))
		+ ((myPos.y - pos.y) * (myPos.y - pos.y))
		+ ((myPos.z - pos.z) * (myPos.z - pos.z)));

	//�̵� ��� ���� �������ش�
	lua_pushnumber(L, distance);

	return 1;
}

//�Է¹��� ���̵� ���� ���� ������Ʈ�� �����ϴ��� ���� �˻�
int IsExistGameObject(lua_State *L)
{
	//�Է¹��� ���� ������Ʈ u_id�� �����Ѵ�
	unsigned int id = (unsigned int)lua_tonumber(L, 1);
	ENGINE::GameObject* obj = g_pScriptComponent->FindU_IDGameObject(id);
	
	//�����ϰ� ĳ���Ͱ� ���� �ʾƼ� �׷����� �ִ� �����̸�
	if (obj && obj->GetIsEnable() == true)	lua_pushboolean(L, true);
	//�ƴϸ�
	else									lua_pushboolean(L, false);

	return 1;
}

//Ÿ���� �� ������Ʈ�� OnDemage()�Լ��� ȣ���Ѵ�
int TargetObjectDemage(lua_State *L)
{
	int UID_TARGET = lua_tonumber(L, 1);
	int power = lua_tonumber(L, 2);

	ENGINE::GameObject* target = ENGINE::g_pGameObjectManager->FindGameObject(UID_TARGET);

	ENGINE::cScriptComponent* TargetScript = (ENGINE::cScriptComponent*)(target->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));

	//��� ������Ʈ �Լ� ȣ��
	lua_State* pLua = TargetScript->GetLua();
	lua_getglobal(pLua, "OnDemage");

	lua_pushnumber(pLua, power);

	lua_pcall(pLua, 1, 0, 0);
	return 0;
}

//���ӿ�����Ʈ ���� �Լ�
int DeleteObject(lua_State *L)
{
	g_pGameObject->OnDisable();
	return 0;
}
namespace ENGINE
{
	cScriptComponent::cScriptComponent()
	{
		eComponent_Type = eSCRIPT_TYPE;
	}
	cScriptComponent::cScriptComponent(GameObject* pGameObject, char* scriptPath)
	{
		eComponent_Type = eSCRIPT_TYPE;

		Init(pGameObject, scriptPath);
	}
	cScriptComponent::~cScriptComponent()
	{
		lua_close(m_Lua);

		if (g_pASTAR_Algorithm != NULL){
			delete g_pASTAR_Algorithm;
			g_pASTAR_Algorithm = NULL;
		}
	}
	void cScriptComponent::Init(GameObject* pGameObject, char* scriptPath)
	{
		g_pScriptComponent = this;
		// �Ű����� ����
		g_pGameObject = pGameObject;
		g_pGameObject->SetCallback_OnCollisionEnter(CollisionEvent);
		m_pGameObject = pGameObject;
		memset(m_scriptPath, 0, 256);
		memcpy(m_scriptPath, scriptPath, strlen(scriptPath));

		//��� ����
		if (m_Lua != NULL){
			lua_close(m_Lua);
			m_Lua = NULL;
		}
		m_Lua = luaL_newstate();

		//��ũ��Ʈ �ε�
		luaL_dofile(m_Lua, m_scriptPath);

		lua_register(m_Lua, "testFunction1", testFunction1);
		lua_register(m_Lua, "testFunction2", testFunction2);
		//��� �۷� �Լ� ����
		lua_register(m_Lua, "DestinationMove", DestinationMove);
		lua_register(m_Lua, "GameObjectToMove", GameObjectToMove);
		lua_register(m_Lua, "GameObjectAddMove", GameObjectAddMove);
		lua_register(m_Lua, "GameObjectAddRotation", GameObjectAddRotation);

		lua_register(m_Lua, "SCRIPT_GetDistance", SCRIPT_GetDistance);
		lua_register(m_Lua, "GetAniSize", GetAniSize);

		lua_register(m_Lua, "GetPosX", GetPosX);
		lua_register(m_Lua, "GetPosY", GetPosY);
		lua_register(m_Lua, "GetPosZ", GetPosZ);

		lua_register(m_Lua, "SetGameObjectPos", SetGameObjectPos);
		lua_register(m_Lua, "SetAni", SetAni);

		lua_register(m_Lua, "GetTagToUID_MostNearGameObject", GetTagToUID_MostNearGameObject);
		lua_register(m_Lua, "UID_GameObjectDistance", UID_GameObjectDistance);

		lua_register(m_Lua, "IsExistGameObject", IsExistGameObject);

		lua_register(m_Lua, "SetAniEvnetFunction", SetAniEvnetFunction);

		lua_register(m_Lua, "TargetObjectDemage", TargetObjectDemage);

		lua_register(m_Lua, "GameObjectFrontMove", GameObjectFrontMove);
		lua_register(m_Lua, "GameObjectFrontRotation", GameObjectFrontRotation);

		lua_register(m_Lua, "DeleteObject", DeleteObject);

		//��� �ʱ�ȭ �Լ� ȣ��
		lua_getglobal(m_Lua, "StartScript");
		lua_pcall(m_Lua, 0, 0, 0);


		//aSTAR����
		if (g_pASTAR_Algorithm == NULL){
			int** tailArr	= g_pWorldMapanager->GetArrWorldMapInfo();
			int tileNum		= g_pWorldMapanager->GetCellNum();
			int tileSize	= g_pWorldMapanager->GetCellSpacing();
			g_pASTAR_Algorithm = new aStarAlgorithm();
			g_pASTAR_Algorithm->Init(tailArr, tileNum, tileNum, tileSize);
		}
	}
	void cScriptComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		g_pScriptComponent = this;
		g_pGameObject = m_pGameObject;

		//��� ������Ʈ �Լ� ȣ��
		lua_getglobal(m_Lua, "UpdateScript");
		
		//�Ű����� ���� �־��ֱ�
		lua_pushnumber(m_Lua, deltaTime);

		lua_pcall(m_Lua, 1, 0, 0);
	}
	void cScriptComponent::PrevRender()
	{

	}
	void cScriptComponent::Render()
	{

	}

	/*
		��ũ��Ʈ �̺�Ʈ ȣ�� �Լ�
	*/
	void cScriptComponent::CollisionEvent(ENGINE::cCollisionShape* collider1, ENGINE::cCollisionShape* collider2)//collider1�� �ڱ� �ڽ� , collider2 ����浹ü
	{
		//�÷��̾� ���� �̸� �÷��̾� ��ũ��Ʈ�� �����Ѵ�
		if (collider1->GetName() == "playerSwordRH" || collider1->GetName() == "playerSwordLH")
		{
			g_pGameObject = collider1->GetOwnerGameObject()->GetParent();
			g_pScriptComponent = (cScriptComponent*)(g_pGameObject->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));
		}
		else{
			g_pGameObject = collider1->GetOwnerGameObject();
			g_pScriptComponent = (cScriptComponent*)(collider1->GetOwnerGameObject()->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));
		}
		
		//��� ������Ʈ �Լ� ȣ��
		if (g_pScriptComponent == NULL) return;

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "CollisionEvent");
		
		//�Ű����� ���� �־��ֱ�
		lua_pushnumber(pLua, g_pGameObject->GetID());
		lua_pushlstring(pLua, collider1->GetName().c_str(), strlen(collider1->GetName().c_str()));
		lua_pushnumber(pLua, collider2->GetOwnerGameObject()->GetID());
		lua_pushlstring(pLua, collider2->GetName().c_str(), strlen(collider2->GetName().c_str()));

		lua_pcall(pLua, 4, 0, 0);
	}
	void cScriptComponent::AnimationEvent1(ENGINE::GameObject* obj)
	{
		g_pGameObject = obj;
		g_pScriptComponent = (cScriptComponent*)(obj->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "AnimationEvent");

		lua_pushnumber(pLua, 1);

		lua_pcall(pLua, 1, 0, 0);
	}
	void cScriptComponent::AnimationEvent2(ENGINE::GameObject* obj)
	{
		g_pGameObject = obj;
		g_pScriptComponent = (cScriptComponent*)(obj->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "AnimationEvent");

		lua_pushnumber(pLua, 2);

		lua_pcall(pLua, 1, 0, 0);
	}
	void cScriptComponent::AnimationEvent3(ENGINE::GameObject* obj)
	{
		g_pGameObject = obj;
		g_pScriptComponent = (cScriptComponent*)(obj->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "AnimationEvent");

		lua_pushnumber(pLua, 3);

		lua_pcall(pLua, 1, 0, 0);
	}
	void cScriptComponent::AnimationEvent4(ENGINE::GameObject* obj)
	{
		g_pGameObject = obj;
		g_pScriptComponent = (cScriptComponent*)(obj->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "AnimationEvent");

		lua_pushnumber(pLua, 4);

		lua_pcall(pLua, 1, 0, 0);
	}
	void cScriptComponent::AnimationEvent5(ENGINE::GameObject* obj)
	{
		g_pGameObject = obj;
		g_pScriptComponent = (cScriptComponent*)(obj->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "AnimationEvent");

		lua_pushnumber(pLua, 5);

		lua_pcall(pLua, 1, 0, 0);
	}
	void cScriptComponent::AnimationEvent6(ENGINE::GameObject* obj)
	{
		g_pGameObject = obj;
		g_pScriptComponent = (cScriptComponent*)(obj->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "AnimationEvent");

		lua_pushnumber(pLua, 6);

		lua_pcall(pLua, 1, 0, 0);
	}
	void cScriptComponent::OnKeyInputEvent(std::string keyValue)
	{
		g_pGameObject = m_pGameObject;
		g_pScriptComponent = this;

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "OnKeyEvent");

		lua_pushlstring(pLua, keyValue.c_str(), strlen(keyValue.c_str()));

		lua_pcall(pLua, 1, 0, 0);
	}
	void cScriptComponent::OnMouseEvent(std::string eventType, D3DXVECTOR3 pos, int U_ID)
	{
		g_pGameObject = m_pGameObject;
		g_pScriptComponent = this;

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "OnMouseEvent");

		//���콺 �̺�Ʈ Ÿ�� ("enemyAttack", "move" ,"npTalk")
		lua_pushlstring(pLua, eventType.c_str(), strlen(eventType.c_str()));

		//���콺 3d��ǥ(x,
		lua_pushnumber(pLua, pos.x);
		lua_pushnumber(pLua, pos.y);
		lua_pushnumber(pLua, pos.z);

		//Ÿ���� move�� ��� ĳ���͸� Ŭ���Ѱ� �ƴϹǷ� -1
		//Ÿ���� enemyAttack�� ��� Ÿ�� enemy U_ID
		//Ÿ���� npTalk�� ��� npc U_ID
		lua_pushnumber(pLua, U_ID);

		lua_pcall(pLua, 5, 0, 0);
	}

	/*
		��� �Լ�
	*/
	GameObject* cScriptComponent::FindU_IDGameObject(unsigned int u_id)
	{
		return g_pGameObjectManager->FindGameObject(u_id);
	}
	void cScriptComponent::FindTagGameObjects(GAMEOBJECT_TAG tag,IN unsigned int* arr, OUT int& size)
	{
		g_pGameObjectManager->FindTagGameObjectsID(tag, arr, size);
	}
	/*
		��ƿ �Լ�
	*/
	double cScriptComponent::GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
	{
		double distance = sqrt(((pos1.x - pos2.x) * (pos1.x - pos2.x))
			+ ((pos1.y - pos2.y) * (pos1.y - pos2.y))
			+ ((pos1.z - pos2.z) * (pos1.z - pos2.z)));

		return distance;
	}
}
