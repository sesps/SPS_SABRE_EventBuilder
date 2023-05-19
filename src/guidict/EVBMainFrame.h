#ifndef EVBMAINFRAME_H
#define EVBMAINFRAME_H

#include "../EventBuilder.h"
#include <TGClient.h>
#include <TGWindow.h>
#include <TGFrame.h>
#include <TGNumberEntry.h>
#include <TGTextEntry.h>
#include <TGButton.h>
#include <TGMenu.h>
#include <TGTextViewStream.h>
#include <TGProgressBar.h>
#include <TTimer.h>
#include <TGFileDialog.h>
#include <TGComboBox.h>
#include "../evb/EVBApp.h"


class EVBMainFrame : public TGMainFrame 
{
public:
	EVBMainFrame(const TGWindow* p, UInt_t w, UInt_t h);
	virtual ~EVBMainFrame();
	void CloseWindow();
	void HandleMenuSelection(int id);
	void DoOpenWorkdir();
	void DoOpenCMapfile();
	void DoOpenSMapfile();
	void DoOpenScalerfile();
	void DoOpenCutfile();
	void DoRun();
	void HandleTypeSelection(int box, int entry);
	bool SetParameters();
	void DisplayWorkdir(const char* dir);
	void DisplayCMap(const char* file);
	void DisplaySMap(const char* file);
	void DisplayScaler(const char* file);
	void DisplayCut(const char* file);
	void SaveConfig(const char* file);
	void LoadConfig(const char* file);
	void DisableAllInput();
	void EnableAllInput();
	void SetProgressBarPosition(long value, long total);


	enum WidgetId 
	{
		WorkDir,
		Cmap,
		Smap,
		Scaler,
		Cut,
		PlotF,
		BField,
		Bke,
		Theta,
		ZT,
		AT,
		ZP,
		AP,
		ZE,
		AE,
		SlowWind,
		FastWind_IC,
		FastWind_Sabre,
		TypeBox,
		RMin,
		RMax,
		Nudge,
		M_Load_Config,
		M_Save_Config,
		M_Exit
	};

	ClassDef(EVBMainFrame, 0);

private:
	TGTextButton *fRunButton, *fOpenWorkButton, *fOpenCMapButton, *fOpenSMapButton, *fOpenScalerButton, *fOpenCutButton;
	TGTextEntry *fWorkField;
	TGTextEntry *fCMapField, * fSMapField;
	TGTextEntry *fScalerField, *fCutField;
	TGComboBox *fTypeBox;

	TGNumberEntryField *fZTField, *fATField, *fZPField, *fAPField, *fZEField, *fAEField;
	TGNumberEntryField *fBField, *fBKEField, *fThetaField;
	TGNumberEntryField *fSlowWindowField, *fFastICField, *fFastSABREField;
	TGNumberEntryField *fRMinField, *fRMaxField;
	TGNumberEntryField *fNudgeField;

	TGHProgressBar* fProgressBar;

	TGPopupMenu *fFileMenu;

	TGFileInfo* fInfo;

	EventBuilder::EVBApp m_builder;
	EventBuilder::EVBParameters m_parameters;

	int counter;
	UInt_t MAIN_W, MAIN_H;
};
#endif
