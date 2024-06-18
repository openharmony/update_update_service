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

#ifndef UPDATE_NOTIFY_H
#define UPDATE_NOTIFY_H

#include <condition_variable>
#include <mutex>
#include <string>
#include "ability_connect_callback_stub.h"
#include "ability_connect_callback_interface.h"
#include "ability_manager_interface.h"
#include "ability_manager_client.h"
#include "if_system_ability_manager.h"
#include "i_update_notify.h"
#include "system_ability_definition.h"
#include "update_no_constructor.h"
#include "want.h"

namespace OHOS {
namespace UpdateEngine {
class UpdateNotify : public IRemoteStub<IUpdateNotify> {
public:
    DISALLOW_COPY_AND_MOVE(UpdateNotify);

    UpdateNotify();
    ~UpdateNotify();
    static sptr<UpdateNotify> GetInstance();
    bool ConnectToAppService(const std::string &eventInfo, const std::string &subscribeInfo);

    void HandleAbilityConnect(const sptr<IRemoteObject> &remoteObject);

private:
    bool HandleMessage(const std::string &message);
    ErrCode ConnectAbility(const AAFwk::Want &want, const sptr<AAFwk::AbilityConnectionStub> &connect);
    ErrCode DisconnectAbility(const sptr<AAFwk::AbilityConnectionStub> &connect);

private:
    static std::mutex instanceLock_;
    static sptr<UpdateNotify> instance_;
    sptr<IRemoteObject> remoteObject_ = nullptr;

    std::mutex connectMutex_;
    std::condition_variable  conditionVal_;

    enum class OucCode {
        UNKNOWN = 0,
        OUC = 5
    };
};

class NotifyConnection : public AAFwk::AbilityConnectionStub {
public:
    explicit NotifyConnection(const sptr<UpdateNotify> &instance);
    ~NotifyConnection() = default;

    void OnAbilityConnectDone(const AppExecFwk::ElementName &element, const sptr<IRemoteObject> &remoteObject,
        int32_t resultCode) override;
    void OnAbilityDisconnectDone(const AppExecFwk::ElementName &element, int resultCode) override;

private:
    sptr<UpdateNotify> instance_ = nullptr;

};
} // namespace UpdateEngine
} // namespace OHOS
#endif // UPDATE_NOTIFY_H