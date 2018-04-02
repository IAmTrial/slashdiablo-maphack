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

#include "Offset.h"
#include <windows.h>
#include <unordered_map>
#include <unordered_set>

#include "../Version/Version.h"

Offset::Offset(const DllFiles dllFile,
        const std::unordered_map<GameVersion, long long int>& offsets) :
    dllFile(dllFile), offsets(offsets) {
}

long long int Offset::getCurrentOffset() const {
    auto offset = offsets.find(Version::getGameVersion());
    return (offset != offsets.cend()) ? offset->second : 0;
}

DWORD Offset::getCurrentAddress() const {
    HMODULE baseAddress = getDllBaseAddress(dllFile);

    if (baseAddress == nullptr) {
        return 0;
    }

    long long int offset = getCurrentOffset();

    DWORD address;

    if (offset < 0) {
        address = (DWORD) GetProcAddress((HINSTANCE) baseAddress,
                                         (LPSTR) - offset);
    } else {
        address = (DWORD) baseAddress + (DWORD)offset;
    }

    return address;
}

HMODULE Offset::getDllBaseAddress(DllFiles dllFile) {
    static std::unordered_map<DllFiles, HMODULE> dllHandles;
    static const std::unordered_map<DllFiles, std::wstring_view>
            dllFilePaths = {
        { DllFiles::BINKW32, L"Binkw32.dll" },
        { DllFiles::BNCLIENT, L"BnClient.dll" },
        { DllFiles::D2CLIENT, L"D2Client.dll" },
        { DllFiles::D2CMP, L"D2CMP.dll" },
        { DllFiles::D2COMMON, L"D2Common.dll" },
        { DllFiles::D2DDRAW, L"D2DDraw.dll" },
        { DllFiles::D2DIRECT3D, L"D2Direct3D.dll" },
        { DllFiles::D2GAME, L"D2Game.dll" },
        { DllFiles::D2GDI, L"D2Gdi.dll" },
        { DllFiles::D2GFX, L"D2Gfx.dll" },
        { DllFiles::D2GLIDE, L"D2Glide.dll" },
        { DllFiles::D2LANG, L"D2Lang.dll" },
        { DllFiles::D2LAUNCH, L"D2Launch.dll" },
        { DllFiles::D2MCPCLIENT, L"D2MCPClient.dll" },
        { DllFiles::D2MULTI, L"D2Multi.dll" },
        { DllFiles::D2NET, L"D2Net.dll" },
        { DllFiles::D2SOUND, L"D2Sound.dll" },
        { DllFiles::D2WIN, L"D2Win.dll" },
        { DllFiles::FOG, L"Fog.dll" },
        { DllFiles::GLIDE3X, L"glide3x.dll" },
        { DllFiles::IJL11, L"Ijl11.dll" },
        { DllFiles::SMACKW32, L"SmackW32.dll" },
        { DllFiles::STORM, L"Storm.dll" }
    };

    static const std::unordered_set<DllFiles> redirectedDlls = {
        DllFiles::BNCLIENT, DllFiles::D2CLIENT, DllFiles::D2CMP,
        DllFiles::D2COMMON, DllFiles::D2DDRAW, DllFiles::D2DIRECT3D,
        DllFiles::D2GAME, DllFiles::D2GDI, DllFiles::D2GFX, DllFiles::D2GLIDE,
        DllFiles::D2LANG, DllFiles::D2LAUNCH, DllFiles::D2MCPCLIENT,
        DllFiles::D2MULTI, DllFiles::D2NET, DllFiles::D2SOUND, DllFiles::D2WIN,
        DllFiles::FOG, DllFiles::STORM
    };

    HMODULE dllAddress = dllHandles[dllFile];

    if (dllAddress == nullptr) {
        std::wstring_view moduleName = dllFilePaths.at(dllFile);

        if (Version::isGameVersion114Plus()) {
            if (redirectedDlls.count(dllFile) == 1) {
                moduleName = L"Game.exe";
            }
        }

        dllAddress = GetModuleHandleW(moduleName.data());

        if (dllAddress == nullptr) {
            dllAddress = LoadLibraryW(moduleName.data());
        }

        dllHandles.at(dllFile) = dllAddress;
    }

    return dllAddress;
}
