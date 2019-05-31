#pragma once
class cScene
{
public:
	virtual void Load(std::wstring & fileName) = 0;
	virtual void UnLoad() = 0;
	inline const std::wstring& GetID() const { return _id; }
	inline void SetID(const std::wstring& id) { _id = id; }

	virtual void Update(DWORD elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void OnEnter();
	virtual void OnLeave();

	cScene();
	~cScene();


protected:
	std::wstring _id;

};

