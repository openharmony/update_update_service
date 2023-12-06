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

#include "update_service_module.h"

#include <cstdint>
#include <vector>

#include "../include/module_log.h"
#include "../include/module_manager.h"

int32_t RegisterFunc(std::vector<int> codes, RequestFuncType handleRemoteRequest, bool isExt)
{
    UTILS_LOGI("enter RegisterFunc");
    OHOS::UpdateEngine::ModuleManager::GetInstance().HookFunc(codes, handleRemoteRequest, isExt);
    return 0;
}

int32_t RegisterOnStartOnStopFunc(std::string phase, LifeCycleFuncType handlePhase)
{
    OHOS::UpdateEngine::ModuleManager::GetInstance().HookOnStartOnStopFunc(phase, handlePhase);
    return 0;
}

int32_t RegisterOnIdleFunc(std::string phase, LifeCycleFuncReturnType handlePhase)
{
    OHOS::UpdateEngine::ModuleManager::GetInstance().HookOnIdleFunc(phase, handlePhase);
    return 0;
}
