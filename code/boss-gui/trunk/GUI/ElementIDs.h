/*	General User Interface for BOSS (Better Oblivion Sorting Software)
	
	Providing a graphical frontend to BOSS's functions.

    Copyright (C) 2011 WrinklyNinja & the BOSS development team.
    http://creativecommons.org/licenses/by-nc-nd/3.0/

	$Revision: 2188 $, $Date: 2011-01-20 10:05:16 +0000 (Thu, 20 Jan 2011) $
*/

#ifndef __ELEMENTIDS__HPP__
#define __ELEMENTIDS__HPP__

//We want to ensure that the GUI-specific code in BOSS-Common is included.
#ifndef BOSSGUI
#define BOSSGUI
#endif

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#       include "wx/wx.h"
#endif

enum {
	//Main window.
    OPTION_EditUserRules = wxID_HIGHEST + 1, // declares an id which will be used to call our button
	OPTION_OpenBOSSlog,
	OPTION_Run,
	OPTION_CheckForUpdates,
    MENU_Quit,
	MENU_OpenMainReadMe,
	MENU_OpenUserRulesReadMe,
	MENU_ShowAbout,
	MENU_ShowSettings,
	DROPDOWN_LogFormat,
	DROPDOWN_Game,
	DROPDOWN_Revert,
	CHECKBOX_ShowBOSSlog,
	CHECKBOX_Update,
	CHECKBOX_EnableVersions,
	CHECKBOX_EnableCRCs,
	CHECKBOX_TrialRun,
	RADIOBUTTON_SortOption,
	RADIOBUTTON_UpdateOption,
	RADIOBUTTON_UndoOption,
	//About window.
	OPTION_ExitAbout,
	//Settings window.
	OPTION_OKExitSettings,
	OPTION_CancelExitSettings,
	DROPDOWN_ProxyType
	//User Rules Editor.
};
#endif