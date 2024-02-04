/*
* Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <algorithm>
#include <functional>
#include <string>

#include "nlohmann/json.hpp"

#include "update_define.h"

namespace OHOS::UpdateEngine {
enum class JsonParseError {
    ERR_OK = 0,
    COMMOM_ERROR,
    MISSING_PROP,
    TYPE_ERROR
};

// 开发人员请使用该工具类封装的安全函数来获取json对象/数组，以及从json对象获取Value
// 当前获取json对象/数组的封装使用的parse方法参数：string输入、无解析回调、解析失败不报异常
class JsonUtils {
public:
    template <typename T>
    static int32_t GetValueAndSetTo(const nlohmann::json &jsonObject, const std::string &key, T &value)
    {
        if (jsonObject.find(key) == jsonObject.end()) {
            return CAST_INT(JsonParseError::MISSING_PROP);
        }
        if (!CheckType(jsonObject.at(key), value)) {
            return CAST_INT(JsonParseError::TYPE_ERROR);
        }
        GetValue(jsonObject, key, value);
        return CAST_INT(JsonParseError::ERR_OK);
    }

    static bool ParseAndGetJsonObject(const std::string &jsonStr, nlohmann::json &root)
    {
        root = nlohmann::json::parse(jsonStr, nullptr, false);
        if (root.is_discarded() || !root.is_object()) {
            return false;
        }
        return true;
    }

    static bool ParseAndGetJsonArray(const std::string &jsonStr, nlohmann::json &root)
    {
        root = nlohmann::json::parse(jsonStr, nullptr, false);
        if (root.is_discarded() || !root.is_array()) {
            return false;
        }
        return true;
    }

    static int32_t GetValueAndSetToArray(const nlohmann::json &jsonObject, const std::string &key,
        nlohmann::json &value)
    {
        if (jsonObject.find(key) == jsonObject.end()) {
            return CAST_INT(JsonParseError::MISSING_PROP);
        }
        if (!jsonObject.at(key).is_array()) {
            return CAST_INT(JsonParseError::TYPE_ERROR);
        }
        jsonObject.at(key).get_to(value);
        return CAST_INT(JsonParseError::ERR_OK);
    }

    static void SetJsonToVector(nlohmann::json &jsonObject, std::vector<std::string> &vector)
    {
        if (jsonObject.is_array()) {
            for (nlohmann::json::iterator it = jsonObject.begin(); it != jsonObject.end(); ++it) {
                if (!it.value().is_string()) {
                    continue;
                }
                vector.push_back(static_cast<std::string>(it.value()));
            }
        }
    }

    static void SetJsonToVector(const nlohmann::json &jsonObject, const std::string &key,
        std::vector<std::string> &vector)
    {
        if (!IsArray(jsonObject, key)) {
            return;
        }
        nlohmann::json jsonArray = jsonObject.at(key);
        SetJsonToVector(jsonArray, vector);
    }

    static bool IsArray(const nlohmann::json &jsonObject, const std::string &key)
    {
        if (jsonObject.find(key) == jsonObject.end()) {
            return false;
        }
        return jsonObject.at(key).is_array();
    }

    template <typename T> static std::string StructToJsonStr(const T &value)
    {
        nlohmann::json jsonObj(value);
        return jsonObj.dump();
    }

    template <typename T> static int32_t JsonStrToStruct(const std::string &jsonStr, T &value)
    {
        if (jsonStr.empty()) {
            return CAST_INT(JsonParseError::COMMOM_ERROR);
        }
        nlohmann::json jsonObj = nlohmann::json::parse(jsonStr, nullptr, false);
        if (!jsonObj.is_discarded() && CheckType(jsonObj, value)) {
            value = jsonObj.get<T>();
            return CAST_INT(JsonParseError::ERR_OK);
        }
        return CAST_INT(JsonParseError::TYPE_ERROR);
    }

private:
    static bool CheckType(const nlohmann::json &jsonObject, std::string &value)
    {
        return jsonObject.is_string();
    }

    static bool CheckType(const nlohmann::json &jsonObject, int32_t &value)
    {
        return jsonObject.is_number();
    }

    static bool CheckType(const nlohmann::json &jsonObject, uint32_t &value)
    {
        return jsonObject.is_number();
    }

    static bool CheckType(const nlohmann::json &jsonObject, uint64_t &value)
    {
        return jsonObject.is_number();
    }

    static bool CheckType(const nlohmann::json &jsonObject, int64_t &value)
    {
        return jsonObject.is_number();
    }

    static bool CheckType(const nlohmann::json &jsonObject, double &value)
    {
        return jsonObject.is_number();
    }

    static bool CheckType(const nlohmann::json &jsonObject, bool &value)
    {
        return jsonObject.is_boolean();
    }

    template <typename T> static bool CheckType(const nlohmann::json &jsonObject, T &value)
    {
        return jsonObject.is_object();
    }

    template <typename T> static bool CheckType(const nlohmann::json &jsonObject, std::vector<T> &value)
    {
        return jsonObject.is_array();
    }

    template <typename T> static void GetValue(const nlohmann::json &jsonObject, const std::string &key, T &value)
    {
        jsonObject.at(key).get_to(value);
    }

    static void GetValue(const nlohmann::json &jsonObject, const std::string &key, std::vector<std::string> &value)
    {
        if (!IsArray(jsonObject, key)) {
            return;
        }
        nlohmann::json jsonArray = jsonObject.at(key);
        for (nlohmann::json::iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
            if (!it.value().is_string()) {
                continue;
            }
            value.push_back(static_cast<std::string>(it.value()));
        }
    }
};
} // namespace OHOS::UpdateEngine
#endif // JSON_UTILS_H