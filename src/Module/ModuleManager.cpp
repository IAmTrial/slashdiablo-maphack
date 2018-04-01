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

#include "ModuleManager.h"
#include <memory>

ModuleManager::ModuleManager() {
}

ModuleManager::~ModuleManager() {
}

void ModuleManager::add(std::shared_ptr<Module> pModule) {
    this->modules.insert(std::make_pair(pModule->getName(), pModule));
}

void ModuleManager::remove(std::wstring_view moduleName) {
    this->modules.erase(moduleName.data());
}

void ModuleManager::loadModules() {
    for (const auto& module : this->modules) {
        module.second->onLoad();
    }
}

void ModuleManager::unloadModules() {
    for (const auto& module : this->modules) {
        module.second->onUnload();
    }
}

void ModuleManager::reloadConfig() {
    for (const auto& module : this->modules) {
        module.second->loadConfig();
    }
}

void ModuleManager::onMpqLoaded() {
    for (const auto& module : this->modules) {
        module.second->onUnload();
    }
}

void ModuleManager::onLoop() {
    for (const auto& module : this->modules) {
        module.second->onLoop();
    }
}

void ModuleManager::onGameJoin(std::string_view name, std::string_view pass, int diff) {
    for (const auto& module : this->modules) {
        module.second->onGameJoin(name, pass, diff);
    }
}

void ModuleManager::onGameExit() {
    for (const auto& module : this->modules) {
        module.second->onGameExit();
    }
}

void ModuleManager::onDraw() {
    for (const auto& module : this->modules) {
        module.second->onDraw();
    }
}

void ModuleManager::onAutomapDraw() {
    for (const auto& module : this->modules) {
        module.second->onAutomapDraw();
    }
}

void ModuleManager::onOOGDraw() {
    for (const auto& module : this->modules) {
        module.second->onOOGDraw();
    }
}

void ModuleManager::onLeftClick(bool up, int x, int y, bool* block) {
    for (const auto& module : this->modules) {
        module.second->onLeftClick(up, x, y, block);
    }
}

void ModuleManager::onRightClick(bool up, int x, int y, bool* block) {
    for (const auto& module : this->modules) {
        module.second->onRightClick(up, x, y, block);
    }
}

void ModuleManager::onKey(bool up, BYTE key, LPARAM lParam, bool* block) {
    for (const auto& module : this->modules) {
        module.second->onKey(up, key, lParam, block);
    }
}

void ModuleManager::onChatPacketRecv(BYTE* packet, bool* block) {
    for (const auto& module : this->modules) {
        module.second->onChatPacketRecv(packet, block);
    }
}

void ModuleManager::onRealmPacketRecv(BYTE* packet, bool* block) {
    for (const auto& module : this->modules) {
        module.second->onRealmPacketRecv(packet, block);
    }
}

void ModuleManager::onGamePacketRecv(BYTE* packet, bool* block) {
    for (const auto& module : this->modules) {
        module.second->onGamePacketRecv(packet, block);
    }
}
