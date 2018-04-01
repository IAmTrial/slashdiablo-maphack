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

#include "Common.h"
#include <iostream>
#include <regex>
#include <string>
#include <string_view>

#include <cerrno>
#include <cwchar>

std::string Common::unicodeToAnsi(std::wstring_view str) {
    char buffer[str.length() + 1];
    const wchar_t *src = str.data();

    std::mbstate_t state;
    if (std::wcsrtombs(buffer, &src, str.length() + 1, &state) ==
            static_cast<std::size_t>(-1)) {
        std::cerr << std::strerror(errno) << std::endl;
        return "";
    }

    return std::string(buffer);
}

std::wstring Common::ansiUnicode(std::string_view str) {
    wchar_t buffer[str.length() + 1];
    const char *src = str.data();

    std::mbstate_t state;
    if (std::mbsrtowcs(buffer, &src, str.length() + 1, &state) ==
            static_cast<std::size_t>(-1)) {
        std::cerr << std::strerror(errno) << std::endl;
        return L"";
    }

    return std::wstring(buffer);
}

std::string Common::strtrim(std::string_view source) {
    size_t startIndex = source.find_first_not_of(' ');
    size_t endIndex = source.find_last_not_of(' ') + 1;

    return std::string(source.substr(startIndex, endIndex - startIndex));
}

std::wstring Common::strtrim(std::wstring_view source) {
    size_t startIndex = source.find_first_not_of(' ');
    size_t endIndex = source.find_last_not_of(' ') + 1;

    return std::wstring(source.substr(startIndex, endIndex - startIndex));
}

bool Common::stobool(std::string_view str) {
    const std::regex TRUE_REGEX("(1)|(y)|(yes)|(true)",
        std::regex_constants::icase);

    return std::regex_match(str.data(), TRUE_REGEX);
}

bool Common::stobool(std::wstring_view str) {
    const std::wregex TRUE_REGEX(L"(1)|(y)|(yes)|(true)",
        std::regex_constants::icase);

    return std::regex_match(str.data(), TRUE_REGEX);
}
