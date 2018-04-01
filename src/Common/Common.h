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

#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <string_view>

namespace Common {
    std::string unicodeToAnsi(std::wstring_view str);
    std::wstring ansiUnicode(std::string_view str);

    std::string strtrim(std::string_view source);
    std::wstring strtrim(std::wstring_view source);

    bool stobool(std::string_view str);
    bool stobool(std::wstring_view str);
}

#endif // COMMON_H
