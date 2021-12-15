/*
	
	FileViewFrame.h
	Wrapper class on a TGTransientFrame (temporary frame assoc. with a main frame)
	Designed to graphically display directories and files for selection. Takes in a type
	to specify the signal pathing.

	Written by G.W. McCann Sep. 2020

*/

#ifndef FILEVIEWFRAME_H
#define FILEVIEWFRAME_H

#include "EVBMainFrame.h"
#include <TGTextEntry.h>
#include <TGFSContainer.h>
#include <TGListView.h>
#include <TQObject.h>
#include <RQ_OBJECT.h>

class FileViewFrame {

	RQ_OBJECT("FileViewFrame"); //ROOT wrapping into the gui environment
public:
	FileViewFrame(const TGWindow* p, const TGFrame* main, UInt_t w, UInt_t h, EVBMainFrame *parent, int type);
	virtual ~FileViewFrame();
	void CloseWindow();
	void DoOk();
	void DoCancel();
	void DoDoubleClick(TGLVEntry* entry, int id);
	void DisplayDir(const TString& name);
	void SendText(const char* text); // *SIGNAL*
	ClassDef(FileViewFrame, 0); //ROOT requirement

private:
	TGTransientFrame *fMain;
	TGTextButton *fOkButton, *fCancelButton;
	TGTextEntry *fNameField;
	TGFileContainer *fContents;
	TGListView *fViewer;
	std::string suffix;
	bool dirFlag;
};

#endif