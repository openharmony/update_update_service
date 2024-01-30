/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "subscribe_info.h"

namespace OHOS::UpdateEngine {
JsonBuilder SubscribeInfo::GetJsonBuilder()
{
    return JsonBuilder()
        .Append("{")
        .Append("upgradeApp", upgradeApp)
        .Append("businessType", businessType.GetJsonBuilder())
        .Append("abilityName", abilityName)
        .Append("subscriberDevId", subscriberDevId)
        .Append("upgradeDevId", upgradeDevId)
        .Append("deviceType", CAST_INT(deviceType))
        .Append("deviceName", deviceName)
        .Append("}");
}
} // namespace OHOS::UpdateEngine