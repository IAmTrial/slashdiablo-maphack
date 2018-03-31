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

#ifndef MODULE_H
#define MODULE_H

#include <windows.h>
#include <string>
#include <string_view>

class Module {
public:
    // Module Events
    virtual void loadConfig() {};
    virtual void onMpqLoaded() {};

    virtual void OnLoop() {};

    // Game Events
    virtual void onGameJoin(const std::string& name, const std::string& pass,
            int diff) {}
    virtual void onGameExit() {}

    // Drawing Events
    virtual void onDraw() {}
    virtual void onAutomapDraw() {}
    virtual void onOOGDraw() {}

    virtual void onLeftClick(bool up, int x, int y, bool* block) {}
    virtual void onRihtClick(bool up, int x, int y, bool* block) {}
    virtual void onKey(bool up, BYTE key, LPARAM lParam, bool* block) {}

    virtual void onChatPacketRecv(BYTE* packet, bool* block) {}
    virtual void onRealmPacketRecv(BYTE* packet, bool* block) {}
    virtual void onGamePacketRecv(BYTE* packet, bool* block) {}

    virtual void onUserInput(const wchar_t* msg, bool fromGame, bool* block) {}
    virtual void onChatMsg(const char* user, const char* msg, bool fromGame, bool* block) {}

    std::wstring getName() const;
    bool isActive() const;

protected:
    Module(std::wstring_view name);
    Module(Module&& module) = default;

private:
    std::wstring name;
    bool active;
};

#endif // MODULE_H
