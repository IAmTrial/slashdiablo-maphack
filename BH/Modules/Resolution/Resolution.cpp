#include "Resolution.h"
#include "../../D2/D2Ptrs.h"
#include "../../BH.h"
#include "../../D2/D2Stubs.h"
#include "../../D2/D2Helpers.h"
#include "ScreenRefresh.h"

Patch* panelPositionPatch = new Patch(Call, D2CLIENT, 0xC3A11, (int)PanelPosition_Interception, 12);

void Resolution::OnLoad() {
	isInGame = false;
	
	newWidth = BH::config->ReadInt("New Width", 1344);
	newHeight = BH::config->ReadInt("New Height", 700);
	Toggles["Toggle Resolution"] = BH::config->ReadToggle("Toggle Resolution", "VK_6", false);
	panelPositionPatch->Install();
}

void Resolution::LoadConfig() {
	
}

void Resolution::OnUnload() {
	panelPositionPatch->Remove();
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

void __declspec(naked) PanelPosition_Interception(void) {
	__asm
	{
		; Check that the mode is set to HD, otherwise run 640 mode code
			cmp eax, 03

			pushad
			mov eax, p_D2CLIENT_TabXOffset
			jne PositionMenuTab_Mode640

			; HD positions
			; Position X offset
			mov dword ptr ds : [eax], 352
			; Position Y offset
			mov dword ptr ds : [eax + 4], -110

			jmp PositionMenuTab_RepositionEnd

		PositionMenuTab_Mode640 :
		mov dword ptr ds : [eax], 0
			mov dword ptr ds : [eax + 4], 0

		PositionMenuTab_RepositionEnd :
									  popad
									  ret
	}
}

