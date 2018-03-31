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

#include "BH.h"
#include <windows.h>
#include <shlwapi.h>

BH::BH(HMODULE instance) : instance(instance) {
    // Locate the full path to the DLL.
    wchar_t path[MAX_PATH];

    if (!GetModuleFileNameW(this->instance, path, MAX_PATH)) {
        MessageBoxW(nullptr, L"GetModuleFileName failed.",
            L"SlashDiablo Maphack Error", MB_OK | MB_ICONERROR);
        std::exit(0);
    }

    // Remove the DLL file name from the path to get the directory.
    if (!PathRemoveFileSpecW(path)) {
        MessageBoxW(nullptr, L"PathCchRemoveFileSpec failed.",
            L"SlashDiablo Maphack Error", MB_OK | MB_ICONERROR);
        std::exit(0);
    }

    this->directoryPath.assign(path);
    this->directoryPath += L"\\";
}

BH::~BH() {
}

void BH::initialize() {
}
