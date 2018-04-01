/**
 * SlashDiablo Maphack
 * BH Maphack: Copyright (C) 2011 McGod
 * SlashDiablo Maphack: Copyright (C) 2012-2018 SlashDiablo Community
 *
 * This file is part of SlashDiablo Maphack.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Toggle {
    const unsigned int toggleKeyCodeValue;
    const bool defaultState;
};

class Config {
public:
    Config(std::string_view directoryAbsolutePath,
            std::string_view fileName);

    bool parse();

    // Functions to read values from the configuration
    bool readBool(std::string_view key, bool defaultValue) const;
    std::string readString(std::string_view key,
            std::string_view defaultValue) const;
    int readInt(std::string_view key, int defaultValue) const;
    unsigned int readKeyValue(std::string_view key,
            std::string_view defaultVirtualName) const;
    Toggle readToggle(std::string_view key,
            std::string_view defaultKeyCodeVirtualName,
            bool defaultState) const;
    std::map<int, std::string> readArray(std::string_view key) const;
    std::map<std::string, std::string> readAssoc(std::string_view key) const;
    std::vector<std::pair<std::string, std::string>> readMapList(
            std::string_view key) const;

    std::string getDirectoryAbsolutePath() const;
    std::string getFileName() const;
    std::string getFileAbsolutePath() const;

private:
    std::string directoryAbsolutePath;
    std::string fileName;
    std::string fileAbsolutePath;
    std::unordered_map<std::string,
        std::vector<std::pair<std::string, std::string>>> contents;
};

#endif // CONFIG_H
