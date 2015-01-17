#pragma once

#include "../../Includes/Includes.hpp"

namespace DX9Graphics
{
	template <typename T>	//	T = Selector로 참조할 모듈
	class NE_DLL TValidator : public NEModule
	{
	public:
		typedef TValidator<T> ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEModuleSelector> arg_validator;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			return tray.push(arg_validator);
		}

		bool _isSelectedOneValid(const NEModule& target)
		{
			const NECodeSet& cs = T::getModuleScriptCodes();

			return cs.find(target.getCodes(NECodeType::SCRIPT)[0]);
		}
	};
}