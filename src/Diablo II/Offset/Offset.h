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

#ifndef OFFSET_H
#define OFFSET_H

#include <windows.h>
#include <unordered_map>

#include "../Version/Version.h"

enum class DllFiles : unsigned long long {
    BINKW32, BNCLIENT, D2CLIENT, D2CMP, D2COMMON, D2DDRAW, D2DIRECT3D, D2GAME,
    D2GDI, D2GFX, D2GLIDE, D2LANG, D2LAUNCH, D2MCPCLIENT, D2MULTI, D2NET,
    D2SOUND, D2WIN, FOG, GLIDE3X, IJL11, SMACKW32, STORM,
};

class Offset {
public:
    Offset(const DllFiles dllFile,
            const std::unordered_map<GameVersion, long long int>& offsets);
    Offset(Offset&& offset) = default;

    long long int getCurrentOffset() const;
    DWORD getCurrentAddress() const;
private:
    DllFiles dllFile;
    std::unordered_map<GameVersion, long long int> offsets;

    static HMODULE getDllBaseAddress(DllFiles dllFile);
};

#endif // OFFSET_H
