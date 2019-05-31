#pragma once
class cTransform
{
public:
	//전체 방위 값을 초기화
	void Reset();
	//행렬 업데이트(로컬 s, r, t 에 변화가 있으면 호출)
	void UpdateMatrix();

	//S, R, T 셋팅
	void SetPosition(float x, float y, float z);
	void SetSclae(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetLocalMatrix();

	//이동, 회전, 스케일링
	void Translation_World(float dx, float dy, float dz); //월드 기준으로 이동
	void Translation_Self(float right_dx, float up_dy, float front_dz); //자신축 기준으로 이동
	void Scaling(float dx, float dy, float dz);	//스케일 값에 변화
	void Rotate_World(float dx, float dy, float dz);	//x,y,z량 만큼 월드축 기준으로 회전
	//void Rotate_Self(float dx, float dy, float dz);	//x,y,z량 만큼 자신축 기준으로 회전

	//Get 함수
	D3DXMATRIXA16 GetWorldMatrix();
	D3DXVECTOR3 GetLocalScale();
	D3DXVECTOR3 GetLocalRotation();
	D3DXVECTOR3 GetLocalPosition();
	
	D3DXVECTOR3 GetLocalRight();
	D3DXVECTOR3 GetLocalUp();
	D3DXVECTOR3 GetLocalFront();
	
	//부모가 없다면 local과 같다
	D3DXVECTOR3 GetWorldPosition();
	D3DXVECTOR3 GetWorldRight();
	D3DXVECTOR3 GetWorldUp();
	D3DXVECTOR3 GetWorldFront();
	
	void Addchild(cTransform* pChild);
	void DetechParent();	//부모로부터 독립
	cTransform* GetParent();

	cTransform();
	~cTransform();

private:
	D3DXVECTOR3 m_LocalPosition;
	D3DXVECTOR3 m_LocalScale;
	D3DXVECTOR3 m_LocalRotation;

	D3DXMATRIXA16 m_matLocal;

	D3DXMATRIXA16 m_matWorld;

	cTransform* m_pParnet;
	std::vector<cTransform*> m_vecChild;

	//로컬 방향벡터
	D3DXVECTOR3 m_LocalRight;
	D3DXVECTOR3 m_LocalUp;
	D3DXVECTOR3 m_LocalFront;

	
};

