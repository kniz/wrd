#include "NENodeManager.hpp"
#include "../NENode/NENode.hpp"

namespace NE
{	
	type_result NENodeManager::initialize()
	{
		return getLocalStack().initialize();
	}
}
