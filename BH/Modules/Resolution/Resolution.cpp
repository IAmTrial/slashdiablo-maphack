#include "Resolution.h"
#include "../../D2/D2Ptrs.h"
#include "../../BH.h"
#include "../../D2/D2Stubs.h"
#include "../../D2/D2Helpers.h"
#include "ScreenRefresh.h"

Patch* panelPositionPatch = new Patch(Call, D2CLIENT, 0xC39F6, (int)PanelPosition_Interception, 39);
Patch* enablePanelBordersPatch = new Patch(Call, D2CLIENT, 0x29262, (int)Resolution::EnableUIPanelBorders_Interception, 5);
Patch* redrawLeftPanelBorderPatch = new Patch(Call, D2CLIENT, 0x271ED, (int)RedrawUILeftPanelBorders_Interception, 154);
Patch* redrawRightPanelBorderPatch = new Patch(Call, D2CLIENT, 0x270F2, (int)RedrawUIRightPanelBorders_Interception, 187);

void Resolution::OnLoad() {
	isInGame = false;
	
	newWidth = BH::config->ReadInt("New Width", 1344);
	newHeight = BH::config->ReadInt("New Height", 700);
	Toggles["Toggle Resolution"] = BH::config->ReadToggle("Toggle Resolution", "VK_6", false);

	panelPositionPatch->Install();
	enablePanelBordersPatch->Install();
	redrawLeftPanelBorderPatch->Install();
	redrawRightPanelBorderPatch->Install();
}

void Resolution::LoadConfig() {
	
}

void Resolution::OnUnload() {
	panelPositionPatch->Remove();
	enablePanelBordersPatch->Remove();
	redrawLeftPanelBorderPatch->Remove();
	redrawRightPanelBorderPatch->Remove();
}

int Resolution::GetMode(int height) {
	switch(height)
	{
		case 800:
			return 2;
		case 640:
			return 1;
	}
	return 3;
}

//Shaggi's resolution fix, all credits to him
void Resolution::SetResolution(int x, int y) {
	//GetMode(*p_D2CLIENT_ScreenSizeY);
	int mode = GetMode(y);
	*p_D2CLIENT_CropMode = 0;
	D2WIN_SetResolutionMode(mode);
	*p_D2CLIENT_ScreenSizeX = *p_D2CLIENT_MapPosX = *p_D2CLIENT_SizeX1 = *p_D2CLIENT_SizeX2 = x;
	*p_D2CLIENT_ScreenSizeY = *p_D2CLIENT_SizeY1 = y;

	*p_D2CLIENT_MapPosY = y - 40; // subtract 40 to correct offsets
	*p_D2CLIENT_InventoryArrangeMode = 0;
	ScreenRefresh::RefreshDisplay();
	//raise resolution changed event so that other modules can readjust positions
	__raise BH::moduleManager->OnResolutionChanged(x, y);
}

void Resolution::OnDraw() {
	// Credits to /u/TravHatesMe aka blurt for sharing this code.
	// Fixes the Hall of Mirrors effect on the bottom UI.
	if (isInGame && (Toggles["Toggle Resolution"].state)) {
		const int UIBoxHalfWidth = 155;
		unsigned int box1Width = (newWidth / 2) - (UIBoxHalfWidth + 165);

		Drawing::Boxhook::Draw(165, 653, box1Width, 50, 0, Drawing::BoxTrans::BTOneFourth);
		Drawing::Boxhook::Draw(827, 653, box1Width, 50, 0, Drawing::BoxTrans::BTOneFourth);
	}
}

void Resolution::OnKey(bool up, BYTE key, LPARAM lParam, bool* block) {
	if (!isInGame)
		return;
	if (key == (Toggles["Toggle Resolution"].toggle)) {
		*block = true;
		if (up) {
			bool isLoaded = Toggles["Toggle Resolution"].state;
			if (!isLoaded) {
				this->SetResolution(newWidth, newHeight);
				Toggles["Toggle Resolution"].state = true;
			}
		}
	}
}

void Resolution::OnGameJoin(const string& name, const string& pass, int diff) {
	isInGame = true;

	//if the user has already toggled it from a previous game, autoload it
	if ((Toggles["Toggle Resolution"].state)
		&& newWidth > 0 && newHeight > 0) {
		this->SetResolution(newWidth, newHeight);
		ScreenRefresh::RefreshDisplay();
	}
}

void Resolution::OnGameExit() {
	isInGame = false;
	//raise resolution changed event so that other modules can readjust positions
	if (Toggles["Toggle Resolution"].state) {
		__raise BH::moduleManager->OnResolutionChanged(800, 600);
	}
}

void PanelPosition_Interception() {
	*p_D2CLIENT_PanelXOffset = (*p_D2CLIENT_ScreenSizeX / 2) - 320;
	*p_D2CLIENT_PanelYOffset = ((int)*p_D2CLIENT_ScreenSizeY - 480) / -2;
}

void RedrawUILeftPanelBorders_Interception() {
	__asm {
		mov dword ptr ds : [ebp + 0x30], eax
	}
	int frameNumber = 0;
	int basePositionX = (*p_D2CLIENT_ScreenSizeX / 2) - 400;
	int basePositionY = (*p_D2CLIENT_ScreenSizeY / 2) - 300;

	// Frame 0
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX, basePositionY + 253, 255, 5, 0);
	frameNumber++;

	// Frame 1
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX + 256, basePositionY + 63, 255, 5, 0);
	frameNumber++;

	// Frame 2
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX, basePositionY + 484, 255, 5, 0);
	frameNumber++;

	// Frame 3
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX, basePositionY + 553, 255, 5, 0);
	frameNumber++;

	// Frame 4
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX + 256, basePositionY + 553, 255, 5, 0);
	frameNumber++;
}

void RedrawUIRightPanelBorders_Interception() {
	__asm {
		mov dword ptr ds : [ebp + 0x30], edx
	}
	int frameNumber = 5;
	int basePositionX = (*p_D2CLIENT_ScreenSizeX / 2);
	int basePositionY = (*p_D2CLIENT_ScreenSizeY / 2) - 300;

	// Frame 5
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX, basePositionY + 63, 255, 5, 0);
	frameNumber++;

	// Frame 6
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX + 144, basePositionY + 253, 255, 5, 0);
	frameNumber++;

	// Frame 7
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX + 313, basePositionY + 484, 255, 5, 0);
	frameNumber++;

	// Frame 8
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX + 144, basePositionY + 553, 255, 5, 0);
	frameNumber++;

	// Frame 9
	D2GFX_DrawUIPanelBorder(&frameNumber, basePositionX, basePositionY + 553, 255, 5, 0);
	frameNumber++;
}

// This enables the expansion panel borders when you open
// panels (char menu, skill menu...)
int __fastcall Resolution::EnableUIPanelBorders_Interception() {
	int compareBypass = (*p_D2CLIENT_ScreenSizeX >= 800) ? 2 : 0;
	return compareBypass;
}

