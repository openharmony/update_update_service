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

#ifndef UPDATE_SERVICE_BUSINESS_TYPE_H
#define UPDATE_SERVICE_BUSINESS_TYPE_H

#include <string>

#include "base_json_struct.h"
#include "business_sub_type.h"
#include "business_vendor.h"
#include "update_define.h"

namespace OHOS::UpdateEngine {
struct BusinessType : public BaseJsonStruct {
    std::string vendor; // BusinessVendor
    BusinessSubType subType = BusinessSubType::FIRMWARE;

    bool operator<(const BusinessType &businessType) const
    {
        if (vendor != businessType.vendor) {
            return vendor < businessType.vendor;
        }

        if (subType != businessType.subType) {
            return CAST_INT(subType) < CAST_INT(businessType.subType);
        }

        return false;
    }

    bool operator!=(const BusinessType &businessType) const
    {
        return vendor != businessType.vendor || CAST_INT(subType) != CAST_INT(businessType.subType);
    }

    JsonBuilder GetJsonBuilder() final;
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_BUSINESS_TYPE_H
