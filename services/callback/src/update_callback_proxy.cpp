/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "update_callback_proxy.h"

#include "event_info.h"
#include "message_parcel_helper.h"
#include "update_log.h"

namespace OHOS::UpdateEngine {
void UpdateCallbackProxy::OnEvent(const EventInfo &eventInfo)
{
    ENGINE_LOGI("UpdateCallbackProxy::OnEvent");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option { MessageOption::TF_SYNC };

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        ENGINE_LOGE("UpdateCallbackProxy WriteInterfaceToken fail");
        return;
    }

    auto remote = Remote();
    ENGINE_CHECK(remote != nullptr, return, "Can not get remote");

    int32_t result = MessageParcelHelper::WriteEventInfo(data, eventInfo);
    ENGINE_CHECK(result == 0, return, "Can not WriteEventInfo");

    result = remote->SendRequest(ON_EVENT, data, reply, option);
    ENGINE_CHECK(result == ERR_OK, return, "Can not SendRequest");
    return;
}
} // namespace OHOS::UpdateEngine

