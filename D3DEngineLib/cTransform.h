#pragma once
class cTransform
{
public:
	//��ü ���� ���� �ʱ�ȭ
	void Reset();
	//��� ������Ʈ(���� s, r, t �� ��ȭ�� ������ ȣ��)
	void UpdateMatrix();

	//S, R, T ����
	void SetPosition(float x, float y, float z);
	void SetSclae(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetLocalMatrix();

	//�̵�, ȸ��, �����ϸ�
	void Translation_World(float dx, float dy, float dz); //���� �������� �̵�
	void Translation_Self(float right_dx, float up_dy, float front_dz); //�ڽ��� �������� �̵�
	void Scaling(float dx, float dy, float dz);	//������ ���� ��ȭ
	void Rotate_World(float dx, float dy, float dz);	//x,y,z�� ��ŭ ������ �������� ȸ��
	//void Rotate_Self(float dx, float dy, float dz);	//x,y,z�� ��ŭ �ڽ��� �������� ȸ��

	//Get �Լ�
	D3DXMATRIXA16 GetWorldMatrix();
	D3DXVECTOR3 GetLocalScale();
	D3DXVECTOR3 GetLocalRotation();
	D3DXVECTOR3 GetLocalPosition();
	
	D3DXVECTOR3 GetLocalRight();
	D3DXVECTOR3 GetLocalUp();
	D3DXVECTOR3 GetLocalFront();
	
	//�θ� ���ٸ� local�� ����
	D3DXVECTOR3 GetWorldPosition();
	D3DXVECTOR3 GetWorldRight();
	D3DXVECTOR3 GetWorldUp();
	D3DXVECTOR3 GetWorldFront();
	
	void Addchild(cTransform* pChild);
	void DetechParent();	//�θ�κ��� ����
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

	//���� ���⺤��
	D3DXVECTOR3 m_LocalRight;
	D3DXVECTOR3 m_LocalUp;
	D3DXVECTOR3 m_LocalFront;

	
};

