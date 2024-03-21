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

#include "alarm_timer_utils.h"

#include <cinttypes>

#include "constant.h"
#include "time_service_client.h"
#include "time_utils.h"
#include "update_log.h"

namespace OHOS {
namespace UpdateEngine {
void AlarmTimerUtils::TimerTaskInfo::OnTrigger()
{
    ENGINE_LOGI("timed task had been triggered");
    if (callBack_ != nullptr) {
        callBack_();
    }
}

void AlarmTimerUtils::TimerTaskInfo::SetType(const int &type)
{
    this->type = type;
}

void AlarmTimerUtils::TimerTaskInfo::SetRepeat(bool repeat)
{
    this->repeat = repeat;
}

void AlarmTimerUtils::TimerTaskInfo::SetInterval(const uint64_t &interval)
{
    this->interval = interval;
}

void AlarmTimerUtils::TimerTaskInfo::SetWantAgent(std::shared_ptr<OHOS::AbilityRuntime::WantAgent::WantAgent> wantAgent)
{
    this->wantAgent = wantAgent;
}

void AlarmTimerUtils::TimerTaskInfo::SetCallbackInfo(const std::function<void()> &callBack)
{
    this->callBack_ = callBack;
}

uint64_t AlarmTimerUtils::RegisterAlarm(int64_t time, const AlarmTimerCallback &callback)
{
    auto timerInfo = std::make_shared<TimerTaskInfo>();
    uint8_t timerType =
        static_cast<uint8_t>(timerInfo->TIMER_TYPE_EXACT) | static_cast<uint8_t>(timerInfo->TIMER_TYPE_WAKEUP);
    timerInfo->SetType(static_cast<int>(timerType));
    timerInfo->SetRepeat(false);
    timerInfo->SetCallbackInfo(callback);

    int64_t currentTime = TimeUtils::GetTimestamp();
    if (currentTime >= time) {
        ENGINE_LOGE("register time is outdated, register time %{public}" PRIu64 "", time);
        return 0;
    }
    int64_t tiggerTimeMs = time * Constant::MILLESECONDS;
    uint64_t timerId = RegisterTimer(tiggerTimeMs, timerInfo);
    return timerId;
}

uint64_t AlarmTimerUtils::RegisterRepeatAlarm(int64_t time, int64_t repeatTime, const AlarmTimerCallback &callback)
{
    ENGINE_LOGI("register repeat timer, start time %{public}" PRId64 ", repeat %{public}" PRId64 "", time, repeatTime);
    auto timerInfo = std::make_shared<TimerTaskInfo>();
    uint8_t timerType = static_cast<uint8_t>(timerInfo->TIMER_TYPE_REALTIME);
    timerInfo->SetType(static_cast<int>(timerType));
    timerInfo->SetRepeat(true);
    int64_t repeatTimeMs = repeatTime * Constant::MILLESECONDS;
    timerInfo->SetInterval(repeatTimeMs);
    timerInfo->SetCallbackInfo(callback);
    uint64_t timerId = RegisterTimer(time, timerInfo);
    return timerId;
}

uint64_t AlarmTimerUtils::RegisterTimer(int64_t time, const std::shared_ptr<MiscServices::ITimerInfo> &timerInfo)
{
    sptr<MiscServices::TimeServiceClient> systemTimer = MiscServices::TimeServiceClient::GetInstance();
    uint64_t timerId = systemTimer->CreateTimer(timerInfo);
    if (timerId == 0) {
        ENGINE_LOGE("CreateTimer failed, register tim %{public}" PRId64 "", time);
        return 0;
    }
    // 将秒时间戳改为毫秒时间戳, alarm定时器不支持秒为单位的时间戳
    if (!systemTimer->StartTimer(timerId, static_cast<uint64_t>(time))) {
        ENGINE_LOGE("StartTimer failed, register time %{public}" PRId64"", time);
        return 0;
    }
    ENGINE_LOGI("register timer success, register time %{public}" PRId64 ", timerId %{public}" PRIu64 "",
        time, timerId);
    return timerId;
}

void AlarmTimerUtils::UnregisterAlarm(uint64_t timerId)
{
    ENGINE_LOGI("UnregisterAlarm, timerId %{public}" PRIu64 "", timerId);
    sptr<MiscServices::TimeServiceClient> systemTimer = MiscServices::TimeServiceClient::GetInstance();
    if (systemTimer != nullptr) {
        systemTimer->DestroyTimer(timerId);
    }
}

uint64_t AlarmTimerUtils::GetSystemBootTime()
{
    int64_t currentBootTime = 0;
    int32_t ret = MiscServices::TimeServiceClient::GetInstance()->GetBootTimeMs(currentBootTime);
    if (ret != 0) {
        ENGINE_LOGE("failed to get boot time");
        return 0;
    }
    uint64_t bootTime = static_cast<uint64_t>(currentBootTime);
    ENGINE_LOGI("GetSystemBootTime bootTime : %{public}" PRIu64 "", bootTime);
    return bootTime;
}
} // namespace UpdateEngine
} // namespace OHOS
