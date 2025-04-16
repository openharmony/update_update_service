/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "task_info.h"
#include "update_log.h"

namespace OHOS::UpdateService {
bool TaskInfo::ReadFromParcel(Parcel &parcel)
{
    existTask = parcel.ReadBool();
    sptr<TaskBody> unmarshallingTaskBody = TaskBody().Unmarshalling(parcel);
    if (unmarshallingTaskBody != nullptr) {
        taskBody = *unmarshallingTaskBody;
    } else {
        ENGINE_LOGE("unmarshallingTaskBody fail");
        return false;
    }
    return true;
}

bool TaskInfo::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteBool(existTask)) {
        ENGINE_LOGE("WriteBool existTask failed");
        return false;
    }
    taskBody.Marshalling(parcel);
    return true;
}

TaskInfo *TaskInfo::Unmarshalling(Parcel &parcel)
{
    TaskInfo *taskInfo = new (std::nothrow) TaskInfo();
    if (taskInfo == nullptr) {
        ENGINE_LOGE("Create taskInfo failed");
        return nullptr;
    }

    if (!taskInfo->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete taskInfo;
        return nullptr;
    }
    return taskInfo;
}
} // namespace OHOS::UpdateService
