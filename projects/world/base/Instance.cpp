#include "Instance.hpp"
#include "../world.hpp"

namespace wrd
{
#define THIS Instance
	WRD_CLASS_DEF(Instance)

	Id THIS::_from_dtor;
	widx THIS::_chk_n_from_alloc = WRD_INDEX_ERROR;

	THIS::THIS() : _id(WRD_INDEX_ERROR, _chk_n_from_alloc, WRD_DUMMY) { _chk_n_from_alloc = WRD_INDEX_ERROR; /* used up. */ }
	THIS::THIS(Id id) : _id(id) {} // no binding required.
	THIS::~THIS()
	{
		_getMgr().unbind(*this);
		_from_dtor = _id;
	}

	wbool THIS::operator==(const This& rhs) const { return _id.num == rhs._id.num; }
	wbool THIS::operator!=(const This& rhs) const { return ! operator==(rhs); }
	void* THIS::operator new(size_t sz) { return _getMgr()._new1(sz); }
	void THIS::operator delete(void* pt, size_t sz) { _getMgr()._del(pt, sz); }

	Id THIS::getId() const
	{
		if(_id.sep.blk_n == WRD_INDEX_ERROR)
			_getMgr().bind((This&)*this);
		return _id;
	}

	wbool THIS::isHeap() const
	{
		const Block& blk = getBlock();
		WRD_IS_NULL(blk, wasnull, false)

		return blk.isHeap();
	}

	CStrong THIS::toStrong() const
	{
		WRD_UNCONST()
		return unconst.toStrong();
	}

	CWeak THIS::toWeak() const
	{
		WRD_UNCONST()
		return unconst.toWeak();
	}

	const Block& THIS::getBlock() const { return _getBlock(getId()); }

	Res& THIS::release()
	{
		_id.sep.blk_n = _id.sep.chk_n = WRD_INDEX_ERROR;
		_id.sep.serial = WRD_DUMMY;
		return wasgood;
	}

	Block& THIS::_getBlock(Id id) { return WRD_GET((Block&) _getMgr().getAkashic()[id], blk); }

	Res& THIS::_setId(Id new1)
	{
		_id = new1;
		return wasgood;
	}

	Instancer& THIS::_getMgr() { return World::get().getInstancer(); }
}
