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

#ifndef BASE_SERVICE_KITS_IMPL_H
#define BASE_SERVICE_KITS_IMPL_H

#include <mutex>

#ifndef ABILITY_RUNTIME_INNER_ENABLE
#include "ability_manager_proxy.h"
#endif
#include "iremote_object.h"

#include "common_death_recipient.h"
#include "load_sa_service.h"
#include "update_define.h"

namespace OHOS::UpdateEngine {
#define RETURN_FAIL_WHEN_SERVICE_NULL(service) \
    if ((service) == nullptr) {                \
        ENGINE_LOGE("Service is null");        \
        return INT_CALL_IPC_ERR;               \
    }

template <typename SERVICE> class BaseServiceKitsImpl {
public:
    explicit BaseServiceKitsImpl(int32_t systemAbilityId, bool isNeedAddDeathRecipient = true)
        : systemAbilityId_(systemAbilityId), isNeedAddDeathRecipient_(isNeedAddDeathRecipient) {}
    virtual ~BaseServiceKitsImpl();

protected:
    sptr<SERVICE> GetService();
    void ResetRemoteService();
    virtual void RegisterCallback(){};

protected:
    std::recursive_mutex remoteServerLock_;
    sptr<SERVICE> remoteServer_ = nullptr;
    sptr<IRemoteObject::DeathRecipient> deathRecipient_ = nullptr;
    int32_t systemAbilityId_;

private:
    void AddDeathRecipient(const sptr<IRemoteObject> &object);
    void ResetService(const wptr<IRemoteObject> &remote);

private:
    bool isNeedAddDeathRecipient_ = true;
};

template <typename SERVICE> BaseServiceKitsImpl<SERVICE>::~BaseServiceKitsImpl()
{
    ENGINE_LOGI("BaseServiceKitsImpl desConstructor");
    std::lock_guard<std::recursive_mutex> lock(remoteServerLock_);
    if (isNeedAddDeathRecipient_ && remoteServer_ != nullptr && deathRecipient_ != nullptr) {
        sptr<IRemoteObject> object = remoteServer_->AsObject();
        if (object != nullptr) {
            bool removeResult = object->RemoveDeathRecipient(deathRecipient_);
            ENGINE_LOGI("RemoveDeathRecipient result is %{public}s", removeResult ? "success" : "error");
        }
    }
    deathRecipient_ = nullptr;
    remoteServer_ = nullptr;
}

template <typename SERVICE> sptr<SERVICE> BaseServiceKitsImpl<SERVICE>::GetService()
{
    ENGINE_LOGI("GetService entry");
    std::lock_guard<std::recursive_mutex> lock(remoteServerLock_);
    if (remoteServer_ != nullptr) {
        return remoteServer_;
    }
    ENGINE_LOGI("GetService recreate service instance");
    sptr<ISystemAbilityManager> manager = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    ENGINE_CHECK(manager != nullptr, return nullptr, "manager is nullptr");
    sptr<IRemoteObject> object = manager->CheckSystemAbility(systemAbilityId_);
    if (object == nullptr) {
        ENGINE_CHECK(LoadSaService::GetInstance()->TryLoadSa(systemAbilityId_), return nullptr, "TryLoadSa fail");
        object = manager->GetSystemAbility(systemAbilityId_);
        ENGINE_CHECK(object != nullptr, return nullptr, "Get  remote object from samgr failed");
    }
    AddDeathRecipient(object);
    remoteServer_ = iface_cast<SERVICE>(object);
    ENGINE_CHECK(remoteServer_ != nullptr, return nullptr, "service iface_cast failed");
    RegisterCallback();
    return remoteServer_;
}

template <typename SERVICE> void BaseServiceKitsImpl<SERVICE>::AddDeathRecipient(const sptr<IRemoteObject> &object)
{
    if (isNeedAddDeathRecipient_ && object != nullptr) {
        if (deathRecipient_ == nullptr) {
            auto resetCallback = [this](const wptr<IRemoteObject> &remote) { this->ResetService(remote); };
            deathRecipient_ = new CommonDeathRecipient(resetCallback);
        }
        if ((object->IsProxyObject()) && (!object->AddDeathRecipient(deathRecipient_))) {
            ENGINE_LOGE("Failed to add death recipient");
        }
    }
}

template <typename SERVICE> void BaseServiceKitsImpl<SERVICE>::ResetRemoteService()
{
    ENGINE_LOGI("ResetRemoteService, do ResetService");
    std::lock_guard<std::recursive_mutex> lock(remoteServerLock_);
    if (remoteServer_ != nullptr) {
        remoteServer_ = nullptr;
    }
}

template <typename SERVICE> void BaseServiceKitsImpl<SERVICE>::ResetService(const wptr<IRemoteObject> &remote)
{
    ENGINE_LOGI("Remote is dead, do ResetService");
    std::lock_guard<std::recursive_mutex> lock(remoteServerLock_);
    if (deathRecipient_ != nullptr && remote != nullptr) {
        remote->RemoveDeathRecipient(deathRecipient_);
    }
    remoteServer_ = nullptr;
    deathRecipient_ = nullptr;
}
}
#endif // BASE_SERVICE_KITS_IMPL_H
