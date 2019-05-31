#pragma once
#include <stdlib.h>
#include <vector>
//row는 행(가로)이고 col은 열(세로)입니다.

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
	// 맵의 모든 타일 데이터( 0 : 이동 가능, )
	int* (*m_MapTileData);
	// 맵의 타일 가로 갯수
	int m_TileRow;
	// 맵의 타일 세로 갯수
	int m_TileCol;
	// 타일의 크기(가로 세로 같다)
	float m_TileSize;
public:
	//씬의 타일 값을 받아와 초기화 한다
	void Init(int* (*pMapTileData), int tileRow, int tileCol, float tileSize);
public:
	//입력받은 타겟위치과 자신의 위치로 타겟으로 이동할 다음 위치를 받는다
	void NextMovePoint(float TargetPosX, float TargetposZ, float myPosX, float myPosZ, float &nextPosX, float &nextPosZ);
	//입력받은 타겟위치와 자신의 위치로 타겟까지 이동하는 길을 찾아서 리턴한다
	void GetMovePointList(float TargetPosX, float TargetposZ, float myPosX, float myPosZ, TilePosList& vMovePoint);
private:
	//aStar알고리즘 함수들
};

