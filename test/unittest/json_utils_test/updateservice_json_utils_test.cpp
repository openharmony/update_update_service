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

#include <gtest/gtest.h>

#include "updateservice_json_utils.h"

using namespace testing::ext;
using namespace testing;

namespace OHOS::UpdateService {
class UpdateServiceJsonUtilsTest : public testing::Test {
public:
    static void SetupTestCase(void) {};
    static void TearDownTestCase(void) {};
    void SetUp() final {};
    void TearDown() final {};
};

/**
* @tc.name: ParseAndGetJsonObjectTrue
* @tc.desc: Test json string is true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, ParseAndGetJsonObjectTrue, TestSize.Level1)
{
    std::string jsonStr = "{\"name\":\"Alice\", \"age\":25}";
    cJSON *root = UpdateServiceJsonUtils::ParseAndGetJsonObject(jsonStr);
    EXPECT_NE(nullptr, root);
    cJSON_Delete(root);
}

/**
* @tc.name: ParseAndGetJsonObjectFalse
* @tc.desc: Test json string is false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, ParseAndGetJsonObjectFalse, TestSize.Level1)
{
    std::string jsonStr = "{\"name\":\"Alice\", \"age\":25, abc}";
    cJSON *root = UpdateServiceJsonUtils::ParseAndGetJsonObject(jsonStr);
    EXPECT_EQ(nullptr, root);
}

/**
* @tc.name: ParseJsonTrue
* @tc.desc: Test json string is true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, ParseJsonTrue, TestSize.Level1)
{
    std::string jsonStr = "{\"name\":\"Alice\", \"age\":25}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    EXPECT_NE(nullptr, root);
}

/**
* @tc.name: ParseJsonFalse
* @tc.desc: Test json string is true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, ParseJsonFalse, TestSize.Level1)
{
    std::string jsonStr = "{\"name\":\"Alice\", \"age\":25, xxx}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    EXPECT_EQ(nullptr, root);
}

/**
* @tc.name: GetValueAndSetToNullptr
* @tc.desc: Test json string, root is nullptr
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToNullptr, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"file\": \"a.txt\", xxx}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    int32_t ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::MISSING_PROP), ret);
}

/**
* @tc.name: GetValueAndSetToKeyNullptr
* @tc.desc: Test json string, key is nullptr
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToKeyNullptr, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    int32_t ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::MISSING_PROP), ret);
}

/**
* @tc.name: GetValueAndSetToKeyNotFound
* @tc.desc: Test json string, key not found
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToKeyNotFound, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    int32_t ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "book", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::MISSING_PROP), ret);
}

/**
* @tc.name: GetValueAndSetToStringTypeErr
* @tc.desc: Test json string, get string type error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToStringTypeErr, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"file\": 1234}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    std::string  fileName;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "file", fileName);
    EXPECT_EQ(CAST_INT(JsonParseError::TYPE_ERROR), ret);
}

/**
* @tc.name: GetValueAndSetToStringOk
* @tc.desc: Test json string, get string ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToStringOk, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    std::string  fileName;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "file", fileName);
    EXPECT_EQ(CAST_INT(JsonParseError::ERR_OK), ret);
    EXPECT_EQ("a.txt", fileName);
}

/**
* @tc.name: GetValueAndSetToBoolTypeErr
* @tc.desc: Test json string, get Bool type error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToBoolTypeErr, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"boolFlag\": 123}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    bool boolFlag;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "boolFlag", boolFlag);
    EXPECT_EQ(CAST_INT(JsonParseError::TYPE_ERROR), ret);
}

/**
* @tc.name: GetValueAndSetToBoolTypeOk
* @tc.desc: Test json string, get Bool type ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToBoolTypeOk, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"boolFlag\": false}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    bool boolFlag;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "boolFlag", boolFlag);
    EXPECT_EQ(CAST_INT(JsonParseError::ERR_OK), ret);
    EXPECT_EQ(false, boolFlag);
}

/**
* @tc.name: GetValueAndSetToInt32TypeError
* @tc.desc: Test json string, int32_t type error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToInt32TypeError, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54.22, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    int32_t  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::TYPE_ERROR), ret);
}

/**
* @tc.name: GetValueAndSetToInt32Ok
* @tc.desc: Test json string, get int32_t ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToInt32Ok, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    int32_t  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::ERR_OK), ret);
    EXPECT_EQ(54, ageNumber);
}

/**
* @tc.name: GetValueAndSetToInt64TypeError
* @tc.desc: Test json string, int64_t type error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToInt64TypeError, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 541212121.44, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    int64_t  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::TYPE_ERROR), ret);
}

/**
* @tc.name: GetValueAndSetToInt64Ok
* @tc.desc: Test json string, get int64_t ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToInt64Ok, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54121212123, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    int64_t  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::ERR_OK), ret);
    EXPECT_EQ(54121212123, ageNumber);
}

/**
* @tc.name: GetValueAndSetToUInt32TypeError
* @tc.desc: Test json string, uint32_t type error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToUInt32TypeError, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": -111, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    uint32_t  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::TYPE_ERROR), ret);
}

/**
* @tc.name: GetValueAndSetToUInt32Ok
* @tc.desc: Test json string, get uint32_t ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToUInt32Ok, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 12, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    uint32_t  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::ERR_OK), ret);
    EXPECT_EQ(12, ageNumber);
}

/**
* @tc.name: GetValueAndSetToUInt64TypeError
* @tc.desc: Test json string, uint64_t type error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToUInt64TypeError, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": -100, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    uint64_t  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::TYPE_ERROR), ret);
}

/**
* @tc.name: GetValueAndSetToUInt64Ok
* @tc.desc: Test json string, get uint64_t ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToUInt64Ok, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 1212121211255, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    uint64_t  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::ERR_OK), ret);
    EXPECT_EQ(1212121211255, ageNumber);
}

/**
* @tc.name: GetValueAndSetToDoubleTypeError
* @tc.desc: Test json string, Double type error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToDoubleTypeError, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": \"NAN\", \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    double  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::TYPE_ERROR), ret);
}

/**
* @tc.name: GetValueAndSetToDoubleOk
* @tc.desc: Test json string, get double ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, GetValueAndSetToDoubleOk, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 1212121211255.99, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    double  ageNumber;
    int ret = UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "age", ageNumber);
    EXPECT_EQ(CAST_INT(JsonParseError::ERR_OK), ret);
    EXPECT_EQ(1212121211255.99, ageNumber);
}

/**
* @tc.name: IsIntegerTrue
* @tc.desc: check isInteger true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, IsIntegerTrue, TestSize.Level1)
{
    double d = 100;
    bool bl = UpdateServiceJsonUtils::IsInteger(d);
    EXPECT_EQ(true, bl);
}

/**
* @tc.name: IsIntegerFalse
* @tc.desc: check isInteger false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, IsIntegerFalse, TestSize.Level1)
{
    double d = 100.123;
    bool bl = UpdateServiceJsonUtils::IsInteger(d);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckIntegerInt32True
* @tc.desc: test json string, check int32 true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckIntegerInt32True, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    int32_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckInteger(item, ageNumber);
    EXPECT_EQ(true, bl);
}

/**
* @tc.name: CheckIntegerInt32False
* @tc.desc: test json string, check int32 false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckIntegerInt32False, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123.11, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    int32_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckInteger(item, ageNumber);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckIntegerInt64True
* @tc.desc: test json string, check int64 true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckIntegerInt64True, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123454545, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    int64_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckInteger(item, ageNumber);
    EXPECT_EQ(true, bl);
}

/**
* @tc.name: CheckIntegerInt64False
* @tc.desc: test json string, check int64 false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckIntegerInt64False, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123.11111, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    int64_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckInteger(item, ageNumber);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckIntegerUInt32True
* @tc.desc: test json string, check uint32 true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckIntegerUInt32True, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    uint32_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckInteger(item, ageNumber);
    EXPECT_EQ(true, bl);
}

/**
* @tc.name: CheckIntegerUInt32False
* @tc.desc: test json string, check uint32 false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckIntegerUInt32False, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": -111, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    uint32_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckInteger(item, ageNumber);
    EXPECT_EQ(false, bl);
}


/**
* @tc.name: CheckIntegerUInt64True
* @tc.desc: test json string, check uint64 true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckIntegerUInt64True, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    uint64_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckInteger(item, ageNumber);
    EXPECT_EQ(true, bl);
}

/**
* @tc.name: CheckIntegerUInt64False
* @tc.desc: test json string, check uint64 false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckIntegerUInt64False, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": -111, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    uint64_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckInteger(item, ageNumber);
    EXPECT_EQ(false, bl);
}

//CheckTypeAndAsign
/**
* @tc.name: CheckTypeAndAsignStringError
* @tc.desc: test json string, check String is error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignStringError, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": -111, \"file\": 1234}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "file");
    ASSERT_NE(nullptr, item);
    std::string fileName;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, fileName);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckTypeAndAsignStringOk
* @tc.desc: test json string, check String is ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignStringOk, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": -111, \"file\": \"1234.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "file");
    ASSERT_NE(nullptr, item);
    std::string fileName;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, fileName);
    EXPECT_EQ(true, bl);
    EXPECT_EQ("1234.txt", fileName);
}

/**
* @tc.name: CheckTypeAndAsignBoolError
* @tc.desc: test json string, check bool is error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignBoolError, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"boolFlag\": 123}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "boolFlag");
    ASSERT_NE(nullptr, item);
    bool boolFlag;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, boolFlag);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckTypeAndAsignBoolOk
* @tc.desc: test json string, check bool is ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignBoolOk, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"boolFlag\": true}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "boolFlag");
    ASSERT_NE(nullptr, item);
    bool boolFlag = false;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, boolFlag);
    EXPECT_EQ(true, bl);
    EXPECT_EQ(true, bl);
}

/**
* @tc.name: CheckTypeAndAsignInt32True
* @tc.desc: test json string, check int32 true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignInt32True, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    int32_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(true, bl);
    EXPECT_EQ(123, ageNumber);
}

/**
* @tc.name: CheckTypeAndAsignInt32False
* @tc.desc: test json string, check int32 false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignInt32False, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123.11, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    int32_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckTypeAndAsignInt64True
* @tc.desc: test json string, check int64 true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignInt64True, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123454545, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    int64_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(true, bl);
    EXPECT_EQ(123454545, ageNumber);
}

/**
* @tc.name: CheckTypeAndAsignInt64False
* @tc.desc: test json string, check int64 false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignInt64False, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123.11111, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    int64_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckTypeAndAsignUInt32True
* @tc.desc: test json string, check uint32 true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignUInt32True, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 123, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    uint32_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(true, bl);
    EXPECT_EQ(123, ageNumber);
}

/**
* @tc.name: CheckTypeAndAsignUInt32False
* @tc.desc: test json string, check uint32 false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignUInt32False, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": -111, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    uint32_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckTypeAndAsignUInt64True
* @tc.desc: test json string, check uint64 true
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignUInt64True, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 12312121, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    uint64_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(true, bl);
    EXPECT_EQ(12312121, ageNumber);
}

/**
* @tc.name: CheckTypeAndAsignUInt64False
* @tc.desc: test json string, check uint64 false
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignUInt64False, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": -111, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    uint64_t  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckTypeAndAsignDoubleFalse
* @tc.desc: Test json string, Double type error
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignDoubleFalse, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": \"NAN\", \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    double  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(false, bl);
}

/**
* @tc.name: CheckTypeAndAsignDoubleTrue
* @tc.desc: Test json string, get double ok
* @tc.type: FUNC
*/
HWTEST_F(UpdateServiceJsonUtilsTest, CheckTypeAndAsignDoubleTrue, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 1212121211255.99, \"file\": \"a.txt\"}";
    auto root = UpdateServiceJsonUtils::ParseJson(jsonStr);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root.get(), "age");
    ASSERT_NE(nullptr, item);
    double  ageNumber;
    bool bl = UpdateServiceJsonUtils::CheckTypeAndAsign(item, ageNumber);
    EXPECT_EQ(true, bl);
    EXPECT_EQ(1212121211255.99, ageNumber);
}
} // namespace OHOS::UpdateService