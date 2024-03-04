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

#include "timer_manager.h"

#include "update_define.h"

namespace OHOS {
namespace UpdateEngine {
constexpr int32_t ONE_SECOND_MILLISECONDS = 1000;

TimerManager::TimerManager()
{
    ENGINE_LOGD("TimerManager Constructor");
    timer_ = std::make_shared<OHOS::Utils::Timer>("Update Service Timer");
    uint32_t ret = timer_->Setup();
    ENGINE_LOGD("TimerManager build timer result %{public}d", ret);
}

TimerManager::~TimerManager()
{
    ENGINE_LOGD("TimerManager Destructor");
    if (timer_ != nullptr) {
        for (auto &[alarmType, timerId] : registeredTimerIdMap_) {
            timer_->Unregister(timerId);
        }
        timer_->Shutdown();
    }
}

bool TimerManager::RegisterLooperEvent(EventType eventType, int64_t looperInterval,
    const OHOS::Utils::Timer::TimerCallback &callback)
{
    ENGINE_LOGI("TimerManager registerLooperEvent EventType %{puiblic}d", CAST_INT(eventType));
    UnregisterLooperEvent(eventType);
    auto weakThis = weak_from_this();
    if (timer_ == nullptr) {
        ENGINE_LOGE("Timer is nullptr!");
        return false;
    }
    uint32_t registerTimerId = timer_->Register(
        [weakThis, eventType, callback]() {
            if (weakThis.expired()) { // 判断宿主是否销毁
                ENGINE_LOGE("TimerManager is destroyed");
                return;
            }
            ENGINE_LOGD("Looper EventType %{publoc}d triggered", CAST_INT(eventType));
            callback();
        },
        looperInterval * ONE_SECOND_MILLISECONDS, false);
    std::lock_guard<std::mutex> lockGuard(mutex_);
    registeredTimerIdMap_.emplace(eventType, registerTimerId);
    ENGINE_LOGD("TimerManager registerLooperEvent EventType %{public}d success", CAST_INT(eventType));
    return true;
}

void TimerManager::UnregisterLooperEvent(EventType eventType)
{
    ENGINE_LOGI("TimerManager UnregisterLooperEvent EventType %{puiblic}d", CAST_INT(eventType));
    std::lock_guard<std::mutex> lockGuard(mutex_);
    auto result = registeredTimerIdMap_.find(eventType);
    if (result == registeredTimerIdMap_.end()) {
        ENGINE_LOGD("EventType %{public}d has not been registered", CAST_INT(eventType));
        return;
    }
    if (timer_ != nullptr) {
        timer_->Unregister(result->second);
    } else {
        ENGINE_LOGE("Timer is nullptr");
    }
    registeredTimerIdMap_.erase(result);
    ENGINE_LOGD("TimerManager UnregisterLooperEvent EventType %{public}d success", CAST_INT(eventType));
}
} // namespace UpdateEngine
} // namespace OHOS
