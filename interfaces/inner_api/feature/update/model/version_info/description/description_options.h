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

#ifndef UPDATE_SERVICE_DESCRIPTION_OPTIONS_H
#define UPDATE_SERVICE_DESCRIPTION_OPTIONS_H

#include <string>
#include <string_ex.h>

#include "description_format.h"
#include "parcel.h"

namespace OHOS::UpdateEngine {
struct DescriptionOptions : public Parcelable {
    DescriptionFormat format = DescriptionFormat::STANDARD;
    std::string language;

    bool ReadFromParcel(Parcel &parcel);
    bool Marshalling(Parcel &parcel) const override;
    static DescriptionOptions *Unmarshalling(Parcel &parcel);
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_DESCRIPTION_OPTIONS_H
