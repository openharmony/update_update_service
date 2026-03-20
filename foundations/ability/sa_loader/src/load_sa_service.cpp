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

#include "load_sa_service.h"

#include <thread>

#include "update_log.h"

namespace OHOS::UpdateService {
LoadSaService::LoadSaService() = default;

LoadSaService::~LoadSaService() = default;

sptr<LoadSaService> LoadSaService::instance_ = nullptr;
std::mutex LoadSaService::instanceLock_;

sptr<LoadSaService> LoadSaService::GetInstance()
{
    if (instance_ == nullptr) {
        std::lock_guard<std::mutex> lock(instanceLock_);
        if (instance_ == nullptr) {
            instance_ = new LoadSaService();
        }
    }
    return instance_;
}

bool LoadSaService::TryLoadSa(int systemAbilityId)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (sm == nullptr) {
        ENGINE_LOGE("samgr object null!");
        return false;
    }
    constexpr int32_t loadTimeSeconds = 5;
    auto remoteObj = sm->LoadSystemAbility(systemAbilityId, loadTimeSeconds);
    return reremoteObj != nullptr;
}
} // namespace OHOS::UpdateService