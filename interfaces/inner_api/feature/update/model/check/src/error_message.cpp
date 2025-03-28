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

#include "error_message.h"

#include <string_ex.h>

namespace OHOS::UpdateEngine {
bool ErrorMessage::ReadFromParcel(Parcel &parcel)
{
    errorCode = parcel.ReadInt32();
    errorMessage = Str16ToStr8(parcel.ReadString16());
    return true;
}

bool ErrorMessage::Marshalling(Parcel &parcel) const
{
    parcel.WriteInt32(errorCode);
    parcel.WriteString16(Str8ToStr16(errorMessage));
    return true;
}

ErrorMessage *ErrorMessage::Unmarshalling(Parcel &parcel)
{
    ErrorMessage *errorMessage = new (std::nothrow) ErrorMessage();
    if (errorMessage == nullptr) {
        ENGINE_LOGE("Create errorMessage failed");
        return nullptr;
    }

    if (!errorMessage->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete errorMessage;
        return nullptr;
    }
    return errorMessage;
}
} // namespace OHOS::UpdateEngine
