#pragma once
#include <stdlib.h>
#include <vector>
//row�� ��(����)�̰� col�� ��(����)�Դϴ�.

struct DirTile
{
	int x;
	int z;
	int state;
	int distance;
};

struct TILEPOS
{
	int x;
	int z;
};

typedef std::vector<TILEPOS> TilePosList;

class aStarAlgorithm
{
public:
	aStarAlgorithm();
	~aStarAlgorithm();
private:
	// ���� ��� Ÿ�� ������( 0 : �̵� ����, )
	int* (*m_MapTileData);
	// ���� Ÿ�� ���� ����
	int m_TileRow;
	// ���� Ÿ�� ���� ����
	int m_TileCol;
	// Ÿ���� ũ��(���� ���� ����)
	float m_TileSize;
public:
	//���� Ÿ�� ���� �޾ƿ� �ʱ�ȭ �Ѵ�
	void Init(int* (*pMapTileData), int tileRow, int tileCol, float tileSize);
public:
	//�Է¹��� Ÿ����ġ�� �ڽ��� ��ġ�� Ÿ������ �̵��� ���� ��ġ�� �޴´�
	void NextMovePoint(float TargetPosX, float TargetposZ, float myPosX, float myPosZ, float &nextPosX, float &nextPosZ);
	//�Է¹��� Ÿ����ġ�� �ڽ��� ��ġ�� Ÿ�ٱ��� �̵��ϴ� ���� ã�Ƽ� �����Ѵ�
	void GetMovePointList(float TargetPosX, float TargetposZ, float myPosX, float myPosZ, TilePosList& vMovePoint);
private:
	//aStar�˰��� �Լ���
};

