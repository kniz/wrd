#include "packLoading.hpp"
#include "../../frame/thread.hpp"

namespace namu {

    NAMU_DEF_ME(packLoading)

    void me::addPath(const std::string& filePath) {
        _paths.push_back(filePath);
    }

    void me::addPath(const std::vector<std::string> paths) {
        if(&_paths == &paths) return;

        _paths.insert(_paths.end(), paths.begin(), paths.end());
    }

    void me::rel() {
        _paths.clear();
    }

    const std::vector<std::string>& me::_getPaths() const {
        return _paths;
    }
}
