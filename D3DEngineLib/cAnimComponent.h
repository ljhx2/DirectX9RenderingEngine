#pragma once
#include "IComponent.h"



namespace ENGINE
{
	//Anim관련 콜백함수 형 선언
	typedef void(*ANIM_CALLBACK)(ENGINE::GameObject*);


	class GameObject;

	struct AnimCallBackFunc
	{
		std::string name;
		long frame;
		ANIM_CALLBACK callbackfunc;
		bool isRun;
	};

	class cAnimComponent : public IComponent
	{
	public:
		virtual void Update(float deltaTime, D3DXMATRIXA16* matWorld);
		virtual void Render();


		void	Play(std::string clipName);
		bool	AddClip(std::string clipName, int setIndex, DOUBLE start, DOUBLE end, bool isLoop);
		void	DeleteAllClip();
		void	DeleteClip(std::string clipName);
		int		GetAniClipNum(){ return m_mapAnimClip.size(); }
		ST_AnimClip* GetCurrentAniClip(){ return  m_pCurrentClip; }
		std::map<std::string, ST_AnimClip*>* cAnimComponent::GetAniClip(){ return &m_mapAnimClip; }


		void AddAnimCallback(std::string clipName, long frame, ANIM_CALLBACK callback);

		cAnimComponent();
		cAnimComponent(GameObject* pOwnerObject);
		virtual ~cAnimComponent();

	private:
		ST_AnimClip* m_pCurrentClip;

		std::map<std::string, ST_AnimClip*>		m_mapAnimClip;
		std::vector<AnimCallBackFunc*> m_vecAnimCallback;

		cSkinnedMeshComponent* skinnedComp;
		cFbxComponent* fbxComp;
	};

}