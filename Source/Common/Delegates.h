#pragma once

namespace quokka
{
	template <typename T_RET, typename T_PARAM0>
	class Callback
	{
	public:
		virtual T_RET Invoke(T_PARAM0) = 0;
	};

	template <typename T_RET, typename T_PARAM0> 
	class StaticCallback : public Callback<T_RET, T_PARAM0>
	{
		typedef T_RET (*FUNC_POINTER)(T_PARAM0);
	public:
		StaticCallback(FUNC_POINTER A_Function) : Function(A_Function) {}

		T_RET Invoke(T_PARAM0 Param0) { return *Function(Param0); }
	private:
		FUNC_POINTER Function;
	};

	template <typename T_CLASS, typename T_RET, typename T_PARAM0>
	class MethodCallback : public Callback<T_RET, T_PARAM0>
	{
		typedef T_RET (T_CLASS::*FUNC_POINTER)(T_PARAM0);
	public:
		MethodCallback(T_CLASS* A_Object, FUNC_POINTER A_Function) : Object(A_Object), Function(A_Function) {}

		T_RET Invoke(T_PARAM0 Param0) { return (Object->*Function)(Param0); }
	private:
		T_CLASS* Object;
		FUNC_POINTER Function;
	};

	template <typename T_RET, typename T_PARAM0>
	class Delegate
	{
		typedef T_RET (*FUNC_POINTER) (T_PARAM0);
	public:
		Delegate(FUNC_POINTER A_Function) : mCallback(new StaticCallback(A_Function)) {}

		template <typename T_CLASS>
		Delegate(T_CLASS* Obj, T_RET (T_CLASS::*Method)(T_PARAM0)) : mCallback(new MethodCallback<T_CLASS, T_RET, T_PARAM0>(Obj, Method)) {}

		~Delegate()
		{
			delete mCallback;
			mCallback = NULL;
		}

		T_RET operator ()(T_PARAM0 Param0) { return mCallback->Invoke(Param0); }

	private:
		Callback<T_RET, T_PARAM0>* mCallback;
	};
}