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

#include "update_callback.h"

#include "update_log.h"

namespace OHOS::UpdateEngine {
UpdateCallback::UpdateCallback(const UpdateCallbackInfo &updateCallback) : updateCallback_(updateCallback) {}

void UpdateCallback::OnEvent(const EventInfo &eventInfo)
{
    ENGINE_LOGI("OnEvent eventId 0x%{public}08x", eventInfo.eventId);
    if (updateCallback_.onEvent != nullptr) {
        updateCallback_.onEvent(eventInfo);
    }
}
} // namespace OHOS::UpdateEngine
