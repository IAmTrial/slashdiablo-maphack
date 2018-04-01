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
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "Common.h"

Config::Config(std::string_view directoryAbsolutePath,
        std::string_view fileName) :
            directoryAbsolutePath(directoryAbsolutePath.data()),
            fileName(fileName.data()),
            fileAbsolutePath(this->directoryAbsolutePath + this->fileName) {
}

/**
 * Parse the configuration file and stores the results in a key->value pair.
 * Can be called multiple times so you can reload the configuration.
 */
bool Config::parse() {
    const std::regex KEY_VALUE_REGEX("(.+?):(.+)");
    const std::regex ASSOC_REGEX("(.+?)(\\[([^\\]]+?)\\])?");

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

        // Grab the full key and value.
        std::smatch keyValueMatches;
        if (!std::regex_match(line, keyValueMatches, KEY_VALUE_REGEX)) {
            continue;
        }

        std::string rawKey = Common::strtrim(keyValueMatches.str(1));
        std::string value = Common::strtrim(keyValueMatches.str(2));

        // Grab the key name and the key index.
        std::smatch assocMatches;
        if (!std::regex_match(rawKey, keyValueMatches, ASSOC_REGEX)) {
            continue;
        }

        std::string keyName = assocMatches.str(1);
        std::string keyIndex = assocMatches.str(3);

        auto& values = contents[keyName];
        values.push_back(std::make_pair(keyIndex, value));
    }

    return true;
}

/**
 * Reads in a boolean from the key-pair.
 */
bool Config::readBool(std::string_view key, bool defaultValue) const {
    // Check if configuration value exists.
    const auto mappedValue = this->contents.find(key.data());

    return (mappedValue != this->contents.cend())
        ? Common::stobool(mappedValue->second.at(0).second)
        : defaultValue;
}

/**
 * Reads in a string from the key-pair.
 */
std::string Config::readString(std::string_view key,
        std::string_view defaultValue) const {
    // Check if configuration value exists.
    const auto mappedValue = this->contents.find(key.data());

    return (mappedValue != this->contents.cend())
        ? mappedValue->second.at(0).second
        : defaultValue.data();
}

/**
 * Reads in a decimal or hex(which is converted to decimal) from the key-pair.
 */
int Config::readInt(std::string_view key, int defaultValue) const {
    // Check if configuration value exists.
    const auto& mappedValue = this->contents.find(key.data());

    if (mappedValue == this->contents.cend()) {
        return defaultValue;
    }

    return (mappedValue->second.at(0).second.find("0x") == 0)
        ? stoi(mappedValue->second.at(0).second, 0, 16)
        : stoi(mappedValue->second.at(0).second);
}

/**
 * Reads in a key from the key->pair.
 */
unsigned int Config::readKeyValue(std::string_view key,
        std::string_view defaultVirtualName) const {
    // Check if configuration value exists.
    const auto& mappedValue = this->contents.find(key.data());

    if (mappedValue == this->contents.cend()) {
        return Common::getVirtualMappedKeyCode(
            defaultVirtualName.data()).value;
    }

    // Grab the proper key code and make sure it's valid
    const KeyCode keyCode =
        Common::getVirtualMappedKeyCode(mappedValue->second.at(0).second);

    if (keyCode.value == 0) {
        return Common::getVirtualMappedKeyCode(
            defaultVirtualName).value;
    }

    return keyCode.value;
}

/**
 * Reads in a toggle from the key->pair.
 * Config Example:
 *     Key: True, VK_A
 */
Toggle Config::readToggle(std::string_view key,
        std::string_view defaultKeyCodeVirtualName,
        bool defaultState) const {
    const std::regex TOGGLE_REGEX("([^,]+?),(.+)");

    //Check if configuration value exists and if the regex matched.
    const auto& mappedValue = this->contents.find(key.data());
    std::smatch matches;

    if (mappedValue == this->contents.cend() ||
            !std::regex_match(mappedValue->second.at(0).second, matches,
                TOGGLE_REGEX)) {
        unsigned int defaultKeyCodeValue =
            Common::getVirtualMappedKeyCode(defaultKeyCodeVirtualName).value;
        Toggle defaultToggle = {
            defaultKeyCodeValue,
            defaultState
        };

        return defaultToggle;
    }

    //Pull the keycode from after the comma.
    std::string keyCodeLiteralName = Common::strtrim(matches.str(2));
    unsigned int keyCodeValue =
        Common::getLiteralMappedKeyCode(keyCodeLiteralName).value;

    //Pull the state from before the comma
    std::string stateString =
        Common::strtrim(matches.str(1));
    bool state = Common::stobool(stateString);

    Toggle toggle {
        keyCodeValue,
        state
    };

    return toggle;
}

/**
 * Reads the contents into a sorted index-based array containing all values of
 * the specified key.
 */
std::map<int, std::string> Config::readArray(std::string_view key) const {
    std::map<int, std::string> values;

    for (const auto& assoc : this->readAssoc(key)) {
        int key = stoi(assoc.first);
        values.insert(std::make_pair(key, assoc.second));
    }

    return values;
}

/**
 * Reads in a map from the key->pair. Order is arbitrary.
 * Config Example:
 *     Value[Test]: 0
 *     Value[Pickles]: 1
 */
std::map<std::string, std::string> Config::readAssoc(
        std::string_view key) const {
    // Read all entries associated with the key and store mappings.
    std::map<std::string, std::string> values;

    const auto mappedKeyValue = this->contents.find(key.data());
    if (mappedKeyValue != this->contents.cend()) {
        return values;
    }

    // Copy values.
    for (const auto& assoc : mappedKeyValue->second) {
        values.insert(assoc);
    }

    return values;
}

/**
 * Reads in a map from the key->pair, preserving original order
 * Config Example:
 *     Value[Test]: 0
 *     Value[Pickles]: 1
 */
std::vector<std::pair<std::string, std::string>> Config::readMapList(
        std::string_view key) const {
    auto mappedValues = this->contents.find(key.data());

    return (mappedValues != this->contents.cend())
        ? mappedValues->second
        : std::vector<std::pair<std::string, std::string>>();
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
