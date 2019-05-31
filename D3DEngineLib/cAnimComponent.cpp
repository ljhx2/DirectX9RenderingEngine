#include "Common.h"
#include "include\D3DRenderer.h"
#include "cAnimComponent.h"


namespace ENGINE
{
	cAnimComponent::cAnimComponent()
	{
		eComponent_Type = eANIMATION_TYPE;
		m_pOwnerObject = NULL;
	}

	cAnimComponent::cAnimComponent(GameObject* pOwnerObject)
	{
		eComponent_Type = eANIMATION_TYPE;
		m_pOwnerObject = pOwnerObject;

		skinnedComp = m_pOwnerObject->GetComponent<cSkinnedMeshComponent*>(eSKINNED_MESH_TYPE);
		fbxComp = m_pOwnerObject->GetComponent<cFbxComponent*>(eFBX_MESH_TYPE);

		m_pCurrentClip = NULL;
	}



	cAnimComponent::~cAnimComponent()
	{
		for each(auto p in m_mapAnimClip)
		{
			SAFE_DELETE(p.second);
		}

		for each(auto p in m_vecAnimCallback)
		{
			SAFE_DELETE(p);
		}
	}

	void cAnimComponent::Update(float deltaTime, D3DXMATRIXA16* matWorld)
	{
		if (m_pCurrentClip == NULL) return;

		int size = m_vecAnimCallback.size();
		for (int i = 0; i < size; ++i)
		{
			if (fbxComp != NULL)
			{
				AnimCallBackFunc* animC = m_vecAnimCallback[i];
				if (m_pCurrentClip->name == animC->name)
				{
					if (animC->isRun && fbxComp->GetCurrentFrame() >= animC->frame)		//지나갔을때 어떻게 해야할지 생각
					{
						animC->callbackfunc(m_pOwnerObject);
						animC->isRun = false;
					}
					if (!animC->isRun && fbxComp->GetCurrentFrame() >= m_pCurrentClip->end)
					{
						animC->isRun = true;
					}
				}
			}
			//else if (skinnedComp != NULL) {}
		}
	}

	void cAnimComponent::Render()
	{
		//사용안함
	}

	void cAnimComponent::Play(std::string clipName)
	{
		m_pCurrentClip = m_mapAnimClip[clipName];

		if (skinnedComp != NULL)
		{
			skinnedComp->StartAnim(m_pCurrentClip);
			return;
		}
		if (fbxComp != NULL)
		{
			fbxComp->StartAnim(m_pCurrentClip);
		}
	}


	bool cAnimComponent::AddClip(std::string clipName, int setIndex, DOUBLE start, DOUBLE end, bool isLoop)
	{
		ST_AnimClip* clip = new ST_AnimClip;
		clip->name = clipName;
		clip->setIndex = setIndex;
		clip->start = start;
		clip->end = end;
		clip->isLoop = isLoop;

		std::pair<std::map<std::string, ST_AnimClip*>::iterator, bool> pr;	//insert() 결과를 알려주는 객체
		pr = m_mapAnimClip.insert(std::make_pair(clipName, clip));

		if (pr.second == true) // insert() 성공
		{
			return true;
		}
		else //insert() 실패
		{
			return false;
		}
	}

	void cAnimComponent::DeleteAllClip()
	{
		for each(auto p in m_mapAnimClip)
		{
			SAFE_DELETE(p.second);
		}
		m_mapAnimClip.clear();
	}
	void cAnimComponent::DeleteClip(std::string clipName)
	{
		std::map<std::string, ST_AnimClip*>::iterator iter;
		for (iter = m_mapAnimClip.begin(); iter != m_mapAnimClip.end(); )
		{
			if (iter->second->name == clipName)
			{
				SAFE_DELETE(iter->second);
				m_mapAnimClip.erase(iter);
				break;
			}
			else
			{
				++iter;
			}
		}
	}

	void cAnimComponent::AddAnimCallback(std::string clipName, long frame, ANIM_CALLBACK callback)
	{
		AnimCallBackFunc* animC = new AnimCallBackFunc;
		animC->name = clipName;
		animC->frame = frame;
		animC->callbackfunc = callback;
		animC->isRun = true;

		m_vecAnimCallback.push_back(animC);
	}
}