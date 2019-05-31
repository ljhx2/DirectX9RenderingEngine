#pragma once
class cGrid
{
public:
	void Setup(float fGap, int nNumHalf);
	void Render();

	cGrid();
	~cGrid();
	
private:
	std::vector<ST_PC_VERTEX> m_LineVertex;
	int m_nNumHalf;
};

