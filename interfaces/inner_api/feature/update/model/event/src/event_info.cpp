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

#include "event_info.h"
#include "update_define.h"
#include "update_log.h"

namespace OHOS::UpdateService {
JsonBuilder EventInfo::GetJsonBuilder()
{
    return JsonBuilder()
        .Append("{")
        .Append("eventId", CAST_INT(eventId))
        .Append("taskBody", taskBody.GetJsonBuilder(eventId))
        .Append("}");
}

bool EventInfo::ReadFromParcel(Parcel &parcel)
{
    eventId = static_cast<EventId>(parcel.ReadUint32());
    sptr<TaskBody> unmarshallingTaskBody = TaskBody().Unmarshalling(parcel);
    if (unmarshallingTaskBody != nullptr) {
        taskBody = *unmarshallingTaskBody;
    } else {
        ENGINE_LOGE("unmarshallingTaskBody fail");
        return false;
    }
    return true;
}

bool EventInfo::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteUint32(static_cast<uint32_t>(eventId))) {
        ENGINE_LOGE("WriteBool eventId failed");
        return false;
    }
    taskBody.Marshalling(parcel);
    return true;
}

EventInfo *EventInfo::Unmarshalling(Parcel &parcel)
{
    EventInfo *eventInfo = new (std::nothrow) EventInfo();
    if (eventInfo == nullptr) {
        ENGINE_LOGE("Create eventInfo failed");
        return nullptr;
    }

    if (!eventInfo->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete eventInfo;
        return nullptr;
    }
    return eventInfo;
}
} // namespace OHOS::UpdateService