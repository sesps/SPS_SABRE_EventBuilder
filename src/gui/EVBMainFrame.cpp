#include "EventBuilder.h"
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

	//Organization hints
	TGLayoutHints *fchints = new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,5,5,5,5);
	TGLayoutHints *fhints = new TGLayoutHints(kLHintsExpandX|kLHintsCenterY,5,5,5,5);
	TGLayoutHints *lhints = new TGLayoutHints(kLHintsCenterY|kLHintsLeft,5,5,5,5);
	TGLayoutHints *bhints = new TGLayoutHints(kLHintsLeft|kLHintsCenterY,5,5,5,5);
	TGLayoutHints *fpbhints = new TGLayoutHints(kLHintsExpandX|kLHintsBottom,5,5,5,5);
	TGLayoutHints *mhints = new TGLayoutHints(kLHintsTop|kLHintsLeft,0,4,0,0);

	//Make the containers and link up all signals/slots

	TGVerticalFrame *InputFrame = new TGVerticalFrame(this, w, h*0.9);

	TGVerticalFrame *NameFrame = new TGVerticalFrame(InputFrame, w, h*0.4);

	TGHorizontalFrame *WorkFrame = new TGHorizontalFrame(NameFrame, w, h*0.1);
	TGLabel* workLabel = new TGLabel(WorkFrame, "Workspace Directory:");
	fWorkField = new TGTextEntry(WorkFrame, new TGTextBuffer(120), WORKDIR);
	fWorkField->Resize(w*0.25, fWorkField->GetDefaultHeight());
	fWorkField->Connect("ReturnPressed()","EVBMainFrame",this,"UpdateWorkdir()");
	fOpenWorkButton = new TGTextButton(WorkFrame, "Open");
	fOpenWorkButton->Connect("Clicked()","EVBMainFrame",this,"DoOpenWorkdir()");
	WorkFrame->AddFrame(workLabel, lhints);
	WorkFrame->AddFrame(fWorkField, fhints);
	WorkFrame->AddFrame(fOpenWorkButton, bhints);

	TGHorizontalFrame *CMapFrame = new TGHorizontalFrame(NameFrame, w, h*0.1);
	TGLabel* cmaplabel = new TGLabel(CMapFrame, "Channel Map File:");
	fCMapField = new TGTextEntry(CMapFrame, new TGTextBuffer(120), CMAP);
	fCMapField->Resize(w*0.25, fCMapField->GetDefaultHeight());
	fCMapField->Connect("ReturnPressed()","EVBMainFrame",this,"UpdateCMap()");
	fOpenCMapButton = new TGTextButton(CMapFrame, "Open");
	fOpenCMapButton->Connect("Clicked()","EVBMainFrame",this,"DoOpenCMapfile()");
	CMapFrame->AddFrame(cmaplabel, lhints);
	CMapFrame->AddFrame(fCMapField, fhints);
	CMapFrame->AddFrame(fOpenCMapButton, bhints);

	TGHorizontalFrame *SMapFrame = new TGHorizontalFrame(NameFrame, w, h*0.1);
	TGLabel* smaplabel = new TGLabel(SMapFrame, "Board Shift File:");
	fSMapField = new TGTextEntry(SMapFrame, new TGTextBuffer(120), SMAP);
	fSMapField->Resize(w*0.25, fSMapField->GetDefaultHeight());
	fSMapField->Connect("ReturnPressed()","EVBMainFrame",this,"UpdateSMap()");
	fOpenSMapButton = new TGTextButton(SMapFrame, "Open");
	fOpenSMapButton->Connect("Clicked()","EVBMainFrame",this,"DoOpenSMapfile()");
	SMapFrame->AddFrame(smaplabel, lhints);
	SMapFrame->AddFrame(fSMapField, fhints);
	SMapFrame->AddFrame(fOpenSMapButton, bhints);

	TGHorizontalFrame *ScalerFrame = new TGHorizontalFrame(NameFrame, w, h*0.1);
	TGLabel* sclabel = new TGLabel(ScalerFrame, "Scaler File: ");
	fScalerField = new TGTextEntry(ScalerFrame, new TGTextBuffer(120), SCALER);
	fScalerField->Connect("ReturnPressed()","EVBMainFrame",this,"UpdateScaler()");
	fOpenScalerButton = new TGTextButton(ScalerFrame, "Open");
	fOpenScalerButton->Connect("Clicked()","EVBMainFrame", this, "DoOpenScalerfile()");
	ScalerFrame->AddFrame(sclabel, lhints);
	ScalerFrame->AddFrame(fScalerField, fhints);
	ScalerFrame->AddFrame(fOpenScalerButton, bhints);

	TGHorizontalFrame *CutFrame = new TGHorizontalFrame(NameFrame, w, h*0.1);
	TGLabel* clabel = new TGLabel(CutFrame, "Cut List: ");
	fCutField = new TGTextEntry(CutFrame, new TGTextBuffer(120), CUT);
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


	TGHorizontalFrame *ParamFrame = new TGHorizontalFrame(InputFrame, w, h*0.1);
	TGLabel *bkelabel = new TGLabel(ParamFrame, "Beam KE (MeV):");
	fBKEField = new TGNumberEntryField(ParamFrame, BKE, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	TGLabel *bfieldlabel = new TGLabel(ParamFrame, "B-Field (G):");
	fBField = new TGNumberEntryField(ParamFrame, BFIELD, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	TGLabel *thetalabel = new TGLabel(ParamFrame, "Angle (deg):");
	fThetaField = new TGNumberEntryField(ParamFrame, THETA, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	TGLabel *ztlabel = new TGLabel(ParamFrame, "ZT:");
	fZTField = new TGNumberEntryField(ParamFrame, ZT, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	TGLabel *atlabel = new TGLabel(ParamFrame, "AT:");
	fATField = new TGNumberEntryField(ParamFrame, AT, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	TGLabel *zplabel = new TGLabel(ParamFrame, "ZP:");
	fZPField = new TGNumberEntryField(ParamFrame, ZP, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	TGLabel *aplabel = new TGLabel(ParamFrame, "AP:");
	fAPField = new TGNumberEntryField(ParamFrame, AP, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	TGLabel *zelabel = new TGLabel(ParamFrame, "ZE:");
	fZEField = new TGNumberEntryField(ParamFrame, ZE, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	TGLabel *aelabel = new TGLabel(ParamFrame, "AE:");
	fAEField = new TGNumberEntryField(ParamFrame, AE, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	ParamFrame->AddFrame(bkelabel, lhints);
	ParamFrame->AddFrame(fBKEField, fhints);
	ParamFrame->AddFrame(bfieldlabel, lhints);
	ParamFrame->AddFrame(fBField, fhints);
	ParamFrame->AddFrame(thetalabel, lhints);
	ParamFrame->AddFrame(fThetaField, fhints);
	ParamFrame->AddFrame(ztlabel, lhints);
	ParamFrame->AddFrame(fZTField, fhints);
	ParamFrame->AddFrame(atlabel, lhints);
	ParamFrame->AddFrame(fATField, fhints);
	ParamFrame->AddFrame(zplabel, lhints);
	ParamFrame->AddFrame(fZPField, fhints);
	ParamFrame->AddFrame(aplabel, lhints);
	ParamFrame->AddFrame(fAPField, fhints);
	ParamFrame->AddFrame(zelabel, lhints);
	ParamFrame->AddFrame(fZEField, fhints);
	ParamFrame->AddFrame(aelabel, lhints);
	ParamFrame->AddFrame(fAEField, fhints);

	TGHorizontalFrame *WindowFrame = new TGHorizontalFrame(InputFrame, w, h*0.1);
	TGLabel *slowlabel = new TGLabel(WindowFrame, "Slow Coincidence Window (ps):");
	fSlowWindowField = new TGNumberEntryField(WindowFrame, SLOWWIND, 0, TGNumberEntry::kNESReal, TGNumberEntry::kNEANonNegative);
	TGLabel *fasticlabel = new TGLabel(WindowFrame, "Fast Coincidence Window IC (ps):");
	fFastICField = new TGNumberEntryField(WindowFrame, FASTWIND_IC, 0, TGNumberEntry::kNESReal, TGNumberEntry::kNEANonNegative);
	TGLabel *fastsabrelabel = new TGLabel(WindowFrame, "Fast Coincidence Window SABRE (ps):");
	fFastSABREField = new TGNumberEntryField(WindowFrame, FASTWIND_SABRE, 0, TGNumberEntry::kNESReal, TGNumberEntry::kNEANonNegative);
	WindowFrame->AddFrame(slowlabel, lhints);
	WindowFrame->AddFrame(fSlowWindowField, fhints);
	WindowFrame->AddFrame(fasticlabel, lhints);
	WindowFrame->AddFrame(fFastICField, fhints);
	WindowFrame->AddFrame(fastsabrelabel, lhints);
	WindowFrame->AddFrame(fFastSABREField, fhints);

	TGHorizontalFrame *RunFrame = new TGHorizontalFrame(InputFrame, w, h*0.1);
	TGLabel *typelabel = new TGLabel(RunFrame, "Operation Type:");
	fTypeBox = new TGComboBox(RunFrame, TYPEBOX);
	//Needs modification for new conversion based sorting GWM -- Dec 2020
	fTypeBox->AddEntry("Convert Slow", GWMEventBuilder::CONVERT_S);
	fTypeBox->AddEntry("Convert Fast", GWMEventBuilder::CONVERT_F);
	fTypeBox->AddEntry("Convert SlowA", GWMEventBuilder::CONVERT_SA);
	fTypeBox->AddEntry("Convert FastA", GWMEventBuilder::CONVERT_FA);
	fTypeBox->AddEntry("Convert", GWMEventBuilder::CONVERT);
	fTypeBox->AddEntry("Merge ROOT", GWMEventBuilder::MERGE);
	fTypeBox->AddEntry("Plot", GWMEventBuilder::PLOT);
	fTypeBox->Resize(200,20);
	fTypeBox->Connect("Selected(Int_t, Int_t)","EVBMainFrame",this,"HandleTypeSelection(Int_t,Int_t)");
	TGLabel *rminlabel = new TGLabel(RunFrame, "Min Run:");
	fRMinField = new TGNumberEntryField(RunFrame, RMIN, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	TGLabel *rmaxlabel = new TGLabel(RunFrame, "Max Run:");
	fRMaxField = new TGNumberEntryField(RunFrame, RMAX, 0, TGNumberEntry::kNESInteger, TGNumberEntry::kNEANonNegative);
	fRunButton = new TGTextButton(RunFrame, "Run!");
	fRunButton->SetState(kButtonDisabled);
	fRunButton->Connect("Clicked()","EVBMainFrame",this,"DoRun()");
	RunFrame->AddFrame(typelabel, lhints);
	RunFrame->AddFrame(fTypeBox, fhints);
	RunFrame->AddFrame(rminlabel, lhints);
	RunFrame->AddFrame(fRMinField, fhints);
	RunFrame->AddFrame(rmaxlabel, lhints);
	RunFrame->AddFrame(fRMaxField, fhints);
	RunFrame->AddFrame(fRunButton, bhints);

	InputFrame->AddFrame(NameFrame, fhints);
	InputFrame->AddFrame(ParamFrame, fhints);
	InputFrame->AddFrame(WindowFrame, fhints);
	InputFrame->AddFrame(RunFrame, fhints);

	TGVerticalFrame *PBFrame = new TGVerticalFrame(this, w, h*0.1);
	TGLabel *pbLabel = new TGLabel(PBFrame, "Build Progress");
	fProgressBar = new TGHProgressBar(PBFrame, TGProgressBar::kFancy, w);
	fProgressBar->ShowPosition();
	fProgressBar->SetBarColor("lightblue");
	fBuilder.AttachProgressBar(fProgressBar);
	PBFrame->AddFrame(pbLabel, lhints);
	PBFrame->AddFrame(fProgressBar, fhints);

	TGMenuBar* menuBar = new TGMenuBar(this, w, h*0.1);
	fFileMenu = new TGPopupMenu(gClient->GetRoot());
	fFileMenu->AddEntry("Load...", M_LOAD_CONFIG);
	fFileMenu->AddEntry("Save...", M_SAVE_CONFIG);
	fFileMenu->AddEntry("Exit", M_EXIT);
	fFileMenu->Connect("Activated(Int_t)","EVBMainFrame", this, "HandleMenuSelection(Int_t)");
	menuBar->AddPopup("File", fFileMenu, mhints);

	AddFrame(menuBar, new TGLayoutHints(kLHintsTop|kLHintsLeft,0,0,0,0));
	AddFrame(InputFrame, fchints);
	AddFrame(PBFrame, fpbhints);

	SetWindowName("GWM Event Builder");
	MapSubwindows();
	Resize();
	MapWindow();

}

EVBMainFrame::~EVBMainFrame() {
	Cleanup();
	delete this;
}

void EVBMainFrame::CloseWindow() {
	gApplication->Terminate();
}

void EVBMainFrame::HandleMenuSelection(int id) {
	if(id == M_SAVE_CONFIG) new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.25, this, M_SAVE_CONFIG);
	else if(id == M_LOAD_CONFIG) new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.25, this, M_LOAD_CONFIG);
	else if(id == M_EXIT) CloseWindow();
}

void EVBMainFrame::DoOpenWorkdir() {
	new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.25, this, WORKDIR);
}

void EVBMainFrame::DoOpenCMapfile() {
	new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.25, this, CMAP);	
}

void EVBMainFrame::DoOpenSMapfile() {
	new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.25, this, SMAP);	
}

void EVBMainFrame::DoOpenScalerfile() {
	new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.25, this, SCALER);
}

void EVBMainFrame::DoOpenCutfile() {
	new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.25, this, CUT);
}

void EVBMainFrame::DoRun() {

	DisableAllInput();

	SetParameters();

	int type = fTypeBox->GetSelected();
	fBuilder.SetAnalysisType(type);

	switch(type) {
		case GWMEventBuilder::PLOT :
		{
			RunPlot();
			break;
		}
		case GWMEventBuilder::CONVERT :
		{
			fBuilder.Convert2RawRoot();
			break;
		}
		case GWMEventBuilder::MERGE :
		{
			fBuilder.MergeROOTFiles();
			break;
		}
		case GWMEventBuilder::CONVERT_S :
		{
			fBuilder.Convert2SortedRoot();
			break;
		}
		case GWMEventBuilder::CONVERT_F :
		{
			fBuilder.Convert2FastSortedRoot();
			break;
		}
		case GWMEventBuilder::CONVERT_SA :
		{
			fBuilder.Convert2SlowAnalyzedRoot();
			break;
		}
		case GWMEventBuilder::CONVERT_FA :
		{
			fBuilder.Convert2FastAnalyzedRoot();
			break;
		}
	}

	EnableAllInput();
}

void EVBMainFrame::HandleTypeSelection(int box, int entry) {
	fRunButton->SetState(kButtonUp);
}

bool EVBMainFrame::SetParameters() {
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

void EVBMainFrame::DisplayWorkdir(const char* dir) {
	fWorkField->SetText(dir);
	fBuilder.SetWorkDirectory(dir);
}

void EVBMainFrame::DisplayCMap(const char* file) {
	fCMapField->SetText(file);
	fBuilder.SetChannelMap(file);
}

void EVBMainFrame::DisplaySMap(const char* file) {
	fSMapField->SetText(file);
	fBuilder.SetBoardShiftFile(file);
}

void EVBMainFrame::DisplayScaler(const char* file) {
	fScalerField->SetText(file);
	fBuilder.SetScalerFile(file);
}

void EVBMainFrame::DisplayCut(const char* file) {
	fCutField->SetText(file);
	fBuilder.SetCutList(file);
}

void EVBMainFrame::SaveConfig(const char* file) {
	std::string filename = file;
	fBuilder.WriteConfigFile(filename);
}

void EVBMainFrame::LoadConfig(const char* file) {
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

void EVBMainFrame::UpdateWorkdir() {
	const char* dir = fWorkField->GetText();
	fBuilder.SetWorkDirectory(dir);
}

void EVBMainFrame::UpdateSMap() {
	const char* file = fSMapField->GetText();
	fBuilder.SetBoardShiftFile(file);
}

void EVBMainFrame::UpdateCMap() {
	const char* file = fCMapField->GetText();
	fBuilder.SetChannelMap(file);
}

void EVBMainFrame::UpdateScaler() {
	const char* file = fScalerField->GetText();
	fBuilder.SetScalerFile(file);
}

void EVBMainFrame::UpdateCut() {
	const char* file = fCutField->GetText();
	fBuilder.SetCutList(file);
}

void EVBMainFrame::RunPlot() {
	fBuilder.PlotHistograms();
}

void EVBMainFrame::RunMerge(const char* file, const char* dir) {}

void EVBMainFrame::DisableAllInput() {
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

void EVBMainFrame::EnableAllInput() {
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
