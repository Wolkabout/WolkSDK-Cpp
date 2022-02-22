/**
 * Copyright 2022 Wolkabout Technology s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FILESYSTEMUTILS_H
#define FILESYSTEMUTILS_H

#include "core/utilities/ByteUtils.h"

#include <string>
#include <vector>

namespace wolkabout
{
class FileSystemUtils
{
public:
    FileSystemUtils() = delete;

    static bool isFilePresent(const std::string& filePath);

    static bool createFileWithContent(const std::string& filePath, const std::string& content);

    static bool createBinaryFileWithContent(const std::string& filePath, const ByteArray& content);

    static bool deleteFile(const std::string& filePath);

    static time_t getLastModified(const std::string& path);

    static std::uint64_t getFileSize(const std::string& path);

    static bool isDirectoryPresent(const std::string& dirPath);

    static bool createDirectory(const std::string& dirPath);

    static bool readFileContent(const std::string& filePath, std::string& content);

    static bool readBinaryFileContent(const std::string& filePath, ByteArray& content);

    static std::vector<std::string> listFiles(const std::string& directoryPath);

    static std::string composePath(const std::string& fileName, const std::string& directory);

    static std::string absolutePath(const std::string& path);

private:
    static const char PATH_DELIMITER;
};
}    // namespace wolkabout

#endif
