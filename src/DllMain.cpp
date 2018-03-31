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

#include <windows.h>
#include "BH.h"

BOOL WINAPI DllMain(HMODULE instance, DWORD reason, VOID* reserved) {
    static BH *slashDiabloHack = nullptr;

    switch(reason) {
    case DLL_PROCESS_ATTACH:
        slashDiabloHack = new BH(instance);

    case DLL_PROCESS_DETACH:
        delete slashDiabloHack;

    default:
        return FALSE;
    }

    return TRUE;
}
