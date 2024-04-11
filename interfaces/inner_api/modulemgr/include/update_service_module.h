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


#ifndef UPDATE_SERVICE_MODULE_H
#define UPDATE_SERVICE_MODULE_H

#include <string>

#include "message_option.h"
#include "message_parcel.h"
#include "parcel.h"
#include "refbase.h"
#include "system_ability_ondemand_reason.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

namespace OHOS::UpdateEngine {
using RequestFuncType = int32_t (*)(uint32_t code,
    OHOS::MessageParcel &data, OHOS::MessageParcel &reply, OHOS::MessageOption &option);

using LifeCycleFuncType = void (*)(const OHOS::SystemAbilityOnDemandReason &reason);
using LifeCycleFuncReturnType = int32_t (*)(const OHOS::SystemAbilityOnDemandReason &reason);

void RegisterFunc(std::vector<uint32_t> codes, RequestFuncType handleRemoteRequest);

void RegisterOnStartOnStopFunc(std::string phase, LifeCycleFuncType handlePhase);
void RegisterOnIdleFunc(std::string phase, LifeCycleFuncReturnType handlePhase);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
} // OHOS::UpdateEngine
#endif // UPDATE_SERVICE_MODULE_H
