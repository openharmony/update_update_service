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

#include <dlfcn.h>
#include <map>
#include <thread>

#include "../include/module_log.h"
#include "../include/module_manager.h"

namespace OHOS {
namespace UpdateEngine {
std::set<int> ModuleManager::extCodesSet_;
std::map<int, RequestFuncType> ModuleManager::onRemoteRequestFuncMap_;
std::map<std::string, LifeCycleFuncType> ModuleManager::onStartOnStopFuncMap_;
std::map<std::string, LifeCycleFuncReturnType> ModuleManager::onIdleFuncMap_;

bool ModuleManager::isLoaded = false;

void ModuleManager::LoadModule(std::string libPath)
{
    std::string prefix = "/system/lib64/updateext";
    std::string suffix = ".so";
    if ((libPath.substr(0, prefix.length()) != prefix) ||
            (libPath.substr(libPath.length() - suffix.length(), suffix.length()) != suffix)) {
        UTILS_LOGE("LoadModule lib path invalid");
        return;
    }
    UTILS_LOGD("LoadModule so path: %{public}s", libPath.c_str());
    if (dueModuleHandler == nullptr) {
        constexpr int32_t maxRetryTimes = 1;
        int32_t retryTimes = 0;
        do {
            dueModuleHandler = dlopen(libPath.c_str(), RTLD_LAZY);
            if (dueModuleHandler != nullptr) {
                isLoaded = true;
                break;
            }
            UTILS_LOGE("openSo path: %{public}s fail", libPath.c_str());
            retryTimes++;
            if (retryInterval_ > 0 && retryTimes <= maxRetryTimes) {
                std::this_thread::sleep_for(std::chrono::milliseconds(retryInterval_));
            }
        } while (retryTimes <= maxRetryTimes);
    } else {
        isLoaded = true;
        UTILS_LOGD("openSo ok");
    }
}

ModuleManager& ModuleManager::GetInstance()
{
    static ModuleManager moduleManager_;
    return moduleManager_;
}

void ModuleManager::HookFunc(std::vector<int> codes, RequestFuncType handleRemoteRequest, bool isExt)
{
    if (isExt) {
        extCodesSet_.insert(codes.begin(), codes.end());
    }
    for (int code : codes) {
        if (onRemoteRequestFuncMap_.find(code) == onRemoteRequestFuncMap_.end()) {
            UTILS_LOGE("code not exist %{public}d onRemoteRequestFuncMap_", code);
            onRemoteRequestFuncMap_.insert(std::make_pair(code, handleRemoteRequest));
        } else {
            UTILS_LOGD("add code %{public}d to onRemoteRequestFuncMap_", code);
            onRemoteRequestFuncMap_[code] = handleRemoteRequest;
        }
    }
}

int32_t ModuleManager::HandleFunc(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    if (onRemoteRequestFuncMap_.find(code) == onRemoteRequestFuncMap_.end()) {
        UTILS_LOGE("HandleFunc code %{public}d not exist", code);
    } else {
        UTILS_LOGD("HandleFunc code %{public}d exist", code);
        return ((RequestFuncType)onRemoteRequestFuncMap_[code])(code, data, reply, option);
    }
    return 0;
}

ModuleManager::ModuleManager() {}

bool ModuleManager::IsModuleLoaded()
{
    return isLoaded;
}

void ModuleManager::HookOnStartOnStopFunc(std::string phase, LifeCycleFuncType handleSAOnStartOnStop)
{
    if (onStartOnStopFuncMap_.find(phase) == onStartOnStopFuncMap_.end()) {
        UTILS_LOGE("phase exist already %{public}s onStartOnStopFuncMap_", phase.c_str());
        onStartOnStopFuncMap_.insert(std::make_pair(phase, handleSAOnStartOnStop));
    } else {
        UTILS_LOGD("add phase %{public}s to onStartOnStopFuncMap_", phase.c_str());
        onStartOnStopFuncMap_[phase] = handleSAOnStartOnStop;
    }
}

int32_t ModuleManager::HandleOnStartOnStopFunc(std::string phase, const OHOS::SystemAbilityOnDemandReason &reason)
{
    if (onStartOnStopFuncMap_.find(phase) == onStartOnStopFuncMap_.end()) {
        UTILS_LOGE("HandleOnStartOnStopFunc phase %{public}s not exist", phase.c_str());
    } else {
        UTILS_LOGD("HandleOnStartOnStopFunc phase %{public}s exist", phase.c_str());
        ((LifeCycleFuncType)onStartOnStopFuncMap_[phase])(reason);
    }
    return 0;
}

void ModuleManager::HookOnIdleFunc(std::string phase, LifeCycleFuncReturnType handleSAOnIdle)
{
    if (onIdleFuncMap_.find(phase) == onIdleFuncMap_.end()) {
        UTILS_LOGE("phase exist already %{public}s onIdleFuncMap_", phase.c_str());
        onIdleFuncMap_.insert(std::make_pair(phase, handleSAOnIdle));
    } else {
        UTILS_LOGD("add phase %{public}s to onIdleFuncMap_", phase.c_str());
        onIdleFuncMap_[phase] = handleSAOnIdle;
    }
}

int32_t ModuleManager::HandleOnIdleFunc(std::string phase, const OHOS::SystemAbilityOnDemandReason &reason)
{
    if (onIdleFuncMap_.find(phase) == onIdleFuncMap_.end()) {
        UTILS_LOGE("HandleOnIdleFunc phase %{public}s not exist", phase.c_str());
    } else {
        UTILS_LOGI("HandleOnIdleFunc phase %{public}s exist", phase.c_str());
        ((LifeCycleFuncReturnType)onIdleFuncMap_[phase])(reason);
    }
    return 0;
}
} // namespace UpdateEngine
} // namespace OHOS
