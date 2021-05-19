#pragma once

class Cocos2dCreator
{
public:
	template<class T>
	static T* CreateRef()
	{
		T* pRet = new(std::nothrow) T();
		if (pRet)
		{
			pRet->retain();
		}
		else
		{
			delete pRet;
			pRet = nullptr;
		}
		return pRet;
	}

	template<class TReturn, class ...TParams>
	static TReturn* CreateNode(TParams&&... params)
	{
		TReturn* pRet = new(std::nothrow) TReturn();
		if (pRet && pRet->init(std::forward<TParams>(params)...))
		{
			pRet->autorelease();
		}
		else
		{
			delete pRet;
			pRet = nullptr;
		}

		return pRet;
	}

	
};
