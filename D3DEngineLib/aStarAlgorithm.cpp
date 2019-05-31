#include "aStarAlgorithm.h"


aStarAlgorithm::aStarAlgorithm()
{
	m_MapTileData = NULL;
}


aStarAlgorithm::~aStarAlgorithm()
{
}

void aStarAlgorithm::Init(int* (*pMapTileData), int tileRow, int tileCol, float tileSize)
{
	m_MapTileData = pMapTileData;

	m_TileRow = tileRow;

	m_TileCol = tileCol;

	m_TileSize = tileSize;
}

void aStarAlgorithm::NextMovePoint(float TargetPosX, float TargetposZ, float myPosX, float myPosZ, float &nextPosX, float &nextPosZ)
{
	//��ǥ�� 0���� �����ϴ� �� �·�� ġȯ
	TargetPosX += (m_TileRow / 2) * m_TileSize;
	TargetposZ = (m_TileCol / 2) * m_TileSize - TargetposZ;

	myPosX += (m_TileRow / 2) * m_TileSize;
	myPosZ = ((m_TileCol / 2) * m_TileSize) - myPosZ;

	TILEPOS tarGetTilePos, myTilePos, NextTilePos;

	//��ġ ��ǥ�� Ÿ�� ��ġ�� ġȯ
	tarGetTilePos.x = int(TargetPosX / m_TileSize);
	tarGetTilePos.z = int(TargetposZ / m_TileSize);

	myTilePos.x = int(myPosX / m_TileSize);
	//myTilePos.z = int(myPosZ / m_TileSize);
	myTilePos.z = (int)myPosZ / (int)m_TileSize;

	//�ڱ� ��ġ���� �̵��� �ִ� 9���� Ÿ�ϰ�
	DirTile dirTilePos[9];
	//�̵��Ҽ� �ִ� Ÿ�ٿ� ���� ����� Ÿ��
	DirTile bestNear;
	//ó���� ������ �־��ش�
	bool isFirst = true;

	for (int col = 0; col < 3; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			int index = col * 3 + row;

			//��ǥ�� �ֱ�
			dirTilePos[index].x = myTilePos.x - 1 + row;
			dirTilePos[index].z = myTilePos.z - 1 + col;

			//�̵��������� ���� �ֱ�
			if (dirTilePos[index].x < 0 || dirTilePos[index].z < 0)		dirTilePos[index].state = 100;
			else														dirTilePos[index].state = m_MapTileData[dirTilePos[index].x][dirTilePos[index].z];

			//�������� ��ŭ ������ �ִ��� �ֱ�
			dirTilePos[index].distance = abs(tarGetTilePos.x - dirTilePos[index].x) + abs(tarGetTilePos.z - dirTilePos[index].z);

			//ó�� �̸� �׳� �־��ش�
			if (isFirst){
				isFirst = false;
				bestNear = dirTilePos[index];
			}
			//���� �ִ� Ÿ��( state = 0 )�� ��� ��ǥ������ �� ������ �ٲ��ش�.
			else{
				if (dirTilePos[index].state == 0 && dirTilePos[index].distance < bestNear.distance)
					bestNear = dirTilePos[index];
			}
		}
	}

	//�̵��� ��ǥ ��ġ�� ġȯ
	nextPosX = (float)bestNear.x * m_TileSize;
	nextPosX -= (m_TileRow / 2) * m_TileSize;
	nextPosX += m_TileSize / 2;//Ÿ�� ����� ��ġ�� ���ش�

	nextPosZ = (float)bestNear.z * m_TileSize;
	nextPosZ = ((m_TileCol / 2) * m_TileSize) - nextPosZ;
	nextPosZ -= m_TileSize / 2;//Ÿ�� ����� ��ġ�� ���ش�
}
void aStarAlgorithm::GetMovePointList(float TargetPosX, float TargetposZ, float myPosX, float myPosZ, TilePosList& vMovePoint)//(Ÿ����ġ, �ڽ��� ��ġ, �̵��ϴ� ������ ������ ���۷���)
{
	
}