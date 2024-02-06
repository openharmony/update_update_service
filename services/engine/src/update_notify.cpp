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

#include "iservice_registry.h"

#include "subscribe_info.h"
#include "update_log.h"

namespace OHOS {
namespace UpdateEngine {
ErrCode UpdateNotify::StartAbility(const AAFwk::Want &want)
{
    ErrCode result = AAFwk::AbilityManagerClient::GetInstance()->StartAbility(want);
    ENGINE_LOGI("StartAbility result %{public}d", result);
    return result;
}

ErrCode UpdateNotify::StopServiceAbility(const AAFwk::Want &want)
{
    ErrCode result = AAFwk::AbilityManagerClient::GetInstance()->StopServiceAbility(want);
    ENGINE_LOGI("StopServiceAbility result %{public}d", result);
    return result;
}

ErrCode UpdateNotify::ConnectAbility(const AAFwk::Want &want, const sptr<AAFwk::IAbilityConnection> &connect,
    const sptr<IRemoteObject> &callerToken)
{
    ErrCode result = AAFwk::AbilityManagerClient::GetInstance()->ConnectAbility(want, connect, callerToken);
    ENGINE_LOGI("ConnectAbility result %{public}d", result);
    return result;
}

ErrCode UpdateNotify::DisconnectAbility(const sptr<AAFwk::IAbilityConnection> &connect)
{
    ErrCode result = AAFwk::AbilityManagerClient::GetInstance()->DisconnectAbility(connect);
    ENGINE_LOGI("DisconnectAbility result %{public}d", result);
    return result;
}

AAFwk::Want UpdateNotify::MakeWant(const std::string &deviceId, const std::string &abilityName,
    const std::string &bundleName, const std::string &subscribeInfo, const std::string &params)
{
    AppExecFwk::ElementName element(deviceId, bundleName, abilityName);
    AAFwk::Want want;
    want.SetElement(element);
    want.SetParam("EventInfo", params);
    want.SetParam("SubscribeInfo", subscribeInfo);
    return want;
}

bool UpdateNotify::NotifyToAppService(const std::string &eventInfo, const std::string &subscribeInfo)
{
    if (eventInfo.empty()) {
        ENGINE_LOGE("NotifyToAppService eventInfo error.");
        return false;
    }
    std::string bundleName = OUC_PACKAGE_NAME;
    std::string abilityName = OUC_SERVICE_EXT_ABILITY_NAME;
    AAFwk::Want want = MakeWant("", abilityName, bundleName, subscribeInfo, eventInfo);
    return StartAbility(want) == OHOS::ERR_OK;
}

void NotifyConnection::OnAbilityConnectDone(const AppExecFwk::ElementName &element,
    const sptr<IRemoteObject> &remoteObject, int32_t resultCode)
{
    ENGINE_LOGI("OnAbilityConnectDone successfully. result %{public}d", resultCode);
}

void NotifyConnection::OnAbilityDisconnectDone(const AppExecFwk::ElementName &element, int resultCode)
{
    ENGINE_LOGI("OnAbilityDisconnectDone successfully. result %{public}d", resultCode);
}
} // namespace UpdateEngine
} // namespace OHOS