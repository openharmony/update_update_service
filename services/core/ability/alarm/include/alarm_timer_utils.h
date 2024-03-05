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

#ifndef ALARM_TIMER_UTILS_H
#define ALARM_TIMER_UTILS_H

#include <functional>
#include <string>

#include "itimer_info.h"

namespace OHOS {
namespace UpdateEngine {
using AlarmTimerCallback = std::function<void()>;

class AlarmTimerUtils {
public:
    static uint64_t RegisterAlarm(int64_t time, const AlarmTimerCallback &callback);
    static void UnregisterAlarm(uint64_t timerId);

    static uint64_t RegisterRepeatAlarm(int64_t time, int64_t repeatTime, const AlarmTimerCallback &callback);
    static uint64_t GetSystemBootTime();

private:
    static uint64_t RegisterTimer(int64_t time, const std::shared_ptr<MiscServices::ITimerInfo> &timerInfo);

private:
    class TimerTaskInfo : public MiscServices::ITimerInfo {
    public:
        TimerTaskInfo() {};
        virtual ~TimerTaskInfo() {};
        void OnTrigger() override;
        void SetType(const int &type) override;
        void SetRepeat(bool repeat) override;
        void SetInterval(const uint64_t &interval) override;
        void SetWantAgent(std::shared_ptr<OHOS::AbilityRuntime::WantAgent::WantAgent> wantAgent) override;
        void SetCallbackInfo(const std::function<void()> &callBack);
    public:
        AlarmTimerCallback callBack_ = nullptr;
    };
};
} // namespace UpdateEngine
} // namespace OHOS
#endif // ALARM_TIMER_UTILS_H