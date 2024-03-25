/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UPDATE_SERVICE_MODULE_MANAGER_H
#define UPDATE_SERVICE_MODULE_MANAGER_H

#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "ipc_skeleton.h"
#include "refbase.h"
#include "system_ability_ondemand_reason.h"

#include "update_service_module.h"

namespace OHOS {
namespace UpdateEngine {
class ModuleManager {
public:
    ModuleManager();
    ~ModuleManager() = default;

    static ModuleManager& GetInstance();

    void LoadModule(std::string libPath);
    bool IsModuleLoaded();
    void HookFunc(std::vector<uint32_t>, RequestFuncType handleRemoteRequest);
    int32_t HandleFunc(uint32_t code, MessageParcel& data, MessageParcel& reply, MessageOption &option);
    void HookOnStartOnStopFunc(std::string phase, LifeCycleFuncType handleSAOnStartOnStop);
    void HandleOnStartOnStopFunc(std::string phase, const OHOS::SystemAbilityOnDemandReason &reason);
    void HookOnIdleFunc(std::string phase, LifeCycleFuncReturnType handleSAOnIdle);
    int32_t HandleOnIdleFunc(std::string phase, const OHOS::SystemAbilityOnDemandReason &reason);

    static std::map<uint32_t, RequestFuncType> onRemoteRequestFuncMap_;

private:
    int32_t retryInterval_ = 0;
    void *dueModuleHandler = nullptr;
    static std::map<std::string, LifeCycleFuncType> onStartOnStopFuncMap_;
    static std::map<std::string, LifeCycleFuncReturnType> onIdleFuncMap_;
    static bool isLoaded;
    static std::mutex onRemoteRequestFuncMapMutex_;
    static std::mutex onStartOnStopFuncMapMutex_;
    static std::mutex onIdleFuncMapMutex_;
};
} // namespace UpdateEngine
} // namespace OHOS
#endif // UPDATE_SERVICE_MODULE_MANAGER_H
