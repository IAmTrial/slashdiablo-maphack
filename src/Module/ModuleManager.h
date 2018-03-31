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

#ifndef MODULEMANAGER
#define MODULEMANAGER

#include <memory>
#include <string>
#include <unordered_map>
#include "Module.h"

class ModuleManager {
public:
    void add(std::shared_ptr<Module> pModule);
    void remove(std::wstring_view moduleName);

    void onLoop();

    void onGameJoin(std::string_view name, std::string_view pass, int diff);
    void onGameExit();

    void onDraw();
    void onAutomapDraw();
    void onOOGDraw();

    void onLeftClick(bool up, int x, int y, bool* block);
    void onRightClick(bool up, int x, int y, bool* block);
    void onKey(bool up, BYTE key, LPARAM lParam, bool* block);

    void onChatPacketRecv(BYTE* packet, bool* block);
    void onRealmPacketRecv(BYTE* packet, bool* block);
    void onGamePacketRecv(BYTE* packet, bool* block);

private:
    std::unordered_map<std::wstring, std::shared_ptr<Module>> modules;
};

#endif // MODULEMANAGER
