/**
 * Copyright 2021 WolkAbout Technology s.r.o.
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

#include "FileSystemUtils.h"

#include "Logger.h"

#include <dirent.h>
#include <fstream>
#include <sys/stat.h>

namespace wolkabout
{
const char FileSystemUtils::PATH_DELIMITER = '/';

bool FileSystemUtils::isFilePresent(const std::string& filePath)
{
    FILE* configFile = ::fopen(filePath.c_str(), "r");
    if (!configFile)
    {
        return false;
    }

    fclose(configFile);
    return true;
}

bool FileSystemUtils::createFileWithContent(const std::string& filePath, const std::string& content)
{
    try
    {
        std::ofstream ofstream;
        ofstream.open(filePath);
        if (!ofstream.is_open())
        {
            return false;
        }

        ofstream << content;
        if (!ofstream)
        {
            deleteFile(filePath);
            return false;
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FileSystemUtils::createBinaryFileWithContent(const std::string& filePath, const ByteArray& content)
{
    try
    {
        std::ofstream ofstream;
        ofstream.open(filePath, std::ofstream::binary);
        if (!ofstream.is_open())
        {
            return false;
        }

        ofstream.write(reinterpret_cast<const char*>(content.data()), content.size());
        if (!ofstream)
        {
            deleteFile(filePath);
            return false;
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FileSystemUtils::deleteFile(const std::string& filePath)
{
    return std::remove(filePath.c_str()) == 0;
}

bool FileSystemUtils::isDirectoryPresent(const std::string& dirPath)
{
    struct stat status;
    if (stat(dirPath.c_str(), &status) == 0)
    {
        return (status.st_mode & S_IFDIR) != 0;
    }

    return false;
}

bool FileSystemUtils::createDirectory(const std::string& dirPath)
{
    if (isDirectoryPresent(dirPath))
    {
        return true;
    }

    return mkdir(dirPath.c_str(), 0700) == 0;
}

bool FileSystemUtils::readFileContent(const std::string& filePath, std::string& content)
{
    std::ifstream ifstream(filePath);
    if (!ifstream.is_open())
    {
        return false;
    }

    content = std::string((std::istreambuf_iterator<char>(ifstream)), std::istreambuf_iterator<char>());
    return true;
}

bool FileSystemUtils::readBinaryFileContent(const std::string& filePath, ByteArray& content)
{
    std::ifstream ifstream(filePath, std::ofstream::binary);
    if (!ifstream.is_open())
    {
        return false;
    }

    content =
      ByteUtils::toByteArray(std::string((std::istreambuf_iterator<char>(ifstream)), std::istreambuf_iterator<char>()));

    return true;
}

std::vector<std::string> FileSystemUtils::listFiles(const std::string& directoryPath)
{
    std::vector<std::string> result;

    DIR* dp = nullptr;
    struct dirent* dirp = nullptr;
    if ((dp = opendir(directoryPath.c_str())) == nullptr)
    {
        return result;
    }

    while ((dirp = readdir(dp)) != nullptr)
    {
        if (dirp->d_type == DT_REG)
        {
            result.emplace_back(dirp->d_name);
        }
    }

    closedir(dp);

    return result;
}

std::string FileSystemUtils::composePath(const std::string& fileName, const std::string& directory)
{
    if (directory.empty())
    {
        return fileName;
    }

    return directory + PATH_DELIMITER + fileName;
}

std::string FileSystemUtils::absolutePath(const std::string& path)
{
    char* resolvedPath = realpath(path.c_str(), nullptr);
    if (resolvedPath)
    {
        std::string fullPath(resolvedPath);
        free(resolvedPath);

        return fullPath;
    }

    return "";
}

std::time_t FileSystemUtils::getLastModified(const std::string& path)
{
    std::time_t lastModified = 0;
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0)
    {
        LOG(ERROR) << "File '" << path << "' does not exist!";
        return lastModified;
    }
    return fileInfo.st_mtime;
}

std::uint64_t FileSystemUtils::getFileSize(const std::string& path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0)
    {
        LOG(ERROR) << "File '" << path << "' does not exist!";
        return 0;
    }
    return static_cast<std::uint64_t>(fileInfo.st_size);
}
}    // namespace wolkabout
