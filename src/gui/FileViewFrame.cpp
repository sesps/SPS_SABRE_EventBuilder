/*
	
	FileViewFrame.cpp
	Wrapper class on a TGTransientFrame (temporary frame assoc. with a main frame)
	Designed to graphically display directories and files for selection. Takes in a type
	to specify the signal pathing.

	Written by G.W. McCann Sep. 2020

*/

#include "EventBuilder.h"
#include "FileViewFrame.h"
#include <TGTextBuffer.h>
#include <TGLabel.h>
#include <TTimer.h>

FileViewFrame::FileViewFrame(const TGWindow* p, const TGFrame* main, UInt_t w, UInt_t h, EVBMainFrame *parent, int type) {
	fMain = new TGTransientFrame(p,main,w,h);
	fMain->SetCleanup(kDeepCleanup); //delete all child frames
	fMain->DontCallClose(); //Close button on window disabled

	dirFlag = false;
	bool rootFlag = false;
	suffix = ".txt";
	if(type == EVBMainFrame::WORKDIR) {
		dirFlag = true;
		suffix = ".NOTHING";
	} else if(type == EVBMainFrame::PLOTF) {
		rootFlag = true;
		suffix = ".root";
	}

	/*Layout orgainization hints*/
	TGLayoutHints *fhints = new TGLayoutHints(kLHintsCenterX|kLHintsCenterY,5,5,5,5);
	TGLayoutHints *thints = new TGLayoutHints(kLHintsExpandX|kLHintsCenterY,5,5,5,5);
	TGLayoutHints *fchints = new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,5,5,5,5);
	TGLayoutHints *lhints = new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5);
	TGLayoutHints *fbhints = new TGLayoutHints(kLHintsCenterX|kLHintsBottom,5,5,5,5);

	/*Create object for displaying*/
	fViewer = new TGListView(fMain, w, h*0.5);
	Pixel_t white;
	gClient->GetColorByName("white", white);
	fContents = new TGFileContainer(fViewer, kSunkenFrame, white);
	fContents->Connect("DoubleClicked(TGFrame*,Int_t)","FileViewFrame",this,"DoDoubleClick(TGLVEntry*,Int_t)");

	/*Add in text options*/
	TGVerticalFrame *NameFrame = new TGVerticalFrame(fMain, w, h*0.25);
	TGLabel *nameLabel;
	if(dirFlag) nameLabel = new TGLabel(NameFrame, "Dir:");
	else nameLabel = new TGLabel(NameFrame, "File:");
	TGTextBuffer* fNameBuffer;
	fNameField = new TGTextEntry(NameFrame, fNameBuffer = new TGTextBuffer(50));
	fNameField->Resize(w*0.5, fNameField->GetDefaultHeight());
	NameFrame->AddFrame(nameLabel, lhints);
	NameFrame->AddFrame(fNameField, thints);

	/*Buttons for ok and cancel*/
	TGHorizontalFrame *ButtonFrame = new TGHorizontalFrame(fMain, w, h*0.25);
	fOkButton = new TGTextButton(ButtonFrame, "Ok");
	fOkButton->Connect("Clicked()","FileViewFrame",this,"DoOk()");
	fCancelButton = new TGTextButton(ButtonFrame, "Cancel");
	fCancelButton->Connect("Clicked()","FileViewFrame",this,"DoCancel()");
	ButtonFrame->AddFrame(fOkButton, fhints);
	ButtonFrame->AddFrame(fCancelButton, fhints);

	fMain->AddFrame(fViewer, fchints);
	fMain->AddFrame(NameFrame, thints);
	fMain->AddFrame(ButtonFrame, fbhints);

	/*Send signal to appropriate location*/
	if(type == EVBMainFrame::WORKDIR) Connect("SendText(const char*)","EVBMainFrame",parent,"DisplayWorkdir(const char*)");
	else if(type == EVBMainFrame::CMAP) Connect("SendText(const char*)","EVBMainFrame",parent,"DisplayCMap(const char*)");
	else if(type == EVBMainFrame::SMAP) Connect("SendText(const char*)","EVBMainFrame",parent,"DisplaySMap(const char*)");
	else if(type == EVBMainFrame::SCALER) Connect("SendText(const char*)","EVBMainFrame",parent,"DisplayScaler(const char*)");
	else if(type == EVBMainFrame::CUT) Connect("SendText(const char*)","EVBMainFrame",parent,"DisplayCut(const char*)");
	else if(type == EVBMainFrame::M_LOAD_CONFIG) Connect("SendText(const char*)","EVBMainFrame",parent,"LoadConfig(const char*)");
	else if(type == EVBMainFrame::M_SAVE_CONFIG) Connect("SendText(const char*)","EVBMainFrame",parent,"SaveConfig(const char*)");
	else if(type == EVBMainFrame::PLOTF) Connect("SendText(const char*)","EVBMainFrame",parent,"RunPlot(const char*)");

	fMain->SetWindowName("Select File");
	fMain->MapSubwindows();
	fMain->Resize();
	fMain->CenterOnParent();
	fMain->MapWindow();

	fContents->SetDefaultHeaders();
	if(dirFlag) fContents->SetFilter("*.NOTHING"); //relevant extension
	else if(rootFlag) fContents->SetFilter("*.root");
	else fContents->SetFilter("*.txt");
	fContents->DisplayDirectory();
	fContents->AddFile(".."); //go back a dir
	fContents->Resize();
	fContents->StopRefreshTimer();

	fMain->Resize();
}

FileViewFrame::~FileViewFrame() {
	fMain->Cleanup(); //delete children
	fMain->DeleteWindow();
}

void FileViewFrame::CloseWindow() {
	delete this;
}

void FileViewFrame::DoOk() {
	/*Prevent user from doing something dumb*/
	fOkButton->SetState(kButtonDisabled);
	fCancelButton->SetState(kButtonDisabled);

	TString filename = fNameField->GetText();
	TString fullpath = TString(fContents->GetDirectory()) + "/" + filename;
	if(fullpath == "") { //check validity
		std::cerr<<"Need to give a name!"<<std::endl;
		fOkButton->SetState(kButtonUp);
		fCancelButton->SetState(kButtonUp);
		return;
	}

	SendText(fullpath.Data()); //signal sent
	//Destroy this frame after a brief pause (to make sure memory isnt freed too quickly)
	TTimer::SingleShot(150,"FileViewFrame",this,"CloseWindow()");
}

void FileViewFrame::DoCancel() {
	/*Prevent user from doing something dumb*/
	fOkButton->SetState(kButtonDisabled);
	fCancelButton->SetState(kButtonDisabled);

	//Destroy this frame after a brief pause (to make sure memory isnt freed too quickly)
	TTimer::SingleShot(150,"FileViewFrame",this,"CloseWindow()");
}

//Handle directory selection
void FileViewFrame::DisplayDir(const TString& name) {
	fContents->SetDefaultHeaders();
	fContents->ChangeDirectory(name);
	fContents->DisplayDirectory();
	fContents->AddFile("..");
	fMain->Resize();
}

//Handle double click
void FileViewFrame::DoDoubleClick(TGLVEntry *entry, int id) {
	if( id != kButton1) return;
	TString dirname(fContents->GetDirectory());
	TString entryname(entry->GetTitle());

	if(entryname.EndsWith(suffix.c_str())) { //check if its a file
		TString name = entryname;
		fNameField->SetText(name.Data());
	} else {
		DisplayDir(entryname);
		if(dirFlag) {
			fNameField->SetText((dirname+"/"+entryname).Data());
		}
	}
}

/*SIGNAL*/
void FileViewFrame::SendText(const char* text) {
	Emit("SendText(const char*)", text);
}
