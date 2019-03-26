/*
 * Copyright 2019 WolkAbout Technology s.r.o.
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

#ifndef FILELIST_H
#define FILELIST_H

#include <string>
#include <vector>

namespace wolkabout
{
class FileList
{
public:
    FileList(std::vector<std::string> files);

    const std::vector<std::string>& getFileNames() const;

private:
    std::vector<std::string> m_fileNames;
};
}    // namespace wolkabout

#endif    // FILELIST_H
