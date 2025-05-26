/*
* Copyright (c) 2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef UPDATESERVICE_JSON_UTILS_H
#define UPDATESERVICE_JSON_UTILS_H

#include <algorithm>
#include <functional>
#include <string>
#include <unistd.h>

#include "cJSON.h"
#include "update_define.h"

namespace OHOS::UpdateService {
enum class JsonParseError {
    ERR_OK = 0,
    COMMOM_ERROR,
    MISSING_PROP,
    TYPE_ERROR
};

// 开发人员请使用该工具类封装的安全函数来获取json对象/数组，以及从json对象获取Value
// 当前获取json对象/数组的封装使用的parse方法参数：string输入、无解析回调、解析失败不报异常
class UpdateServiceJsonUtils {
public:
    template <typename T> static int32_t GetValueAndSetTo(cJSON *jsonObject, const std::string &key, T &value)
    {
        if (!jsonObject || key.empty()) {
            return CAST_INT(JsonParseError::MISSING_PROP);
        }

        cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonObject, key.c_str());
        if (item == nullptr) {
            return CAST_INT(JsonParseError::MISSING_PROP);
        }

        if (!CheckTypeAndAsign(item, value)) {
            return CAST_INT(JsonParseError::TYPE_ERROR);
        }
        return CAST_INT(JsonParseError::ERR_OK);
    }

    static cJSON *ParseAndGetJsonObject(const std::string &jsonStr)
    {
        cJSON *object = cJSON_Parse(jsonStr.c_str());
        if (object == nullptr) {
            return nullptr;
        }

        if (!cJSON_IsObject(object)) {
            cJSON_Delete(object);
            return nullptr;
        }

        return object;
    }

    static std::shared_ptr<cJSON> ParseJson(const std::string &jsonStr)
    {
        cJSON *root = cJSON_Parse(jsonStr.c_str());
        if (root == nullptr) {
            return nullptr;
        }

        return std::shared_ptr<cJSON>(root, [](cJSON *r) { cJSON_Delete(r); });
    }

private:

    static bool IsInteger(double d)
    {
        return std::floor(d) == d;
    }

    // 判断int32_t, int64_t, uint32_t, uint64_t 是否合法整数
    template <typename T> static bool CheckInteger(cJSON *jsonObj, T &value)
    {
        if (!cJSON_IsNumber(jsonObj)) {
            return false;
        }

        double objValue = jsonObj->valuedouble;
        if (!IsInteger(objValue)) {
            return false;
        }

        if (std::is_same_v<T, int32_t>) {
            if (objValue < std::numeric_limits<int32_t>::min() || objValue > std::numeric_limits<int32_t>::max()) {
                return false;
            }
        }

        if (std::is_same_v<T, int64_t>) {
            if (objValue < std::numeric_limits<int64_t>::min() || objValue > std::numeric_limits<int64_t>::max()) {
                return false;
            }
        }

        if (std::is_same_v<T, uint32_t>) {
            if (objValue < 0 || objValue > std::numeric_limits<uint32_t>::max()) {
                return false;
            }
        }

        if (std::is_same_v<T, uint64_t>) {
            if (objValue < 0 || objValue > std::numeric_limits<uint64_t>::max()) {
                return false;
            }
        }
        value = objValue;
        return true;
    }

    // 检查基本类型
    template <typename T> static bool CheckTypeAndAsign(cJSON *jsonObject, T &value)
    {
        if constexpr (std::is_same_v<T, std::string>) {
            if (cJSON_IsString(jsonObject)) {
                value = jsonObject->valuestring;
                return true;
            }
            return false;
        }

        if constexpr (std::is_same_v<T, bool>) {
            if (jsonObject->type != cJSON_True && jsonObject->type != cJSON_False) {
                return false;
            }
            value = cJSON_IsTrue(jsonObject) ? true : false;
            return true;
        }

        if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, int64_t> ||
            std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>) {
            return CheckInteger(jsonObject, value);
        }

        if constexpr (std::is_same_v<T, double>) {
            if (!cJSON_IsNumber(jsonObject)) {
                return false;
            }
            double dbValue = jsonObject->valuedouble;
            if (!std::isfinite(dbValue)) {
                return false;
            }
            value = dbValue;
            return true;
        }
        return false;
    }
};
} // namespace OHOS::UpdateService
#endif // UPDATESERVICE_JSON_UTILS_H
