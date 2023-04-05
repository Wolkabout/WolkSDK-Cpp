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

#include <any>
#include <sstream>

#define private public
#define protected public
#include "core/utilities/FileSystemUtils.h"
#undef private
#undef protected

#include "core/utilities/Logger.h"

#include <gtest/gtest.h>

#include <fstream>

using namespace wolkabout;
using namespace wolkabout::legacy;
using namespace ::testing;

class FileSystemUtilsTests : public ::testing::Test
{
public:
    static void SetUpTestCase() { Logger::init(LogLevel::TRACE, Logger::Type::CONSOLE); }

    void TearDown() override
    {
        std::remove((TEST_DIR + "/" + TEST_FILE).c_str());
        rmdir(TEST_DIR.c_str());
        std::remove(TEST_FILE.c_str());
    }

    bool createFile()
    {
        std::ofstream _;
        _.open(TEST_FILE);
        return _.is_open();
    }

    bool createDir() { return mkdir(TEST_DIR.c_str(), 0777) == 0; }

    bool createFileInDir()
    {
        std::ofstream _;
        _.open(TEST_DIR + "/" + TEST_FILE);
        return _.is_open();
    }

    bool writeToFile(const std::string& info)
    {
        std::ofstream _;
        _.open(TEST_FILE);
        if (_.is_open())
            _ << info;
        return _.is_open();
    }

    std::string readFile()
    {
        std::stringstream ss;
        std::ifstream _;
        _.open(TEST_FILE);
        if (_.is_open())
            for (auto line = std::string{}; getline(_, line);)
                ss << line;
        return ss.str();
    }

    bool changePermissions(std::uint16_t permissions) { return chmod(TEST_FILE.c_str(), permissions) == 0; }

    static std::string currentDirectory()
    {
        // Make place for the path
        char path[UINT16_MAX];
        path[UINT16_MAX - 1] = '\0';

        // Try to read the cwd
        if (auto dir = getcwd(path, UINT16_MAX - 1))
            return {dir};
        return {};
    }

    const std::string TEST_DIR = "test-dir";

    const std::string TEST_FILE = "test.file";
};

TEST_F(FileSystemUtilsTests, IsFilePresentNotPresent)
{
    EXPECT_FALSE(FileSystemUtils::isFilePresent("./totally-not-existing-file"));
}

TEST_F(FileSystemUtilsTests, IsFilePresentUnreadable)
{
    ASSERT_TRUE(createFile());
    ASSERT_TRUE(changePermissions(0222));
    EXPECT_FALSE(FileSystemUtils::isFilePresent(TEST_FILE));
}

TEST_F(FileSystemUtilsTests, DeleteFile)
{
    ASSERT_TRUE(createFile());
    ASSERT_TRUE(changePermissions(0222));
    EXPECT_TRUE(FileSystemUtils::deleteFile(TEST_FILE));
    EXPECT_FALSE(FileSystemUtils::deleteFile(TEST_FILE));
}

TEST_F(FileSystemUtilsTests, ComposePath)
{
    const auto directory = currentDirectory();
    EXPECT_EQ(FileSystemUtils::composePath(TEST_FILE, directory), directory + "/" + TEST_FILE);
}

TEST_F(FileSystemUtilsTests, ComposePathEmptyDirectory)
{
    EXPECT_EQ(FileSystemUtils::composePath(TEST_FILE, {}), TEST_FILE);
}

TEST_F(FileSystemUtilsTests, AbsolutePathOfFile)
{
    ASSERT_TRUE(createFile());
    EXPECT_EQ(FileSystemUtils::absolutePath(TEST_FILE), currentDirectory() + "/" + TEST_FILE);
}

TEST_F(FileSystemUtilsTests, AbsolutePathOfFileThatDoesntExist)
{
    EXPECT_EQ(FileSystemUtils::absolutePath(TEST_FILE), std::string{});
}

TEST_F(FileSystemUtilsTests, GetLastModified)
{
    ASSERT_TRUE(writeToFile("Hello!"));
    EXPECT_GT(FileSystemUtils::getLastModified(TEST_FILE), 0);
}

TEST_F(FileSystemUtilsTests, GetLastModifiedFileThatDoesntExist)
{
    EXPECT_EQ(FileSystemUtils::getLastModified(TEST_FILE), 0);
}

TEST_F(FileSystemUtilsTests, GetFileSize)
{
    ASSERT_TRUE(writeToFile("Hello!"));
    EXPECT_EQ(FileSystemUtils::getFileSize(TEST_FILE), 6);
}

TEST_F(FileSystemUtilsTests, GetFileSizeFileThatDoesntExist)
{
    EXPECT_EQ(FileSystemUtils::getFileSize(TEST_FILE), 0);
}

TEST_F(FileSystemUtilsTests, CreateFileWithContent)
{
    ASSERT_TRUE(FileSystemUtils::createFileWithContent(TEST_FILE, "Hello World!"));
    ASSERT_EQ(readFile(), "Hello World!");
}

TEST_F(FileSystemUtilsTests, CreateFileWithContentFileExistsAndIsUnreadable)
{
    ASSERT_TRUE(createFile());
    ASSERT_TRUE(changePermissions(0111));
    ASSERT_FALSE(FileSystemUtils::createFileWithContent(TEST_FILE, "Hello World!"));
}

TEST_F(FileSystemUtilsTests, CreateFileBinaryWithContent)
{
    ASSERT_TRUE(FileSystemUtils::createBinaryFileWithContent(TEST_FILE, ByteArray{65, 65, 65, 65, 65}));
    ASSERT_EQ(readFile(), "AAAAA");
}

TEST_F(FileSystemUtilsTests, CreateFileBinaryWithContentFileExistsAndIsUnreadable)
{
    ASSERT_TRUE(createFile());
    ASSERT_TRUE(changePermissions(0111));
    ASSERT_FALSE(FileSystemUtils::createBinaryFileWithContent(TEST_FILE, ByteArray{65, 65, 65, 65, 65}));
}

TEST_F(FileSystemUtilsTests, ReadFileContent)
{
    ASSERT_TRUE(createFile());
    ASSERT_TRUE(writeToFile("Hello World!"));

    // Make place for the read value
    auto value = std::string{};
    ASSERT_TRUE(FileSystemUtils::readFileContent(TEST_FILE, value));
    ASSERT_EQ(value, "Hello World!");
}

TEST_F(FileSystemUtilsTests, ReadFileContentFileDoesntExist)
{
    auto value = std::string{};
    ASSERT_FALSE(FileSystemUtils::readFileContent(TEST_FILE, value));
}

TEST_F(FileSystemUtilsTests, ReadFileBinary)
{
    ASSERT_TRUE(createFile());
    ASSERT_TRUE(writeToFile("AAAAA"));

    // Make place for the read value
    auto value = ByteArray{};
    ASSERT_TRUE(FileSystemUtils::readBinaryFileContent(TEST_FILE, value));
    ASSERT_EQ(value.size(), 5);
    for (const auto& byte : value)
        EXPECT_EQ(byte, 65);
}

TEST_F(FileSystemUtilsTests, ReadFileBinaryFileDoesntExist)
{
    auto value = ByteArray{};
    ASSERT_FALSE(FileSystemUtils::readBinaryFileContent(TEST_FILE, value));
}

TEST_F(FileSystemUtilsTests, IsDirectoryPresent)
{
    ASSERT_TRUE(createDir());
    ASSERT_TRUE(FileSystemUtils::isDirectoryPresent(TEST_DIR));
}

TEST_F(FileSystemUtilsTests, IsDirectoryPresentNoDirectory)
{
    ASSERT_FALSE(FileSystemUtils::isDirectoryPresent(TEST_DIR));
}

TEST_F(FileSystemUtilsTests, CreateDirectory)
{
    ASSERT_TRUE(FileSystemUtils::createDirectory(TEST_DIR));
    struct stat dir
    {
    };
    ASSERT_EQ(stat(TEST_DIR.c_str(), &dir), 0);
    ASSERT_NE(dir.st_mode & S_IFDIR, 0);
}

TEST_F(FileSystemUtilsTests, CreateDirectoryIfItAlreadyExists)
{
    ASSERT_TRUE(createDir());
    ASSERT_TRUE(FileSystemUtils::createDirectory(TEST_DIR));
}

TEST_F(FileSystemUtilsTests, ListFileInDirectory)
{
    ASSERT_TRUE(createDir());
    ASSERT_TRUE(createFileInDir());
    auto files = std::vector<std::string>{};
    ASSERT_NO_FATAL_FAILURE(files = FileSystemUtils::listFiles(TEST_DIR));
    ASSERT_FALSE(files.empty());
    ASSERT_EQ(files.front(), TEST_FILE);
}

TEST_F(FileSystemUtilsTests, ListFilesInDirectoryNoDirectory)
{
    ASSERT_TRUE(FileSystemUtils::listFiles(TEST_DIR).empty());
}
