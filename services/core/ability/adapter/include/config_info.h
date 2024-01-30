//
// Created by x30015763 on 2024/1/23.
//

#ifndef UPDATE_SERVICE_CONFIG_INFO_H
#define UPDATE_SERVICE_CONFIG_INFO_H

#include <cstdint>
#include <string>

namespace OHOS::UpdateEngine {
struct ConfigInfo {
    std::string businessDomain;
    uint32_t abInstallTimeout = 1800; // 1800s
    std::string moduleLibPath;
};
} // OHOS::UpdateEngine
#endif //UPDATE_SERVICE_CONFIG_INFO_H
