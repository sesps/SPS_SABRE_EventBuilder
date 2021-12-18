#include "EVBMainFrame.h"
#include "FileViewFrame.h"
#include <TGLabel.h>
#include <TGTextBuffer.h>
#include <TApplication.h>
#include <TSystem.h>

EVBMainFrame::EVBMainFrame(const TGWindow* p, UInt_t w, UInt_t h) : 
	TGMainFrame(p, w, h, kVerticalFrame)
{
	SetCleanup(kDeepCleanup);
	MAIN_W = w; MAIN_H = h;

	fInfo = new TGFileInfo();
	//Organization hints
	TGLayoutHints *fchints = new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,5,5,5,5);
	TGLayoutHints *fhints = new TGLayoutHints(kLHintsExpandX|kLHintsCenterY,5,5,5,5);
	TGLayoutHints *lhints = new TGLayoutHints(kLHintsCenterY|kLHintsLeft,5,5,5,5);
	TGLayoutHints *bhints = new TGLayoutHints(kLHintsLeft|kLHintsCenterY,5,5,5,5);
	TGLayoutHints *fpbhints = new TGLayoutHints(kLHintsExpandX|kLHintsBottom,5,5,5,5);
	TGLayoutHints *mhints = new TGLayoutHints(kLHintsTop|kLHintsLeft,0,4,0,0);

	//Make the containers and link up all signals/slots

	TGGroupFrame* pathGroup = new TGGroupFrame(this, "Working Paths");
	TGVerticalFrame *NameFrame = new TGVerticalFrame(pathGroup, w, h*0.3);

	TGHorizontalFrame *WorkFrame = new TGHorizontalFrame(NameFrame, w, h*0.06);
	TGLabel* workLabel = new TGLabel(WorkFrame, "Workspace Directory:");
	fWorkField = new TGTextEntry(WorkFrame, new TGTextBuffer(120), WorkDir);
	fWorkField->Resize(w*0.25, fWorkField->GetDefaultHeight());
	fWorkField->Connect("ReturnPressed()","EVBMainFrame",this,"UpdateWorkdir()");
	fOpenWorkButton = new TGTextButton(WorkFrame, "Open");
	fOpenWorkButton->Connect("Clicked()","EVBMainFrame",this,"DoOpenWorkdir()");
	WorkFrame->AddFrame(workLabel, lhints);
	WorkFrame->AddFrame(fWorkField, fhints);
	WorkFrame->AddFrame(fOpenWorkButton, bhints);

	TGHorizontalFrame *CMapFrame = new TGHorizontalFrame(NameFrame, w, h*0.06);
	TGLabel* cmaplabel = new TGLabel(CMapFrame, "Channel Map File:");
	fCMapField = new TGTextEntry(CMapFrame, new TGTextBuffer(120), Cmap);
	fCMapField->Resize(w*0.25, fCMapField->GetDefaultHeight());
	fCMapField->Connect("ReturnPressed()","EVBMainFrame",this,"UpdateCMap()");
	fOpenCMapButton = new TGTextButton(CMapFrame, "Open");
	fOpenCMapButton->Connect("Clicked()","EVBMainFrame",this,"DoOpenCMapfile()");
	CMapFrame->AddFrame(cmaplabel, lhints);
	CMapFrame->AddFrame(fCMapField, fhints);
	CMapFrame->AddFrame(fOpenCMapButton, bhints);

	TGHorizontalFrame *SMapFrame = new TGHorizontalFrame(NameFrame, w, h*0.06);
	TGLabel* smaplabel = new TGLabel(SMapFrame, "Board Shift File:");
	fSMapField = new TGTextEntry(SMapFrame, new TGTextBuffer(120), Smap);
	fSMapField->Resize(w*0.25, fSMapField->GetDefaultHeight());
	fSMapField->Connect("ReturnPressed()","EVBMainFrame",this,"UpdateSMap()");
	fOpenSMapButton = new TGTextButton(SMapFrame, "Open");
	fOpenSMapButton->Connect("Clicked()","EVBMainFrame",this,"DoOpenSMapfile()");
	SMapFrame->AddFrame(smaplabel, lhints);
	SMapFrame->AddFrame(fSMapField, fhints);
	SMapFrame->AddFrame(fOpenSMapButton, bhints);

	TGHorizontalFrame *ScalerFrame = new TGHorizontalFrame(NameFrame, w, h*0.06);
	TGLabel* sclabel = new TGLabel(ScalerFrame, "Scaler File: ");
	fScalerField = new TGTextEntry(ScalerFrame, new TGTextBuffer(120), Scaler);
	fScalerField->Connect("ReturnPressed()","EVBMainFrame",this,"UpdateScaler()");
	fOpenScalerButton = new TGTextButton(ScalerFrame, "Open");
	fOpenScalerButton->Connect("Clicked()","EVBMainFrame", this, "DoOpenScalerfile()");
	ScalerFrame->AddFrame(sclabel, lhints);
	ScalerFrame->AddFrame(fScalerField, fhints);
	ScalerFrame->AddFrame(fOpenScalerButton, bhints);

	TGHorizontalFrame *CutFrame = new TGHorizontalFrame(NameFrame, w, h*0.06);
	TGLabel* clabel = new TGLabel(CutFrame, "Cut List: ");
	fCutField = new TGTextEntry(CutFrame, new TGTextBuffer(120), Cut);
	fCutField->Connect("ReturnPressed()","EVBMainFrame",this,"UpdateCut()");
	fOpenCutButton = new TGTextButton(CutFrame, "Open");
	fOpenCutButton->Connect("Clicked()","EVBMainFrame",this,"DoOpenCutfile()");
	CutFrame->AddFrame(clabel, lhints);
	CutFrame->AddFrame(fCutField, fhints);
	CutFrame->AddFrame(fOpenCutButton, bhints);

	NameFrame->AddFrame(WorkFrame, fhints);
	NameFrame->AddFrame(CMapFrame, fhints);
	NameFrame->AddFrame(SMapFrame, fhints);
	NameFrame->AddFrame(ScalerFrame, fhints);
	NameFrame->AddFrame(CutFrame, fhints);

	pathGroup->AddFrame(NameFrame, fhints);


	TGHorizontalFrame *ParamFrame = new TGHorizontalFrame(this, w, h*0.4);

	TGGroupFrame* reactionGroup = new TGGroupFrame(ParamFrame, "Reaction Inputs");
	TGHorizontalFrame* reactionFrame = new TGHorizontalFrame(reactionGroup, w*0.7, h*0.5);

	TGVerticalFrame* targFrame = new TGVerticalFrame(reactionFrame, w*0.175, h*0.5);
	TGHorizontalFrame* zTargFrame = new TGHorizontalFrame(targFrame, w*0.3, h*0.2);
	TGLabel *ztlabel = new TGLabel(zTargFrame, "ZT:");
	fZTField = new TGNumberEntryField(zTargFrame, ZT, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	zTargFrame->AddFrame(ztlabel, lhints);
	zTargFrame->AddFrame(fZTField, fhints);
	TGHorizontalFrame* aTargFrame = new TGHorizontalFrame(targFrame, w*0.175, h*0.2);
	TGLabel *atlabel = new TGLabel(aTargFrame, "AT:");
	fATField = new TGNumberEntryField(aTargFrame, AT, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	aTargFrame->AddFrame(atlabel, lhints);
	aTargFrame->AddFrame(fATField, fhints);
	targFrame->AddFrame(zTargFrame, fhints);
	targFrame->AddFrame(aTargFrame, fhints);

	TGVerticalFrame* projFrame = new TGVerticalFrame(reactionFrame, w*0.175, h*0.4);
	TGHorizontalFrame* zProjFrame = new TGHorizontalFrame(projFrame, w*0.175, h*0.2);
	TGLabel *zplabel = new TGLabel(zProjFrame, "ZP:");
	fZPField = new TGNumberEntryField(zProjFrame, ZP, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	zProjFrame->AddFrame(zplabel, lhints);
	zProjFrame->AddFrame(fZPField, fhints);
	TGHorizontalFrame* aProjFrame = new TGHorizontalFrame(projFrame, w*0.175, h*0.2);
	TGLabel *aplabel = new TGLabel(aProjFrame, "AP:");
	fAPField = new TGNumberEntryField(aProjFrame, AP, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	aProjFrame->AddFrame(aplabel, lhints);
	aProjFrame->AddFrame(fAPField, fhints);
	projFrame->AddFrame(zProjFrame, fhints);
	projFrame->AddFrame(aProjFrame, fhints);

	TGVerticalFrame* ejectFrame = new TGVerticalFrame(reactionFrame, w*0.175, h*0.4);
	TGHorizontalFrame* zEjectFrame = new TGHorizontalFrame(ejectFrame, w*0.175, h*0.2);
	TGLabel *zelabel = new TGLabel(zEjectFrame, "ZE:");
	fZEField = new TGNumberEntryField(zEjectFrame, ZE, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	zEjectFrame->AddFrame(zelabel, lhints);
	zEjectFrame->AddFrame(fZEField, fhints);
	TGHorizontalFrame* aEjectFrame = new TGHorizontalFrame(ejectFrame, w*0.175, h*0.2);
	TGLabel *aelabel = new TGLabel(aEjectFrame, "AE:");
	fAEField = new TGNumberEntryField(aEjectFrame, AE, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	aEjectFrame->AddFrame(aelabel, lhints);
	aEjectFrame->AddFrame(fAEField, fhints);
	ejectFrame->AddFrame(zEjectFrame, fhints);
	ejectFrame->AddFrame(aEjectFrame, fhints);

	TGVerticalFrame* extraFrame = new TGVerticalFrame(reactionFrame, w*0.175, h*0.4);
	TGHorizontalFrame* beamFrame = new TGHorizontalFrame(extraFrame, w*0.175, h*0.15);
	TGLabel *bkelabel = new TGLabel(beamFrame, "Beam KE (MeV):");
	fBKEField = new TGNumberEntryField(beamFrame, Bke, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	beamFrame->AddFrame(bkelabel, lhints);
	beamFrame->AddFrame(fBKEField, fhints);
	TGHorizontalFrame* bfFrame = new TGHorizontalFrame(extraFrame, w*0.175, h*0.15);
	TGLabel *bfieldlabel = new TGLabel(bfFrame, "B-Field (G):");
	fBField = new TGNumberEntryField(bfFrame, BField, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	bfFrame->AddFrame(bfieldlabel, lhints);
	bfFrame->AddFrame(fBField, fhints);
	TGHorizontalFrame* thetaFrame = new TGHorizontalFrame(extraFrame, w*0.175, h*0.15);
	TGLabel *thetalabel = new TGLabel(thetaFrame, "Angle (deg):");
	fThetaField = new TGNumberEntryField(thetaFrame, Theta, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	thetaFrame->AddFrame(thetalabel, lhints);
	thetaFrame->AddFrame(fThetaField, fhints);
	extraFrame->AddFrame(beamFrame, fhints);
	extraFrame->AddFrame(bfFrame, fhints);
	extraFrame->AddFrame(thetaFrame, fhints);

	reactionFrame->AddFrame(targFrame, fhints);
	reactionFrame->AddFrame(projFrame, fhints);
	reactionFrame->AddFrame(ejectFrame, fhints);
	reactionFrame->AddFrame(extraFrame, fhints);

	reactionGroup->AddFrame(reactionFrame, fhints);


	TGGroupFrame* eventGroup = new TGGroupFrame(ParamFrame, "Event Inputs");
	TGVerticalFrame* eventFrame = new TGVerticalFrame(eventGroup, w*0.3, h*0.5);

	TGHorizontalFrame *slowFrame = new TGHorizontalFrame(eventFrame, w*0.4, h*0.1);
	TGLabel *slowlabel = new TGLabel(slowFrame, "Slow Coincidence Window (ps):");
	fSlowWindowField = new TGNumberEntryField(slowFrame, SlowWind, 0, TGNumberEntry::kNESReal, TGNumberEntry::kNEANonNegative);
	slowFrame->AddFrame(slowlabel, lhints);
	slowFrame->AddFrame(fSlowWindowField, fhints);

	TGHorizontalFrame* fastICFrame = new TGHorizontalFrame(eventFrame, w*0.3, h*0.1);
	TGLabel *fasticlabel = new TGLabel(fastICFrame, "Fast Coincidence Window IC (ps):");
	fFastICField = new TGNumberEntryField(fastICFrame, FastWind_IC, 0, TGNumberEntry::kNESReal, TGNumberEntry::kNEANonNegative);
	fastICFrame->AddFrame(fasticlabel, lhints);
	fastICFrame->AddFrame(fFastICField, fhints);

	TGHorizontalFrame* fastSABREFrame = new TGHorizontalFrame(eventFrame, w*0.3, h*0.1);
	TGLabel *fastsabrelabel = new TGLabel(fastSABREFrame, "Fast Coincidence Window SABRE (ps):");
	fFastSABREField = new TGNumberEntryField(fastSABREFrame, FastWind_Sabre, 0, TGNumberEntry::kNESReal, TGNumberEntry::kNEANonNegative);
	fastSABREFrame->AddFrame(fastsabrelabel, lhints);
	fastSABREFrame->AddFrame(fFastSABREField, fhints);

	TGHorizontalFrame *opFrame = new TGHorizontalFrame(eventFrame, w*0.3, h*0.1);
	TGLabel *typelabel = new TGLabel(opFrame, "Operation Type:");
	fTypeBox = new TGComboBox(opFrame, TypeBox);
	//Needs modification for new conversion based sorting GWM -- Dec 2020
	fTypeBox->AddEntry("Convert Slow", EventBuilder::EVBApp::Operation::ConvertSlow);
	fTypeBox->AddEntry("Convert Fast", EventBuilder::EVBApp::Operation::ConvertFast);
	fTypeBox->AddEntry("Convert SlowA", EventBuilder::EVBApp::Operation::ConvertSlowA);
	fTypeBox->AddEntry("Convert FastA", EventBuilder::EVBApp::Operation::ConvertFastA);
	fTypeBox->AddEntry("Convert", EventBuilder::EVBApp::Operation::Convert);
	fTypeBox->AddEntry("Merge ROOT", EventBuilder::EVBApp::Operation::Merge);
	fTypeBox->AddEntry("Plot", EventBuilder::EVBApp::Operation::Plot);
	fTypeBox->Resize(200,20);
	fTypeBox->Connect("Selected(Int_t, Int_t)","EVBMainFrame",this,"HandleTypeSelection(Int_t,Int_t)");
	opFrame->AddFrame(typelabel, lhints);
	opFrame->AddFrame(fTypeBox, fhints);

	TGHorizontalFrame* rminFrame = new TGHorizontalFrame(eventFrame, w*0.3, h*0.1);
	TGLabel *rminlabel = new TGLabel(rminFrame, "Min Run:");
	fRMinField = new TGNumberEntryField(rminFrame, RMin, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	rminFrame->AddFrame(rminlabel, lhints);
	rminFrame->AddFrame(fRMinField, fhints);

	TGHorizontalFrame* rmaxFrame = new TGHorizontalFrame(eventFrame, w*0.3, h*0.1);
	TGLabel *rmaxlabel = new TGLabel(rmaxFrame, "Max Run:");
	fRMaxField = new TGNumberEntryField(rmaxFrame, RMax, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	rmaxFrame->AddFrame(rmaxlabel, lhints);
	rmaxFrame->AddFrame(fRMaxField, fhints);

	fRunButton = new TGTextButton(eventFrame, "Run!");
	fRunButton->SetState(kButtonDisabled);
	fRunButton->Connect("Clicked()","EVBMainFrame",this,"DoRun()");
	
	eventFrame->AddFrame(slowFrame, fhints);
	eventFrame->AddFrame(fastICFrame, fhints);
	eventFrame->AddFrame(fastSABREFrame, fhints);
	eventFrame->AddFrame(opFrame, fhints);
	eventFrame->AddFrame(rminFrame, fhints);
	eventFrame->AddFrame(rmaxFrame, fhints);
	eventFrame->AddFrame(fRunButton, bhints);

	eventGroup->AddFrame(eventFrame,fhints);

	ParamFrame->AddFrame(reactionGroup, fhints);
	ParamFrame->AddFrame(eventGroup, new TGLayoutHints(kLHintsExpandY|kLHintsLeft,5,5,5,5));

	TGVerticalFrame *PBFrame = new TGVerticalFrame(this, w, h*0.1);
	TGLabel *pbLabel = new TGLabel(PBFrame, "Build Progress");
	fProgressBar = new TGHProgressBar(PBFrame, TGProgressBar::kFancy, w);
	fProgressBar->ShowPosition();
	fProgressBar->SetBarColor("lightblue");
	//fBuilder.AttachProgressBar(fProgressBar);
	PBFrame->AddFrame(pbLabel, lhints);
	PBFrame->AddFrame(fProgressBar, fhints);

	TGMenuBar* menuBar = new TGMenuBar(this, w, h*0.1);
	fFileMenu = new TGPopupMenu(gClient->GetRoot());
	fFileMenu->AddEntry("Load...", M_Load_Config);
	fFileMenu->AddEntry("Save...", M_Save_Config);
	fFileMenu->AddEntry("Exit", M_Exit);
	fFileMenu->Connect("Activated(Int_t)","EVBMainFrame", this, "HandleMenuSelection(Int_t)");
	menuBar->AddPopup("File", fFileMenu, mhints);

	AddFrame(menuBar, new TGLayoutHints(kLHintsTop|kLHintsLeft,0,0,0,0));
	AddFrame(pathGroup, new TGLayoutHints(kLHintsTop|kLHintsExpandX,5,5,5,5));
	AddFrame(ParamFrame, new TGLayoutHints(kLHintsCenterX|kLHintsExpandY,5,5,5,5));
	AddFrame(PBFrame, fpbhints);

	fBuilder.SetProgressCallbackFunc(BIND_PROGRESS_CALLBACK_FUNCTION(EVBMainFrame::SetProgressBarPosition));
	fBuilder.SetProgressFraction(0.01);
	SetWindowName("GWM Event Builder");
	MapSubwindows();
	Resize();
	MapWindow();

}

EVBMainFrame::~EVBMainFrame() 
{
	Cleanup();
	delete fInfo;
	delete this;
}

void EVBMainFrame::CloseWindow() 
{
	gApplication->Terminate();
}

void EVBMainFrame::HandleMenuSelection(int id) 
{
	if(id == M_Save_Config)
	{
		new TGFileDialog(gClient->GetRoot(), this, kFDOpen, fInfo);
		if(fInfo->fFilename)
			SaveConfig(fInfo->fFilename);
	}
	else if(id == M_Load_Config)
	{
		new TGFileDialog(gClient->GetRoot(), this, kFDOpen, fInfo);
		if(fInfo->fFilename)
			LoadConfig(fInfo->fFilename);
	}
	else if(id == M_Exit)
		CloseWindow();
}

void EVBMainFrame::DoOpenWorkdir() 
{
	new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H, this, WorkDir);
}

void EVBMainFrame::DoOpenCMapfile() 
{
	new TGFileDialog(gClient->GetRoot(), this, kFDOpen, fInfo);
	if(fInfo->fFilename)
		DisplayCMap(fInfo->fFilename);
}

void EVBMainFrame::DoOpenSMapfile() 
{
	new TGFileDialog(gClient->GetRoot(), this, kFDOpen, fInfo);
	if(fInfo->fFilename)
		DisplaySMap(fInfo->fFilename);
}

void EVBMainFrame::DoOpenScalerfile() 
{
	new TGFileDialog(gClient->GetRoot(), this, kFDOpen, fInfo);
	if(fInfo->fFilename)
		DisplayScaler(fInfo->fFilename);
}

void EVBMainFrame::DoOpenCutfile() 
{
	new TGFileDialog(gClient->GetRoot(), this, kFDOpen, fInfo);
	if(fInfo->fFilename)
		DisplayCut(fInfo->fFilename);
}

void EVBMainFrame::DoRun() 
{

	DisableAllInput();

	SetParameters();

	int type = fTypeBox->GetSelected();

	switch(type)
	{
		case EventBuilder::EVBApp::Operation::Plot :
		{
			RunPlot();
			break;
		}
		case EventBuilder::EVBApp::Operation::Convert :
		{
			fBuilder.Convert2RawRoot();
			break;
		}
		case EventBuilder::EVBApp::Operation::Merge :
		{
			fBuilder.MergeROOTFiles();
			break;
		}
		case EventBuilder::EVBApp::Operation::ConvertSlow :
		{
			fBuilder.Convert2SortedRoot();
			break;
		}
		case EventBuilder::EVBApp::Operation::ConvertFast :
		{
			fBuilder.Convert2FastSortedRoot();
			break;
		}
		case EventBuilder::EVBApp::Operation::ConvertSlowA :
		{
			fBuilder.Convert2SlowAnalyzedRoot();
			break;
		}
		case EventBuilder::EVBApp::Operation::ConvertFastA :
		{
			fBuilder.Convert2FastAnalyzedRoot();
			break;
		}
	}

	EnableAllInput();
}

void EVBMainFrame::HandleTypeSelection(int box, int entry)
{
	fRunButton->SetState(kButtonUp);
}

bool EVBMainFrame::SetParameters()
{
	fBuilder.SetRunRange(fRMinField->GetIntNumber(), fRMaxField->GetIntNumber());
	fBuilder.SetSlowCoincidenceWindow(fSlowWindowField->GetNumber());
	fBuilder.SetFastWindowIonChamber(fFastICField->GetNumber());
	fBuilder.SetFastWindowSABRE(fFastSABREField->GetNumber());
	UpdateWorkdir();
	UpdateSMap();
	UpdateCMap();
	UpdateScaler();
	UpdateCut();
	bool test = fBuilder.SetKinematicParameters(fZTField->GetIntNumber(), fATField->GetIntNumber(),
												fZPField->GetIntNumber(), fAPField->GetIntNumber(),
												fZEField->GetIntNumber(), fAEField->GetIntNumber(),
												fBField->GetNumber(), fThetaField->GetNumber(),
												fBKEField->GetNumber());
	return test;
}

void EVBMainFrame::DisplayWorkdir(const char* dir) 
{
	fWorkField->SetText(dir);
	fBuilder.SetWorkDirectory(dir);
}

void EVBMainFrame::DisplayCMap(const char* file) 
{
	fCMapField->SetText(file);
	fBuilder.SetChannelMap(file);
}

void EVBMainFrame::DisplaySMap(const char* file) 
{
	fSMapField->SetText(file);
	fBuilder.SetBoardShiftFile(file);
}

void EVBMainFrame::DisplayScaler(const char* file) 
{
	fScalerField->SetText(file);
	fBuilder.SetScalerFile(file);
}

void EVBMainFrame::DisplayCut(const char* file) 
{
	fCutField->SetText(file);
	fBuilder.SetCutList(file);
}

void EVBMainFrame::SaveConfig(const char* file) 
{
	std::string filename = file;
	fBuilder.WriteConfigFile(filename);
}

void EVBMainFrame::LoadConfig(const char* file) 
{
	std::string filename = file;
	fBuilder.ReadConfigFile(filename);

	fWorkField->SetText(fBuilder.GetWorkDirectory().c_str());
	fCMapField->SetText(fBuilder.GetChannelMap().c_str());
	fSMapField->SetText(fBuilder.GetBoardShiftFile().c_str());
	fCutField->SetText(fBuilder.GetCutList().c_str());
	fScalerField->SetText(fBuilder.GetScalerFile().c_str());
	
	fZTField->SetIntNumber(fBuilder.GetTargetZ());
	fATField->SetIntNumber(fBuilder.GetTargetA());
	fZPField->SetIntNumber(fBuilder.GetProjectileZ());
	fAPField->SetIntNumber(fBuilder.GetProjectileA());
	fZEField->SetIntNumber(fBuilder.GetEjectileZ());
	fAEField->SetIntNumber(fBuilder.GetEjectileA());
	fBKEField->SetNumber(fBuilder.GetBeamKE());
	fBField->SetNumber(fBuilder.GetBField());
	fThetaField->SetNumber(fBuilder.GetTheta());

	fSlowWindowField->SetNumber(fBuilder.GetSlowCoincidenceWindow());
	fFastSABREField->SetNumber(fBuilder.GetFastWindowSABRE());
	fFastICField->SetNumber(fBuilder.GetFastWindowIonChamber());

	fRMaxField->SetIntNumber(fBuilder.GetRunMax());
	fRMinField->SetIntNumber(fBuilder.GetRunMin());

}

void EVBMainFrame::UpdateWorkdir() 
{
	const char* dir = fWorkField->GetText();
	fBuilder.SetWorkDirectory(dir);
}

void EVBMainFrame::UpdateSMap() 
{
	const char* file = fSMapField->GetText();
	fBuilder.SetBoardShiftFile(file);
}

void EVBMainFrame::UpdateCMap() 
{
	const char* file = fCMapField->GetText();
	fBuilder.SetChannelMap(file);
}

void EVBMainFrame::UpdateScaler() 
{
	const char* file = fScalerField->GetText();
	fBuilder.SetScalerFile(file);
}

void EVBMainFrame::UpdateCut() 
{
	const char* file = fCutField->GetText();
	fBuilder.SetCutList(file);
}

void EVBMainFrame::RunPlot() 
{
	fBuilder.PlotHistograms();
}

void EVBMainFrame::RunMerge(const char* file, const char* dir) {}

void EVBMainFrame::DisableAllInput() 
{
	fRunButton->SetState(kButtonDisabled);
	fOpenWorkButton->SetState(kButtonDisabled);
	fOpenCMapButton->SetState(kButtonDisabled);
	fOpenSMapButton->SetState(kButtonDisabled);
	fOpenScalerButton->SetState(kButtonDisabled);
	fOpenCutButton->SetState(kButtonDisabled);

	fWorkField->SetState(false);
	fCMapField->SetState(false);
	fSMapField->SetState(false);
	fScalerField->SetState(false);
	fCutField->SetState(false);

	fTypeBox->SetEnabled(false);

	fZTField->SetState(false);
	fATField->SetState(false);
	fZPField->SetState(false);
	fAPField->SetState(false);
	fZEField->SetState(false);
	fAEField->SetState(false);

	fBField->SetState(false);
	fBKEField->SetState(false);
	fThetaField->SetState(false);

	fRMaxField->SetState(false);
	fRMinField->SetState(false);

	fSlowWindowField->SetState(false);
	fFastICField->SetState(false);
	fFastSABREField->SetState(false);
}

void EVBMainFrame::EnableAllInput() 
{
	fRunButton->SetState(kButtonUp);
	fOpenWorkButton->SetState(kButtonUp);
	fOpenCMapButton->SetState(kButtonUp);
	fOpenSMapButton->SetState(kButtonUp);
	fOpenScalerButton->SetState(kButtonUp);
	fOpenCutButton->SetState(kButtonUp);

	fWorkField->SetState(true);
	fCMapField->SetState(true);
	fSMapField->SetState(true);
	fScalerField->SetState(true);
	fCutField->SetState(true);

	fTypeBox->SetEnabled(true);

	fZTField->SetState(true);
	fATField->SetState(true);
	fZPField->SetState(true);
	fAPField->SetState(true);
	fZEField->SetState(true);
	fAEField->SetState(true);

	fBField->SetState(true);
	fBKEField->SetState(true);
	fThetaField->SetState(true);

	fRMaxField->SetState(true);
	fRMinField->SetState(true);

	fSlowWindowField->SetState(true);
	fFastICField->SetState(true);
	fFastSABREField->SetState(true);

}

void EVBMainFrame::SetProgressBarPosition(long val, long total)
{
	fProgressBar->SetMin(0);
	fProgressBar->SetMax(total);
	fProgressBar->SetPosition(val);
	gSystem->ProcessEvents();
}