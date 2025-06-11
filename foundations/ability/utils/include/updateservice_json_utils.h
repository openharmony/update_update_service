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
        if (!item) {
            return CAST_INT(JsonParseError::MISSING_PROP);
        }

        if (!CheckBaseType(item, value)) {
            return CAST_INT(JsonParseError::TYPE_ERROR);
        }
        GetValue(item, value);
        return CAST_INT(JsonParseError::ERR_OK);
    }

    static cJSON *ParseAndGetJsonObject(const std::string &jsonStr)
    {
        cJSON *object = cJSON_Parse(jsonStr.c_str());
        if (!object) {
            return nullptr;
        }

        if (!cJSON_IsObject(object)) {
            cJSON_Delete(object);
            return nullptr;
        }

        return object;
    }

private:

    // 检查基本类型
    template <typename T> static bool CheckBaseType(cJSON *jsonObject, T &value)
    {
        if (!jsonObject) {
            return false;
        }

        if constexpr (std::is_same_v<T, std::string>) {
            return  cJSON_IsString(jsonObject);
        } else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, int64_t> ||
            std::is_same_v<T, double>) {
            return cJSON_IsNumber(jsonObject);
        } else if constexpr (std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>) {
            return cJSON_IsNumber(jsonObject) && jsonObject->valuedouble >=0;
        } else if constexpr (std::is_same_v<T, bool>) {
            return (jsonObject->type == cJSON_True || jsonObject->type == cJSON_False);
        }  else {
            return false;
        }
    }

    template <typename T>
    static bool CheckArrayType(cJSON *jsonObject, std::vector<T> &value)
    {
        return cJSON_IsArray(jsonObject);
    }

    static void GetValueVecString(const cJSON *jsonArray, std::vector<std::string> &value)
    {
        if (!jsonArray || !cJSON_IsArray(jsonArray)) {
            return;
        }

        const cJSON *element = nullptr;
        cJSON_ArrayForEach(element, jsonArray) {
            if (cJSON_IsString(element) && element->valuestring != nullptr) {
                value.push_back(element->valuestring);
            }
        }
    }

    static void GetValueVecInt(const cJSON *jsonArray, std::vector<int> &value)
    {
        if (!jsonArray || !cJSON_IsArray(jsonArray)) {
            return;
        }

        const cJSON *element = nullptr;
        cJSON_ArrayForEach(element, jsonArray) {
            if (cJSON_IsNumber(element)) {
                value.push_back(element->valueint);
            }
        }
    }

    template <typename T>
    static void GetValue(cJSON *item, T &value)
    {
        if (!item) {
            return;
        }

        if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
            if (cJSON_IsNumber(item)) {
                (sizeof(T) <= sizeof(int)) ? value = static_cast<T>(item->valueint) :
                    value = static_cast<T>(item->valuedouble);
            }
        } else if constexpr (std::is_floating_point_v<T>) {
            if (cJSON_IsNumber(item)) {
                value = static_cast<T>(item->valuedouble);
            }
        } else if constexpr (std::is_same_v<T, bool>) {
            value = cJSON_IsTrue(item) ? true : false;
        } else if constexpr (std::is_same_v<T, std::string>) {
            if (cJSON_IsString(item)) {
                value = item->valuestring;
            }
        } else if constexpr (std::is_same_v<T, std::vector<int>>) {
            GetValueVecInt(item, value);
        } else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
            GetValueVecString(item, value);
        } else {
            return;
        }
    }
};
} // namespace OHOS::UpdateService
#endif // UPDATESERVICE_JSON_UTILS_H
