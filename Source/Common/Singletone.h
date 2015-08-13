#pragma once

namespace quokka
{
	template <class T>
	class Singletone
	{
	public:	
		~Singletone()
		{
			delete Instance;
		}
		static T* GetInstance()
		{
			if(!Instance)
				Instance = new T();
			return Instance;
		}
	protected:
		static T* Instance;
	};
}