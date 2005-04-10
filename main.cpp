///////////////////////////////////////////////////////////////////////////////
// Application : MoleSVN
// Package     : 
// File        : main.cpp
// Description : defines the entry point of the add-on 
// Author      : cedric.bresson@artcoder.com
///////////////////////////////////////////////////////////////////////////////
#include <Application.h> 
#include <InterfaceKit.h> 
#include <StorageKit.h> 

#include <stdio.h> 
#include <string.h> 
#include <list>

#include <be/add-ons/tracker/TrackerAddon.h>
#include "Config.h"
#include "MoleSvnAddon.h"


using namespace std;

void process_refs(entry_ref dir_ref, BMessage *msg, void *) 
{
	TRACE_METHOD ((CC_APPLICATION, REPORT_METHOD, "MoleSvn starts here!"));
	
	try
	{
		// check if the user has seleted files...
		int nRefs; 
		entry_ref file_ref; 
		list<entry_ref> lstEntry;	
		for (nRefs=0; msg->FindRef("refs", nRefs, &file_ref) == B_NO_ERROR; nRefs++) 
		{
			lstEntry.push_back(file_ref);
		}

		// Create the instance of the addon
		MoleSvnAddon MyAddon(dir_ref, lstEntry);
	
		// Retrieve mouse position
		// I don't know a much safer method...
		BWindow* pWindow = new BWindow(BRect(0, 0, 100, 100), "MoleSVN", B_TITLED_WINDOW, B_NO_WORKSPACE_ACTIVATION);
		BView* pView = new BView(BRect(0, 0, 100, 100), "MoleSVN", B_FOLLOW_H_CENTER, B_WILL_DRAW);
		pWindow->AddChild(pView);
		BPoint MousePosition;
		uint32 nButtons;
		pView->GetMouse(&MousePosition, &nButtons);
		pView->ConvertToScreen(&MousePosition);
		pWindow->Quit();
		MousePosition.x -= 10;
		MousePosition.y -= 10;
		TRACE_OBJECT ((CC_APPLICATION, CR_OBJECT, &MousePosition, "Mouse position"));
	
		// Show the menu, depending of the selected files 
		MyAddon.ShowMenu(MousePosition);
	}
	catch(MoleSvnException e)
	{
		(new BAlert(0, 
					(string("Error : \n\n") + string(e.what())).c_str(),
					"Okay",
					0,
					0,
					B_WIDTH_AS_USUAL,
					B_STOP_ALERT))->Go();
	}
	catch(...)
	{
		(new BAlert(0, 
					"Error : \n\nunhandled exception",
					"Okay",
					0,
					0,
					B_WIDTH_AS_USUAL,
					B_STOP_ALERT))->Go();
	}
} 

int main() 
{ 
	new BApplication("application/x-artcoder-add-on"); 
	
	// Show a tiny message ;)
	ShowAboutWindow();
	
	delete be_app; 
	return 0;
}