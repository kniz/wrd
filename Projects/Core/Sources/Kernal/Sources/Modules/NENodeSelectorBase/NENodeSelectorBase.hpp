#pragma once

//	include:
#include "../NEKey/NEKey.hpp"
#include "../NECodeSet/NECodeSet.hpp"
#include "../../Commons/Units/NECodeType/NECodeType.hpp"
#include "../../Commons/Interfaces/NEInitializable/NEInitializable.hpp"

namespace NE
{
	class NENode;	

	class NE_DLL NENodeSelectorBase : public NEKey, public NEInitializable
	{
		NE_DECLARE_INTERFACE(NENodeSelectorBase, NEKey)

	public:
		This(const NETString& keyname = "");
		This(const This& source);

		//	소멸자:
	public:
		virtual ~NENodeSelectorBase();

		//	연산자 오버로딩:
	public:
		const This& operator=(const This& source);
		type_bool operator==(const This& source) const;
		type_bool operator!=(const This& source) const;

		//	인터페이스:		
		//		상속:
	public:
		const NECodeType& getCodesType() const;
		type_result setCodesType(const NECodeType& new_one);
		type_result setCodes(const NECodeSet& new_codeset);
		const NECodeSet& getCodes() const;
		type_count getCountLimit() const;
		void setCountLimit(type_count limit);
		type_bool isUsingAndOperation() const;
		void setUsingAndOperation(type_bool enable);
		virtual type_result bind() = 0;

	public:
		virtual type_result initializeReferingPoint() = 0;

	public:
		virtual void releaseValue();

	public:
		virtual void release();
		virtual type_result isValid() const;
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

		//	내부 함수:
	private:
		const This& _assign(const This& source);
		void _release();

	protected:
		type_bool _use_and_operation;
		NECodeSet _codes;
		type_count _count;
		type_count _countlimit;
		type_bool _is_peeking_locked;
	};
}
