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

#include "update_notify.h"

#include "extension_manager_client.h"
#include "iservice_registry.h"

#include "subscribe_info.h"
#include "update_log.h"

namespace OHOS {
namespace UpdateEngine {
ErrCode UpdateNotify::ConnectAbility(const AAFwk::Want &want, const sptr<AAFwk::AbilityConnectionStub> &connect)
{
    ErrCode result =
        AAFwk::ExtensionManagerClient::GetInstance().ConnectServiceExtensionAbility(want, connect, nullptr, -1);
    ENGINE_LOGI("ConnectAbility result %{public}d", result);
    return result;
}

ErrCode UpdateNotify::DisconnectAbility(const sptr<AAFwk::AbilityConnectionStub> &connect)
{
    ErrCode result =
        AAFwk::ExtensionManagerClient::GetInstance().DisconnectAbility(connect);
    ENGINE_LOGI("DisconnectAbility result %{public}d", result);
    return result;
}

bool UpdateNotify::ConnectToAppService(const std::string &eventInfo, const std::string &subscribeInfo)
{
    if (eventInfo.empty()) {
        ENGINE_LOGE("ConnectToAppService eventInfo error.");
        return false;
    }
    std::string message = JsonBuilder().Append("{").Append("EventInfo", eventInfo)
        .Append("SubscribeInfo", subscribeInfo).Append("}").ToJson();
    return HandleMessage(message);
}

bool UpdateNotify::HandleMessage(const std::string &message)
{
    std::string bundleName = OUC_PACKAGE_NAME;
    std::string abilityName = OUC_SERVICE_EXT_ABILITY_NAME;
    AAFwk::Want want;
    want.SetElementName(bundleName, abilityName);
    want.SetParam("Timeout", OUC_TIMEOUT);
    auto connect = sptr<NotifyConnection>::MakeSptr();
    if (ConnectAbility(want, connect) != OHOS::ERR_OK) {
        ENGINE_LOGE("SendMessage, can not connect to ouc");
        return false;
    }

    MessageParcel data;
    if (!data.WriteString16(Str8ToStr16(message))) {
        ENGINE_LOGE("SendMessage, write subscribeInfo failed");
        return false;
    }
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);
    int32_t result = connect->SendMessage(CAST_INT(OucCode::OUC), data, reply, option);
    if (result != 0) {
        ENGINE_LOGE("SendMessage SendRequest, error result %{public}d", result);
        DisconnectAbility(connect);
        return false;
    }
    return true;
}

void NotifyConnection::OnAbilityConnectDone(const AppExecFwk::ElementName &element,
    const sptr<IRemoteObject> &remoteObject, int32_t resultCode)
{
    ENGINE_LOGI("OnAbilityConnectDone successfully. result %{public}d", resultCode);
    if (resultCode != ERR_OK) {
        ENGINE_LOGE("ability connect failed, error code: %{public}d", resultCode);
        return;
    }
    ENGINE_LOGI("ability connect success, ability name %{public}s", element.GetAbilityName().c_str());
    if (remoteObject == nullptr) {
        ENGINE_LOGE("get remoteObject failed");
        return;
    }
    remoteObject_ = remoteObject;
    conditionVal_.notify_all();
}

void NotifyConnection::OnAbilityDisconnectDone(const AppExecFwk::ElementName &element, int32_t resultCode)
{
    ENGINE_LOGI("OnAbilityDisconnectDone successfully. result %{public}d", resultCode);
}

int32_t NotifyConnection::SendMessage(int32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    std::unique_lock<std::mutex> uniqueLock(connectedMutex_);
    conditionVal_.wait_for(uniqueLock, std::chrono::seconds(OUC_CONNECT_TIMEOUT));
    if (remoteObject_ != nullptr) {
        return remoteObject_->SendRequest(code, data, reply, option);
    }
    return -1;
}
} // namespace UpdateEngine
} // namespace OHOS