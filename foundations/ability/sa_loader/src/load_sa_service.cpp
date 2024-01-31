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

namespace OHOS::UpdateEngine {
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
    InitStatus();
    return LoadSa(systemAbilityId);
}
void LoadSaService::OnLoadSystemAbilitySuccess(int32_t systemAbilityId, const sptr<IRemoteObject> &remoteObject)
{
    ENGINE_LOGI("SystemAbility load Success systemAbilityId: %{public}d, IRemoteObject result: %{public}s",
        systemAbilityId, (remoteObject != nullptr) ? "succeed" : "failed");
    loadSaStatus_ = (remoteObject != nullptr) ? LoadSaStatus::SUCCESS : LoadSaStatus::FAIL;
}

void LoadSaService::OnLoadSystemAbilityFail(int32_t systemAbilityId)
{
    ENGINE_LOGE("SystemAbility Load fail the systemAbilityId is: %{public}d", systemAbilityId);
    loadSaStatus_ = LoadSaStatus::FAIL;
}

void LoadSaService::InitStatus()
{
    loadSaStatus_ = LoadSaStatus::WAIT_RESULT;
}

bool LoadSaService::CheckSaLoaded()
{
    constexpr int64_t sleepTime = 50;   // 睡眠时间 单位：毫秒
    constexpr int32_t retryTimes = 100; // 重试次数
    int32_t retry = retryTimes;
    ENGINE_LOGI("Waiting for CheckSaLoaded");
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        LoadSaStatus loadSaStatus = loadSaStatus_;
        if (loadSaStatus != LoadSaStatus::WAIT_RESULT) {
            bool isSaLoaded = loadSaStatus == LoadSaStatus::SUCCESS;
            ENGINE_LOGI("found OnLoad result: %{public}s", isSaLoaded ? "succeed" : "failed");
            return isSaLoaded;
        }
    } while (--retry >= 0);
    ENGINE_LOGE("CheckSaLoaded didn't get OnLoad result");
    return false;
}

bool LoadSaService::LoadSa(int systemAbilityId)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (sm == nullptr) {
        ENGINE_LOGE("samgr object null!");
        return false;
    }
    int32_t result = sm->LoadSystemAbility(systemAbilityId, this);
    if (result != ERR_OK) {
        ENGINE_LOGE("systemAbilityId: %{public}d, load failed, result code: %{public}d", systemAbilityId, result);
        return false;
    }
    if (!CheckSaLoaded()) {
        ENGINE_LOGE("systemAbilityId: %{public}d, CheckSaLoaded failed", systemAbilityId);
        return false;
    }
    ENGINE_LOGI("systemAbilityId: %{public}d, load succeed", systemAbilityId);
    return true;
}
} // namespace OHOS::UpdateEngine