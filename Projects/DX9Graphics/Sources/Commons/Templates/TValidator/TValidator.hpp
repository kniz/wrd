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

		virtual type_result _onFetchModule()
		{
			arg_validator.getValue().NENodeSelector::isUsingAutoBinding() = true;
			arg_validator.getValue().isUsingAutoBinding() = true;

			return RESULT_SUCCESS;
		}

		T& cast(NEModule& target)
		{
			const NECodeSet& cs = T::getModuleScriptCodes();
			T* nullpointer = 0x00;
			if (!&target)
				return *nullpointer;

			NECode sample(target.getScriptCode(), NECodeType(NECodeType::MODULE_SCRIPT));

			if(	&target								&&
				cs.find(sample) != NE_INDEX_ERROR	)
				return static_cast<T&>(target);

			return *nullpointer;
		}

		T& cast(NETArgument<NEModuleSelector>& target)
		{
			T* nullpointer = 0x00;
			NEModule& module = target.getValue().getModule();
			if( ! &module) return *nullpointer;

			target.getValue().initializeReferingPoint();

			return cast(module);
		}
	};
}