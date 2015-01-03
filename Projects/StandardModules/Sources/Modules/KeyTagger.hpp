#pragma once

#include "NamedUnitTagger.hpp"

namespace NE
{
	class NE_DLL KeyTagger : public NEModule
	{
	public:
		typedef KeyTagger ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEKeySelector>	arg_targets;
		NETArgument<NETStringKey>	arg_name;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_targets);
			tray.push(arg_name);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			arg_name.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_name.setEnable(false);
			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			type_result result = RESULT_SUCCESS;
			NEKeySelector& ks = arg_targets.getValue();
			while (NEKey* n = &ks.getKey())
			{
				if(arg_name.getPurpose() == NEArgumentBase::READ_BY)
					n->getName() = arg_name.getValue();
				else
					result |= arg_name.setValue(n->getName());
			}

			return result;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;
			static bool is_triggered_once = false;

			if (!is_triggered_once)
			{
				is_triggered_once = true;
				NEExportable::ModuleHeader supers = SuperClass::getHeader();

				_instance.getName() = "KeyTagger";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/02";
				_instance.getComment() =
					"참조한 Module 객체들의 특정 속성값들을 가져오거나(get) 할당할 수 있습니다(set).\n"
					"\tTargets:\t참조할 객체들\n"
					"\tKeyName:\t키의 이름";
					supers.getComment();
				//"주어진 객체들의 다음과 같은 속성들에 대해서 값을 가져오거나 할당 할 수 있습니다.\n"
				//"\tEnable:\t객체의 활성화 여부.\n"
				//"\tCodes:\t객체의 주어진 Code값들";					
				NETStringSet args = supers.getArgumentsComments();
				args.resize(4);
				args.pushFront("KeyName\n키의 이름입니다.");
				args.pushFront("Targets\n속성을 가져오거나 할당하게될 Module 들입니다.");
			}

			return _instance;
		}
	};
}