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

#include "Config.h"
#include <fstream>
#include <string>
#include <string_view>

#include "Common.h"

Config::Config(std::string_view directoryAbsolutePath,
        std::string_view fileName) :
            directoryAbsolutePath(directoryAbsolutePath.data()),
            fileName(fileName.data()),
            fileAbsolutePath(this->directoryAbsolutePath + this->fileName) {
}

bool Config::parse() {
    // Open the config file for reading.
    std::ifstream configFileStream(this->directoryAbsolutePath +
        this->fileName);
    if (!configFileStream.is_open()) {
        return false;
    }

    // Wipe all old values.
    this->contents.clear();

    // Read each line from the file.
    std::string line;
    while (std::getline(configFileStream, line)) {
        // Remove any comments from the config.
        if (line.find("//") != std::string::npos) {
            line.erase(line.find("//"));
        }

        // If there's nothing after comments are removed, then skip this line.
        if (line.length() == 0) {
            continue;
        }

        // Grab the key and value and store them.
        std::string key =
            Common::strtrim(line.substr(0, line.find_first_of(":")));
        std::string value =
            Common::strtrim(line.substr(line.find_first_of(":") + 1));

        contents.insert(std::make_pair(key, value));
    }

    return true;
}

std::string Config::getDirectoryAbsolutePath() const {
    return this->directoryAbsolutePath;
}

std::string Config::getFileName() const {
    return this->fileName;
}

std::string Config::getFileAbsolutePath() const {
    return this->fileAbsolutePath;
}
