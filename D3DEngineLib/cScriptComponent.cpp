#include "Common.h"
#include "include\D3DRenderer.h"
#include "cScriptComponent.h"

#include "aStarAlgorithm.h"


//루아 스크립트를 가지고 있는 게임 오브젝트
ENGINE::GameObject* g_pGameObject = NULL;
ENGINE::cScriptComponent* g_pScriptComponent = NULL;

//aSar알고리즘 변수
aStarAlgorithm* g_pASTAR_Algorithm = NULL;

//테스트 함수
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

//입력받은 위치로 속도만큼 움직인다
int DestinationMove(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();

	D3DXVECTOR3 destPos;
	double speed;

	//도착할 지점
	destPos.x = lua_tonumber(L, 1);
	destPos.y = lua_tonumber(L, 2);
	destPos.z = lua_tonumber(L, 3);
	//이동속도
	speed = lua_tonumber(L, 4);

	D3DXVECTOR3 nextPos;
	g_pASTAR_Algorithm->NextMovePoint(destPos.x, destPos.z, myPos.x, myPos.z, nextPos.x, nextPos.z);
	nextPos.y = myPos.y;

	D3DXVECTOR3 dir;

	//방향벡터 구하기
	D3DXVECTOR3 temp = nextPos.operator-=(myPos);
	D3DXVec3Normalize(&dir, &temp);

	//방향에 맞게 y축회전 시키기
	D3DXVECTOR3 front = g_pGameObject->GetTransform()->GetLocalFront();

	float inner = D3DXVec2Dot(&D3DXVECTOR2(front.x, front.z), &D3DXVECTOR2(dir.x, dir.z));

	float seta = acos(inner);

	//회전값이 미세하면 걍 0.0으로 해준다
	if (seta < 0.05){
		//같은 위치에 있으면
		if ((front.x >= 0 && dir.x >= 0) || (front.x < 0 && dir.x < 0))
			seta = 0.0;
		else seta = 0.05;
	}
	//방향 벡터가 1사분면 일떄
	else if (front.x >= 0 && front.z >= 0){
		if (dir.x >= 0 && dir.z >= 0){//1사분면
			if (front.x <= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2사분면
			seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3사분면
			if (-front.x >= dir.x) seta = -0.05;
			else seta = 0.05;
		}
		else{
			seta = 0.05;
		}
	}
	//방향 벡터가 2사분면 일떄
	else if (front.x < 0 && front.z >= 0){
		if (dir.x >= 0 && dir.z >= 0){//1사분면
			seta = 0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2사분면
			if (front.x <= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3사분면
			seta = -0.05;
		}
		else{
			if (-front.x >= dir.x) seta = -0.05;
			else seta = 0.05;
		}
	}
	//방향 벡터가 3사분면 일떄
	else if (front.x < 0 && front.z < 0){
		if (dir.x >= 0 && dir.z >= 0){//1사분면
			if (-front.x >= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2사분면
			seta = 0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3사분면
			if (front.x <= dir.x) seta = -0.05;
			else seta = 0.05;
		}
		else{
			seta = -0.05;
		}
	}
	//방향 벡터가 4사분면 일떄
	else {
		if (dir.x >= 0 && dir.z >= 0){//1사분면
			seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2사분면
			if (-front.x >= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3사분면
			seta = 0.05;
		}
		else{
			if (front.x <= dir.x) seta = -0.05;
			else seta = 0.05;
		}
	}
	g_pGameObject->GetTransform()->Rotate_World(0, seta, 0);
	
	//이동
	dir.operator*=(speed);
	g_pGameObject->GetTransform()->Translation_World(dir.x, dir.y, dir.z);

	return 0;
}
// 입력받은 G_ID의 게임 오브젝트를 향해 이동을 한다. 입력받은 G_ID를 가진 오브젝트가 없다면 false, 있다면 true
int GameObjectToMove(lua_State *L)
{
	//목표인 오브젝트 고유아이디
	int U_ID = (int)lua_tonumber(L, 1);
	//이동속도
	double speed = lua_tonumber(L, 2);

	ENGINE::GameObject* target = g_pScriptComponent->FindU_IDGameObject(U_ID);
	//얻어온 오브젝트가 없거나 죽어서 그리지 않으면 false를 주고 함수를 끝낸다
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

	//방향벡터 구하기
	D3DXVECTOR3 temp = destPos.operator-=(myPos);
	D3DXVec3Normalize(&dir, &temp);

	//방향에 맞게 y축회전 시키기
	D3DXVECTOR3 front = g_pGameObject->GetTransform()->GetLocalFront();

	float inner = D3DXVec2Dot(&D3DXVECTOR2(front.x, front.z), &D3DXVECTOR2(dir.x, dir.z));

	float seta = acos(inner);

	//회전값이 미세하면 걍 0.0으로 해준다
	if (seta < 0.05){
		//같은 위치에 있으면
		if ((front.x >= 0 && dir.x >= 0) || (front.x < 0 && dir.x < 0))
			seta = 0.0;
		else seta = 0.05;
	}
	//방향 벡터가 1사분면 일떄
	else if (front.x >= 0 && front.z >= 0){
		if (dir.x >= 0 && dir.z >= 0){//1사분면
			if (front.x <= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2사분면
			seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3사분면
			if (-front.x >= dir.x) seta = -0.05;
			else seta = 0.05;
		}
		else{
			seta = 0.05;
		}
	}
	//방향 벡터가 2사분면 일떄
	else if (front.x < 0 && front.z >= 0){
		if (dir.x >= 0 && dir.z >= 0){//1사분면
			seta = 0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2사분면
			if (front.x <= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3사분면
			seta = -0.05;
		}
		else{
			if (-front.x >= dir.x) seta = -0.05;
			else seta = 0.05;
		}
	}
	//방향 벡터가 3사분면 일떄
	else if (front.x < 0 && front.z < 0){
		if (dir.x >= 0 && dir.z >= 0){//1사분면
			if (-front.x >= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2사분면
			seta = 0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3사분면
			if (front.x <= dir.x) seta = -0.05;
			else seta = 0.05;
		}
		else{
			seta = -0.05;
		}
	}
	//방향 벡터가 4사분면 일떄
	else {
		if (dir.x >= 0 && dir.z >= 0){//1사분면
			seta = -0.05;
		}
		else if (dir.x < 0 && dir.z >= 0){//2사분면
			if (-front.x >= dir.x) seta = 0.05;
			else seta = -0.05;
		}
		else if (dir.x < 0 && dir.z < 0){//3사분면
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
	//이동
	dir.operator*=(speed);
	g_pGameObject->GetTransform()->Translation_World(dir.x, dir.y, dir.z);

	lua_pushboolean(L, true);

	return 1;
}
//입력받은 벡터만큼 이동 시킨다(속도x)
int GameObjectAddMove(lua_State *L)
{
	D3DXVECTOR3 pos = g_pGameObject->GetTranslate();
	D3DXVECTOR3 moveValue;

	// lua statck 위치 이동값x,y,z값을 뽑아낸다.
	moveValue.x = lua_tonumber(L, 1);
	moveValue.y = lua_tonumber(L, 2);
	moveValue.z = lua_tonumber(L, 3);

	pos.operator+=(moveValue);

	g_pGameObject->SetTranslate(pos.x, pos.y, pos.z);

	return 0;
}
//입력받은 벡터만큼 회전 시킨다
int GameObjectAddRotation(lua_State *L)
{
	D3DXVECTOR3 rot = g_pGameObject->GetRotation();
	D3DXVECTOR3 rotValue;

	// lua statck 에서 회전 이동값 x,y,z값을 뽑아낸다.
	rotValue.x = lua_tonumber(L, 1);
	rotValue.y = lua_tonumber(L, 2);
	rotValue.z = lua_tonumber(L, 3);

	rot.operator+=(rotValue);

	g_pGameObject->SetRotation(rot.x, rot.y, rot.z);

	return 0;
}
//입력받은 값만큼 front방향으로 이동한다
int GameObjectFrontMove(lua_State *L)/*(double speed)*/
{
	double speed = lua_tonumber(L, 1);
	g_pGameObject->GetTransform()->Translation_Self(0, 0, speed);
	return 0;
}
//입력받은 값만큼 front방향을 기준으로 회전한다
int GameObjectFrontRotation(lua_State *L)/*(double rot)*/
{
	double rotValue = lua_tonumber(L, 1);
	g_pGameObject->GetTransform()->Rotate_World(0, rotValue, 0);
	return 0;
}

//자기가 가지고 있는 애니클립 갯수를 리턴한다
int GetAniSize(lua_State *L)
{
	ENGINE::cAnimComponent* pAniComPonent = NULL;

	//애니 컴포넌트가 있다면 그 포인터를 받아온다
	int componentSize = g_pGameObject->GetComponentSize();

	for (int i = 0; i < componentSize; i++)
	{
		if (g_pGameObject->GetCompoent(i)->GetType() == ENGINE::eANIMATION_TYPE)
		{
			pAniComPonent = (ENGINE::cAnimComponent*)g_pGameObject->GetCompoent(i);
		}
	}
	if (pAniComPonent == NULL){
		//애니 컴포넌트가 존재안하면 0 리턴
		lua_pushnumber(L, 0);
		return 1;
	}

	//ani_size를 리턴해준다
	lua_pushnumber(L, pAniComPonent->GetAniClipNum());

	return 1;
}
//애니메이션을 지정한다
int SetAni(lua_State *L)
{
	ENGINE::cAnimComponent* pAniComPonent = NULL;

	//애니 컴포넌트가 있다면 그 포인터를 받아온다
	int componentSize = g_pGameObject->GetComponentSize();

	for (int i = 0; i < componentSize; i++)
	{
		if (g_pGameObject->GetCompoent(i)->GetType() == ENGINE::eANIMATION_TYPE)
		{
			pAniComPonent = (ENGINE::cAnimComponent*)g_pGameObject->GetCompoent(i);
		}
	}
	if (pAniComPonent == NULL) return 1;

	//스크립트에서 입력받은 선택한 애니 인덱스를 가져온다
	std::string aniIndex = lua_tostring(L, 1);

	pAniComPonent->Play(aniIndex);

	return 0;
}
//애니메이션 이벤트 콜백 함수 지정
int SetAniEvnetFunction(lua_State *L)
{
	//함수가 호출될 애니 인덱스를 가져온다
	std::string aniIndex = lua_tostring(L, 1);
	//함수가 호출될 애니 프레임
	int frame = lua_tonumber(L, 2);
	//호출될 함수 타입
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



//자기 위치의 x값을 리턴한다
int GetPosX(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();

	lua_pushnumber(L, myPos.x);
	return 1;
}
//자기 위치의 y값을 리턴한다
int GetPosY(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();

	lua_pushnumber(L, myPos.y);
	return 1;
}
//자기 위치의 z값을 리턴한다
int GetPosZ(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();

	lua_pushnumber(L, myPos.z);
	return 1;
}

//자기 위치를 지정한다
int SetGameObjectPos(lua_State *L)
{
	D3DXVECTOR3 pos;


	// lua statck 위치값 x,y,z값을 뽑아낸다.
	pos.x = lua_tonumber(L, 1);
	pos.y = lua_tonumber(L, 2);
	pos.z = lua_tonumber(L, 3);

	g_pGameObject->SetTranslate(pos.x, pos.y, pos.z);

	return 0;
}


//특정 태그 객체중 자기와 가장 가까운 오브젝트의 id를 받는다
int GetTagToUID_MostNearGameObject(lua_State *L)
{
	//입력받은 태그 넘버를 받는다
	int nTag = (int)lua_tonumber(L, 1);
	int sightRange = lua_tonumber(L, 2);

	ENGINE::GAMEOBJECT_TAG tag = (ENGINE::GAMEOBJECT_TAG)nTag;

	//찾은 게임 오브젝트들이 갯수
	int findObjSize = 0;
	//찾은 게임 오브젝트 u_id 배열
	unsigned int UID_Arr[100];
	g_pScriptComponent->FindTagGameObjects(tag, UID_Arr, findObjSize);

	//가장 가까운 태그 객체를 찾는다
	double mostNearDistance = 1000000.0;
	int mostNearUID = -1;
	for (int i = 0; i < findObjSize; i++){
		ENGINE::GameObject* obj = g_pScriptComponent->FindU_IDGameObject(UID_Arr[i]);
		//죽어서 안그리는 존재이면 넘어간다
		bool isRender = obj->GetIsEnable();
		if (isRender == false)	continue;

		//자신의 위치
		D3DXVECTOR3 pos1 = g_pGameObject->GetTranslate();
		//검색한 게임 오브젝트 위치
		D3DXVECTOR3 pos2 = obj->GetTranslate();

		double distance = g_pScriptComponent->GetDistance(pos1, pos2);

		//처음 입력받거난 전에 입력 받은 객체보다 더 가까우면 바꾸어준다
		if (mostNearUID == -1 || distance < mostNearDistance){
			mostNearUID = UID_Arr[i];
			mostNearDistance = distance;
		}
	}

	//제일 가까운 오브젝트가 시야 사거리 밖이면 -1을 넘겨준다
	if (sightRange <= mostNearDistance)
		lua_pushnumber(L, -1);
	else
		lua_pushnumber(L, mostNearUID);
	return 1;
}

//특정 게임 오브젝트와 나와의 거리를 구한다
int UID_GameObjectDistance(lua_State *L)
{
	//입력받은 게임 오브젝트 u_id를 저장한다
	unsigned int id = (unsigned int)lua_tonumber(L, 1);
	ENGINE::GameObject* obj = g_pScriptComponent->FindU_IDGameObject(id);

	//자신의 위치
	D3DXVECTOR3 pos1 = g_pGameObject->GetTranslate();
	//검색한 게임 오브젝트 위치
	D3DXVECTOR3 pos2 = obj->GetTranslate();
	
	double distance = g_pScriptComponent->GetDistance(pos1, pos2);

	//거리 값을 리턴해준다
	lua_pushnumber(L, distance);

	return 1;
}
//입력받은 위치와 자기 위치와의 거리를 구하여 리턴한다
int SCRIPT_GetDistance(lua_State *L)
{
	D3DXVECTOR3 myPos = g_pGameObject->GetTranslate();
	D3DXVECTOR3 pos;

	// lua statck 에서 기준점을 얻어낸다.
	pos.x = lua_tonumber(L, 1);
	pos.y = lua_tonumber(L, 2);
	pos.z = lua_tonumber(L, 3);

	double distance = sqrt(((myPos.x - pos.x) * (myPos.x - pos.x))
		+ ((myPos.y - pos.y) * (myPos.y - pos.y))
		+ ((myPos.z - pos.z) * (myPos.z - pos.z)));

	//이동 결과 값을 리턴해준다
	lua_pushnumber(L, distance);

	return 1;
}

//입력받은 아이디를 가진 게임 오브젝트가 존재하는지 여부 검사
int IsExistGameObject(lua_State *L)
{
	//입력받은 게임 오브젝트 u_id를 저장한다
	unsigned int id = (unsigned int)lua_tonumber(L, 1);
	ENGINE::GameObject* obj = g_pScriptComponent->FindU_IDGameObject(id);
	
	//존재하고 캐릭터가 죽지 않아서 그려지고 있는 상태이면
	if (obj && obj->GetIsEnable() == true)	lua_pushboolean(L, true);
	//아니면
	else									lua_pushboolean(L, false);

	return 1;
}

//타겟이 된 오브젝트의 OnDemage()함수를 호출한다
int TargetObjectDemage(lua_State *L)
{
	int UID_TARGET = lua_tonumber(L, 1);
	int power = lua_tonumber(L, 2);

	ENGINE::GameObject* target = ENGINE::g_pGameObjectManager->FindGameObject(UID_TARGET);

	ENGINE::cScriptComponent* TargetScript = (ENGINE::cScriptComponent*)(target->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));

	//루아 업데이트 함수 호출
	lua_State* pLua = TargetScript->GetLua();
	lua_getglobal(pLua, "OnDemage");

	lua_pushnumber(pLua, power);

	lua_pcall(pLua, 1, 0, 0);
	return 0;
}

//게임오브젝트 삭제 함수
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
		// 매개변수 저장
		g_pGameObject = pGameObject;
		g_pGameObject->SetCallback_OnCollisionEnter(CollisionEvent);
		m_pGameObject = pGameObject;
		memset(m_scriptPath, 0, 256);
		memcpy(m_scriptPath, scriptPath, strlen(scriptPath));

		//루아 생성
		if (m_Lua != NULL){
			lua_close(m_Lua);
			m_Lua = NULL;
		}
		m_Lua = luaL_newstate();

		//스크립트 로드
		luaL_dofile(m_Lua, m_scriptPath);

		lua_register(m_Lua, "testFunction1", testFunction1);
		lua_register(m_Lua, "testFunction2", testFunction2);
		//루아 글루 함수 저장
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

		//루아 초기화 함수 호출
		lua_getglobal(m_Lua, "StartScript");
		lua_pcall(m_Lua, 0, 0, 0);


		//aSTAR생성
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

		//루아 업데이트 함수 호출
		lua_getglobal(m_Lua, "UpdateScript");
		
		//매개변수 시작 넣어주기
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
		스크립트 이벤트 호출 함수
	*/
	void cScriptComponent::CollisionEvent(ENGINE::cCollisionShape* collider1, ENGINE::cCollisionShape* collider2)//collider1이 자기 자신 , collider2 상대충돌체
	{
		//플레이어 무기 이면 플레이어 스크립트를 실행한다
		if (collider1->GetName() == "playerSwordRH" || collider1->GetName() == "playerSwordLH")
		{
			g_pGameObject = collider1->GetOwnerGameObject()->GetParent();
			g_pScriptComponent = (cScriptComponent*)(g_pGameObject->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));
		}
		else{
			g_pGameObject = collider1->GetOwnerGameObject();
			g_pScriptComponent = (cScriptComponent*)(collider1->GetOwnerGameObject()->GetComponent<ENGINE::cScriptComponent*>(ENGINE::eSCRIPT_TYPE));
		}
		
		//루아 업데이트 함수 호출
		if (g_pScriptComponent == NULL) return;

		lua_State* pLua = g_pScriptComponent->GetLua();
		lua_getglobal(pLua, "CollisionEvent");
		
		//매개변수 시작 넣어주기
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

		//마우스 이벤트 타입 ("enemyAttack", "move" ,"npTalk")
		lua_pushlstring(pLua, eventType.c_str(), strlen(eventType.c_str()));

		//마우스 3d좌표(x,
		lua_pushnumber(pLua, pos.x);
		lua_pushnumber(pLua, pos.y);
		lua_pushnumber(pLua, pos.z);

		//타입이 move일 경우 캐릭터를 클릭한게 아니므로 -1
		//타입이 enemyAttack일 경우 타겟 enemy U_ID
		//타입이 npTalk일 경우 npc U_ID
		lua_pushnumber(pLua, U_ID);

		lua_pcall(pLua, 5, 0, 0);
	}

	/*
		기능 함수
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
		유틸 함수
	*/
	double cScriptComponent::GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
	{
		double distance = sqrt(((pos1.x - pos2.x) * (pos1.x - pos2.x))
			+ ((pos1.y - pos2.y) * (pos1.y - pos2.y))
			+ ((pos1.z - pos2.z) * (pos1.z - pos2.z)));

		return distance;
	}
}
