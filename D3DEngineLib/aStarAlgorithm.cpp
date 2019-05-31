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
	//좌표를 0부터 시작하는 맵 좌료로 치환
	TargetPosX += (m_TileRow / 2) * m_TileSize;
	TargetposZ = (m_TileCol / 2) * m_TileSize - TargetposZ;

	myPosX += (m_TileRow / 2) * m_TileSize;
	myPosZ = ((m_TileCol / 2) * m_TileSize) - myPosZ;

	TILEPOS tarGetTilePos, myTilePos, NextTilePos;

	//위치 좌표를 타일 위치로 치환
	tarGetTilePos.x = int(TargetPosX / m_TileSize);
	tarGetTilePos.z = int(TargetposZ / m_TileSize);

	myTilePos.x = int(myPosX / m_TileSize);
	//myTilePos.z = int(myPosZ / m_TileSize);
	myTilePos.z = (int)myPosZ / (int)m_TileSize;

	//자기 위치에서 이동할 있는 9개의 타일값
	DirTile dirTilePos[9];
	//이동할수 있는 타겟에 가장 가까운 타일
	DirTile bestNear;
	//처음은 무조건 넣어준다
	bool isFirst = true;

	for (int col = 0; col < 3; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			int index = col * 3 + row;

			//좌표값 넣기
			dirTilePos[index].x = myTilePos.x - 1 + row;
			dirTilePos[index].z = myTilePos.z - 1 + col;

			//이동가능한지 여부 넣기
			if (dirTilePos[index].x < 0 || dirTilePos[index].z < 0)		dirTilePos[index].state = 100;
			else														dirTilePos[index].state = m_MapTileData[dirTilePos[index].x][dirTilePos[index].z];

			//도착지와 얼만큼 떨어져 있는지 넣기
			dirTilePos[index].distance = abs(tarGetTilePos.x - dirTilePos[index].x) + abs(tarGetTilePos.z - dirTilePos[index].z);

			//처음 이면 그냥 넣어준다
			if (isFirst){
				isFirst = false;
				bestNear = dirTilePos[index];
			}
			//갈수 있는 타일( state = 0 )일 경우 목표지점에 더 가까우면 바꿔준다.
			else{
				if (dirTilePos[index].state == 0 && dirTilePos[index].distance < bestNear.distance)
					bestNear = dirTilePos[index];
			}
		}
	}

	//이동할 좌표 위치로 치환
	nextPosX = (float)bestNear.x * m_TileSize;
	nextPosX -= (m_TileRow / 2) * m_TileSize;
	nextPosX += m_TileSize / 2;//타일 가운데를 위치로 해준다

	nextPosZ = (float)bestNear.z * m_TileSize;
	nextPosZ = ((m_TileCol / 2) * m_TileSize) - nextPosZ;
	nextPosZ -= m_TileSize / 2;//타일 가운데를 위치로 해준다
}
void aStarAlgorithm::GetMovePointList(float TargetPosX, float TargetposZ, float myPosX, float myPosZ, TilePosList& vMovePoint)//(타겟위치, 자신의 위치, 이동하는 지점을 저장할 레퍼런스)
{
	
}