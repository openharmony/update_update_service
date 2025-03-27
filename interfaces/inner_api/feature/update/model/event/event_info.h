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

#ifndef UPDATE_SERVICE_EVENT_INFO_H
#define UPDATE_SERVICE_EVENT_INFO_H

#include <string_ex.h>

#include "base_json_struct.h"
#include "error_message.h"
#include "event_id.h"
#include "parcel.h"
#include "task_body.h"
#include "version_component.h"

namespace OHOS::UpdateEngine {
struct EventInfo : public BaseJsonStruct, public Parcelable {
    EventId eventId = EventId::EVENT_TASK_BASE;
    TaskBody taskBody;

    EventInfo() = default;
    EventInfo(EventId id, TaskBody body) : eventId(id), taskBody(std::move(body)) {}

    JsonBuilder GetJsonBuilder() final;

    bool ReadFromParcel(Parcel &parcel);
    bool Marshalling(Parcel &parcel) const override;
    static EventInfo *Unmarshalling(Parcel &parcel);
};
} // OHOS::UpdateEngine
#endif // UPDATE_SERVICE_EVENT_INFO_H
