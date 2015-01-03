#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL NodeSelectorTagger : public NEModule
	{
	public:
		typedef NodeSelectorTagger ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NENodeSelector>	arg_targets;
		NETArgument<NEIntKey>		arg_manager_type;
		NETArgument<NECodeSetKey>	arg_codes;
		NETArgument<NEIntKey>		arg_countlimit;
		NETArgument<NEBooleanKey>	arg_use_and_operation;
		NETArgument<NEBooleanKey>	arg_use_auto_binding;
		NETArgument<NEIntKey>		arg_initializer_count;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_manager_type.setEnable(false);
			arg_manager_type.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_codes.setEnable(false);
			arg_codes.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_countlimit.setEnable(false);
			arg_countlimit.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_use_and_operation.setEnable(false);
			arg_use_and_operation.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_use_auto_binding.setEnable(false);
			arg_use_auto_binding.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_initializer_count.setEnable(false);
			arg_initializer_count.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_targets);
			tray.push(arg_manager_type);
			tray.push(arg_codes);
			tray.push(arg_countlimit);
			tray.push(arg_use_and_operation);
			tray.push(arg_use_auto_binding);
			tray.push(arg_initializer_count);

			return RESULT_SUCCESS;
		}
		type_result _passPipes(NENodeSelector& n)
		{
			type_result result = RESULT_SUCCESS;

			if(arg_manager_type.isEnable())
			{
				if (arg_manager_type.getPurpose() == NEArgumentBase::READ_BY)
					result |= n.setManager(NEType::Type(arg_manager_type.getValue()));
				else
					result |= arg_manager_type.setValue(n.getManagerType());
			}
			if(arg_codes.isEnable())
			{
				const NECodeSet& codes = arg_codes.getValue();

				if (arg_codes.getPurpose() == NEArgumentBase::READ_BY)
					result |= n.setCodes(codes);
				else
					result |= arg_codes.setValue(n.getCodes());
			}
			if(arg_countlimit.isEnable())
			{
				if (arg_countlimit.getPurpose() == NEArgumentBase::READ_BY)
					n.setCountLimit(arg_countlimit.getValue());
				else
					result |= arg_countlimit.setValue(n.getCountLimit());
			}
			if(arg_use_and_operation.isEnable())
			{
				if (arg_use_and_operation.getPurpose() == NEArgumentBase::READ_BY)
					n.setUsingAndOperation(arg_use_and_operation.getValue());
				else
					result |= arg_use_and_operation.setValue(n.isUsingAndOperation());
			}
			if(arg_use_auto_binding.isEnable())
			{
				if (arg_use_auto_binding.getPurpose() == NEArgumentBase::READ_BY)
					n.isUsingAutoBinding() = arg_use_auto_binding.getValue();
				else
					result |= arg_use_auto_binding.setValue(n.isUsingAutoBinding());
			}
			type_int initializer_count = arg_initializer_count.getValue();
			if (initializer_count != 0)
			{
				result |= n.initializeReferingPoint();

				if(initializer_count > 0)
					initializer_count--;
			}

			return result;
		}
		virtual type_result _onExecute()
		{
			return _passPipes(arg_targets.getValue());
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "NodeSelectorTagger";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/02";
				_instance.getComment() =
					"NodeSelector의 속성값을 가져오거나, 변경할 수 있습니다."
					"변경 할 수 있는 속성은 다음과 같습니다.\n"
					"- ManagerType: NodeSelector가 탐색할 Manager의 종류를 말합니다.\n"
					"- Codes\n"
					"- CountLimit\n"
					"- Use AND Operation\n\n"
					"Initializer Count는 지정된 숫자만큼 execute시마다 초기화 시킵니다."
					"-1이면, 숫자제한없이 execute 될때마다 항상 초기화 됩니다.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(7);
				args.push("Target\n속성을 수정할 대상입니다.");
				args.push("ManagerType");
				args.push("Codes");
				args.push("CountLimit");
				args.push("UseAndOperation");
				args.push("UseAutoBinding");
				args.push("InitializerCount");
			}

			return _instance;
		}
	};
}