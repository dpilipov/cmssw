/*
 * \file DTLocalTriggerTask.cc
 *
 * \author M. Zanetti - INFN Padova
 *
*/

#include "DQM/DTMonitorModule/src/DTLocalTriggerTask.h"

// Framework
#include "FWCore/Framework/interface/EventSetup.h"

// DT trigger
#include "DQM/DTMonitorModule/interface/DTTrigGeomUtils.h"

// Geometry
#include "DataFormats/GeometryVector/interface/Pi.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTTopology.h"

//Root
#include"TH1.h"
#include"TAxis.h"

#include <sstream>
#include <iostream>
#include <fstream>


using namespace edm;
using namespace std;

DTLocalTriggerTask::DTLocalTriggerTask(const edm::ParameterSet& ps) :
  trigGeomUtils(0),
  isLocalRun(ps.getUntrackedParameter<bool>("localrun", true))
 {
  if (!isLocalRun) {
    ltcDigiCollectionToken_ = consumes<LTCDigiCollection>(
        ps.getParameter<edm::InputTag>("ltcDigiCollectionTag"));
  }

  LogTrace("DTDQM|DTMonitorModule|DTLocalTriggerTask") << "[DTLocalTriggerTask]: Constructor"<<endl;

  tpMode           = ps.getUntrackedParameter<bool>("testPulseMode", false);
  detailedAnalysis = ps.getUntrackedParameter<bool>("detailedAnalysis", false);
  doTMTheta       = ps.getUntrackedParameter<bool>("enableTMTheta", false);
  tm_Token_       = consumes<L1MuDTChambPhContainer>(
      edm::InputTag(ps.getUntrackedParameter<string>("tm_label", "dttpgprod")));
// NEW (M.C Fouz July14) Needed, since at least version 710 
  tmTh_Token_       = consumes<L1MuDTChambThContainer>(
      edm::InputTag(ps.getUntrackedParameter<string>("tm_label", "dttpgprod")));
// end NEW

  ros_Token_       = consumes<DTLocalTriggerCollection>(
      edm::InputTag(ps.getUntrackedParameter<string>("ros_label", "dtunpacker")));
  seg_Token_       = consumes<DTRecSegment4DCollection>(
      edm::InputTag(ps.getUntrackedParameter<string>("seg_label", "dt4DSegments")));

  if (tpMode) {
    baseFolderTM = "DT/11-LocalTriggerTP-TM/";
    baseFolderDDU = "DT/12-LocalTriggerTP-DDU/";
  }
  else {
    baseFolderTM = "DT/03-LocalTrigger-TM/";
    baseFolderDDU = "DT/04-LocalTrigger-DDU/";
  }

  parameters = ps;

}


DTLocalTriggerTask::~DTLocalTriggerTask() {

  LogTrace("DTDQM|DTMonitorModule|DTLocalTriggerTask") << "[DTLocalTriggerTask]: analyzed " << nevents << " events" << endl;
  if (trigGeomUtils) { delete trigGeomUtils; }

}

void DTLocalTriggerTask::dqmBeginRun(const edm::Run& run, const edm::EventSetup& context) {

  nevents = 0;
  context.get<MuonGeometryRecord>().get(muonGeom);
  trigGeomUtils = new DTTrigGeomUtils(muonGeom);

}

void DTLocalTriggerTask::bookHistograms(DQMStore::IBooker & ibooker, edm::Run const & iRun, edm::EventSetup const & context) {

  LogTrace("DTDQM|DTMonitorModule|DTLocalTriggerTask") << "[DTLocalTriggerTask]: bookHistograms" << endl;

  if(parameters.getUntrackedParameter<bool>("staticBooking", true)) {  // Static histo booking

    vector<string> trigSources;
    if(parameters.getUntrackedParameter<bool>("localrun", true)) {
      trigSources.push_back("");
    }
    else {
      trigSources.push_back("_DTonly");
      trigSources.push_back("_NoDT");
      trigSources.push_back("_DTalso");
    }
    vector<string>::const_iterator trigSrcIt  = trigSources.begin();
    vector<string>::const_iterator trigSrcEnd = trigSources.end();

    if(parameters.getUntrackedParameter<bool>("process_tm", true)) {
      bookBarrelHistos(ibooker, "TM_ErrorsChamberID");
    }

    if (tpMode) {
      for (int stat=1;stat<5;++stat){
	for (int wh=-2;wh<3;++wh){
	  for (int sect=1;sect<13;++sect){
	    DTChamberId dtChId(wh,stat,sect);

	    if (parameters.getUntrackedParameter<bool>("process_tm", true)){ // TM data
	      bookHistos(ibooker, dtChId,"LocalTriggerPhi","TM_BXvsQual"+(*trigSrcIt));
	      bookHistos(ibooker, dtChId,"LocalTriggerPhi","TM_QualvsPhirad"+(*trigSrcIt));
	    }

	    if (parameters.getUntrackedParameter<bool>("process_ros", true)){ // DDU data
	      bookHistos(ibooker, dtChId,"LocalTriggerPhi","DDU_BXvsQual"+(*trigSrcIt));
	    }

	  }
	}
      } // end of loop
    }
    else {
      for (;trigSrcIt!=trigSrcEnd;++trigSrcIt){
	for (int wh=-2;wh<3;++wh){
	  if (parameters.getUntrackedParameter<bool>("process_tm", true) &&
	      parameters.getUntrackedParameter<bool>("process_ros", true)){ // TM+DDU data
	    bookWheelHistos(ibooker, wh,"COM_BXDiff"+(*trigSrcIt));
	  }
	  for (int sect=1;sect<13;++sect){
	    for (int stat=1;stat<5;++stat){
	      DTChamberId dtChId(wh,stat,sect);
	      if (parameters.getUntrackedParameter<bool>("process_tm", true)){ // TM data

		bookHistos(ibooker, dtChId,"LocalTriggerPhi","TM_BXvsQual"+(*trigSrcIt));

		if (detailedAnalysis) {
		  bookHistos(ibooker, dtChId,"LocalTriggerPhi","TM_QualvsPhirad"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"LocalTriggerPhi","TM_QualvsPhibend"+(*trigSrcIt));
		}
		bookHistos(ibooker, dtChId,"LocalTriggerPhi","TM_Flag1stvsQual"+(*trigSrcIt));
		bookHistos(ibooker, dtChId,"LocalTriggerPhi","TM_BestQual"+(*trigSrcIt));
		if (stat!=4 && doTMTheta){
		  bookHistos(ibooker, dtChId,"LocalTriggerTheta","TM_PositionvsBX"+(*trigSrcIt));
		}

		if (parameters.getUntrackedParameter<bool>("process_seg", true)){ // TM + Segemnt
		  bookHistos(ibooker, dtChId,"Segment","TM_PhitkvsPhitrig"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","TM_PhibtkvsPhibtrig"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","TM_PhiResidual"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","TM_PhiResidualvsLUTPhi"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","TM_PhibResidual"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","TM_HitstkvsQualtrig"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","TM_TrackPosvsAngle"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","TM_TrackPosvsAngleandTrig"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","TM_TrackPosvsAngleandTrigHHHL"+(*trigSrcIt));
		  if(stat!=4){
		    bookHistos(ibooker, dtChId,"Segment","TM_TrackThetaPosvsAngle"+(*trigSrcIt)); // theta view
		    bookHistos(ibooker, dtChId,"Segment","TM_TrackThetaPosvsAngleandTrig"+(*trigSrcIt));
// 		    bookHistos(ibooker, dtChId,"Segment","TM_TrackThetaPosvsAngleandTrigH"+(*trigSrcIt));     // TM theta quality not available!
		  }
		}

	      }

	      if (parameters.getUntrackedParameter<bool>("process_ros", true)){ // DDU data

		bookHistos(ibooker, dtChId,"LocalTriggerPhi","DDU_BXvsQual"+(*trigSrcIt));
		bookHistos(ibooker, dtChId,"LocalTriggerPhi","DDU_Flag1stvsQual"+(*trigSrcIt));
		bookHistos(ibooker, dtChId,"LocalTriggerPhi","DDU_BestQual"+(*trigSrcIt));
		if(stat!=4){                                                    // theta view
		  bookHistos(ibooker, dtChId,"LocalTriggerTheta","DDU_ThetaBXvsQual"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"LocalTriggerTheta","DDU_ThetaBestQual"+(*trigSrcIt));
		}

		if (parameters.getUntrackedParameter<bool>("process_seg", true)){ // DDU + Segment
		  bookHistos(ibooker, dtChId,"Segment","DDU_HitstkvsQualtrig"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","DDU_TrackPosvsAngle"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","DDU_TrackPosvsAngleandTrig"+(*trigSrcIt));
		  bookHistos(ibooker, dtChId,"Segment","DDU_TrackPosvsAngleandTrigHHHL"+(*trigSrcIt));
		  if(stat!=4){
		    bookHistos(ibooker, dtChId,"Segment","DDU_TrackThetaPosvsAngle"+(*trigSrcIt)); // theta view
		    bookHistos(ibooker, dtChId,"Segment","DDU_TrackThetaPosvsAngleandTrig"+(*trigSrcIt));
		    bookHistos(ibooker, dtChId,"Segment","DDU_TrackThetaPosvsAngleandTrigH"+(*trigSrcIt));
		  }
		}

	      }

	      if (parameters.getUntrackedParameter<bool>("process_tm", true) &&
		  parameters.getUntrackedParameter<bool>("process_ros", true)){ // TM+DDU data
		bookHistos(ibooker, dtChId,"LocalTriggerPhi","COM_QualDDUvsQualTM"+(*trigSrcIt));
	      }

	    }
	  }
	  for (int sect=13;sect<15;++sect){
	    DTChamberId dtChId(wh,4,sect);
	    if (parameters.getUntrackedParameter<bool>("process_tm", true) &&
		parameters.getUntrackedParameter<bool>("process_seg", true)){ // TM+SEG LUTs data
	      bookHistos(ibooker, dtChId,"Segment","TM_PhitkvsPhitrig"+(*trigSrcIt));
	      bookHistos(ibooker, dtChId,"Segment","TM_PhibtkvsPhibtrig"+(*trigSrcIt));
	      bookHistos(ibooker, dtChId,"Segment","TM_PhiResidual"+(*trigSrcIt));
	      bookHistos(ibooker, dtChId,"Segment","TM_PhiResidualvsLUTPhi"+(*trigSrcIt));
	      bookHistos(ibooker, dtChId,"Segment","TM_PhibResidual"+(*trigSrcIt));
	    }
	  }
	}
      }// end of loop
    }

  }


}


void DTLocalTriggerTask::beginLuminosityBlock(const LuminosityBlock& lumiSeg, const EventSetup& context) {

  LogTrace("DTDQM|DTMonitorModule|DTLocalTriggerTask") << "[DTLocalTriggerTask]: Begin of LS transition" << endl;

  if(lumiSeg.id().luminosityBlock()%parameters.getUntrackedParameter<int>("ResetCycle", 3) == 0) {
    for(map<uint32_t, map<string, MonitorElement*> > ::const_iterator histo = digiHistos.begin();
	histo != digiHistos.end();
	histo++) {
      for(map<string, MonitorElement*> ::const_iterator ht = (*histo).second.begin();
	  ht != (*histo).second.end();
	  ht++) {
	(*ht).second->Reset();
      }
    }
  }

}

void DTLocalTriggerTask::analyze(const edm::Event& e, const edm::EventSetup& c){
  if (!nevents){

    edm::Handle<L1MuDTChambPhContainer> l1DTTPGPh;
    e.getByToken(tm_Token_, l1DTTPGPh);
    edm::Handle<L1MuDTChambThContainer> l1DTTPGTh;
    // e.getByToken(tm_Token_, l1DTTPGTh);// CHANGED (M.C Fouz July14) Needed, since at least version 710
    e.getByToken(tmTh_Token_, l1DTTPGTh); // CHANGED (F.R.Cavallo Nov14)

    useTM = (l1DTTPGPh.isValid() || l1DTTPGTh.isValid()) && parameters.getUntrackedParameter<bool>("process_tm", true) ;

    Handle<DTLocalTriggerCollection> l1DDUTrigs;
    e.getByToken(ros_Token_,l1DDUTrigs);
    useDDU = l1DDUTrigs.isValid() && parameters.getUntrackedParameter<bool>("process_ros", true) ;

    Handle<DTRecSegment4DCollection> all4DSegments;
    e.getByToken(seg_Token_, all4DSegments);
    useSEG = all4DSegments.isValid() && parameters.getUntrackedParameter<bool>("process_seg", true) ;

  }

  nevents++;

  triggerSource(e);

  if ( useTM ) {
    edm::Handle<L1MuDTChambPhContainer> l1DTTPGPh;
    e.getByToken(tm_Token_, l1DTTPGPh);
    vector<L1MuDTChambPhDigi> const*  l1PhTrig = l1DTTPGPh->getContainer();

    edm::Handle<L1MuDTChambThContainer> l1DTTPGTh;
    //e.getByToken(tm_Token_, l1DTTPGTh);// CHANGED (M.C Fouz July14) Needed, since at least version 710
    e.getByToken(tmTh_Token_, l1DTTPGTh);// CHANGED (F.R. Cavallo Nov14)

    vector<L1MuDTChambThDigi> const*  l1ThTrig = l1DTTPGTh->getContainer();

    runTMAnalysis(l1PhTrig, l1ThTrig);
  }
  if ( useDDU ) {
    Handle<DTLocalTriggerCollection> l1DDUTrigs;
    e.getByToken(ros_Token_, l1DDUTrigs);

    runDDUAnalysis(l1DDUTrigs);
  }
  if ( !tpMode && useSEG ) {
    Handle<DTRecSegment4DCollection> segments4D;
    e.getByToken(seg_Token_, segments4D);

    runSegmentAnalysis(segments4D);
  }
  if ( !tpMode && useTM && useDDU ) {
    runDDUvsTMAnalysis(trigsrc);
  }

}


void DTLocalTriggerTask::bookBarrelHistos(DQMStore::IBooker & ibooker, string histoTag) {

  bool isTM = histoTag.substr(0,2) == "TM";
  ibooker.setCurrentFolder(topFolder(isTM));
  if (histoTag == "TM_ErrorsChamberID") {
    tm_IDDataErrorPlot = ibooker.book1D(histoTag.c_str(),"TM Data ID Error",5,-2,3);
    tm_IDDataErrorPlot->setAxisTitle("wheel",1);
  }

  return;

}

void DTLocalTriggerTask::bookHistos(DQMStore::IBooker & ibooker, const DTChamberId& dtCh, string folder, string histoTag) {

  int wh=dtCh.wheel();
  int sc=dtCh.sector();
  stringstream wheel; wheel << wh;
  stringstream station; station << dtCh.station();
  stringstream sector; sector << sc;

  double minBX=0;
  double maxBX=0;
  int  rangeBX=0;

  bool isTM = histoTag.substr(0,2) == "TM";
  string histoType = histoTag.substr(4,histoTag.find("_",4)-4); //DDU case
  if(isTM)   histoType = histoTag.substr(3,histoTag.find("_",3)-3);
 
  ibooker.setCurrentFolder(topFolder(isTM) + "Wheel" + wheel.str() +
			"/Sector" + sector.str() +
			"/Station" + station.str() + "/" + folder);

  string histoName = histoTag + "_W" + wheel.str() + "_Sec" + sector.str() + "_St" + station.str();

  LogTrace("DTDQM|DTMonitorModule|DTLocalTriggerTask") << "[DTLocalTriggerTask]: booking " << topFolder(isTM) << "Wheel" << wheel.str()
						       << "/Sector" << sector.str()
						       << "/Station"<< station.str() << "/" << folder << "/" << histoName << endl;

  if (histoType.find("BX") != string::npos){
    if (histoTag.substr(0,2) == "TM"){
      minBX= parameters.getUntrackedParameter<int>("minBXTM",-2) - 0.5;
      maxBX= parameters.getUntrackedParameter<int>("maxBXTM",2) + 0.5;
    }
    else {
      minBX= parameters.getUntrackedParameter<int>("minBXDDU",0) - 0.5;
      maxBX= parameters.getUntrackedParameter<int>("maxBXDDU",20) + 0.5;
    }
    rangeBX = (int)(maxBX-minBX);
  }

  if ( folder == "LocalTriggerPhi") {

    if( histoType == "BXvsQual" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"BX vs trigger quality",7,-0.5,6.5,rangeBX,minBX,maxBX);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],1);
      return ;
    }
    if( histoType == "BestQual" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book1D(histoName,"Trigger quality of best primitives",7,-0.5,6.5);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],1);
      return ;
    }
    if( histoType == "QualvsPhirad" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"Trigger quality vs local position",100,-500.,500.,7,-0.5,6.5);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],2);
      return ;
    }
    if( histoType == "QualvsPhibend" ) {
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"Trigger quality vs local direction",200,-40.,40.,7,-0.5,6.5);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],2);
      return ;
    }
    if( histoType == "Flag1stvsQual" ) {
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"1st/2nd trig flag vs quality",7,-0.5,6.5,2,-0.5,1.5);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],1);
      return ;
    }
    if( histoType == "QualDDUvsQualTM" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"DDU quality vs TM quality",8,-1.5,6.5,8,-1.5,6.5);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],1);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],2);
      return ;
    }

  }
  else if ( folder == "LocalTriggerTheta")   {

    if( histoType == "PositionvsBX" ) {
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"Theta trigger position vs BX",rangeBX,minBX,maxBX,7,-0.5,6.5);
      return ;
    }
    if( histoType == "PositionvsQual" ) {
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"Theta trigger position vs quality",7,-0.5,6.5,7,-0.5,6.5);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],1);
      return ;
    }
    if( histoType == "ThetaBXvsQual" ) {
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"BX vs trigger quality",7,-0.5,6.5,rangeBX,minBX,maxBX);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],1);
    }
    if( histoType == "ThetaBestQual" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book1D(histoName,"Trigger quality of best primitives (theta)",7,-0.5,6.5);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],1);
      return ;
    }

  }
  else if ( folder == "Segment")   {

    if( histoType.find("TrackThetaPosvsAngle" ) == 0 ) {

      string histoLabel = "Position vs Angle (theta)";
      if (histoType.find("andTrigH") != string::npos) histoLabel += " for H triggers";
      else if (histoType.find("andTrig") != string::npos) histoLabel += " for triggers";

      float min,max;
      int nbins;
      trigGeomUtils->thetaRange(dtCh,min,max,nbins);
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,histoLabel,16,-40.,40.,nbins,min,max);
      return ;
    }
    if( histoType.find("TrackPosvsAngle") == 0 ){

      float min,max;
      int nbins;
      trigGeomUtils->phiRange(dtCh,min,max,nbins);

      string histoLabel = "Position vs Angle (phi)";
      if (histoType.find("andTrigHHHL")  != string::npos) histoLabel += " for HH/HL triggers";
      else if (histoType.find("andTrig") != string::npos) histoLabel += " for triggers";

      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,histoLabel,16,-40.,40.,nbins,min,max);
      return ;
    }
    if( histoType == "PhitkvsPhitrig" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"Local position: segment vs trigger",100,-500.,500.,100,-500.,500.);
      return ;
    }
    if( histoType == "PhibtkvsPhibtrig" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"Local direction : segment vs trigger",200,-40.,40.,200,-40.,40.);
      return ;
    }
    if( histoType == "PhiResidual" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book1D(histoName,"Trigger local position - Segment local position (correlated triggers)",400,-10.,10.);
      return ;
    }
    if( histoType == "PhibResidual" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book1D(histoName,"Trigger local direction - Segment local direction (correlated triggers)",500,-10.,10.);
      return ;
    }
    if( histoType == "HitstkvsQualtrig" ){
      (digiHistos[dtCh.rawId()])[histoTag] =
	ibooker.book2D(histoName,"Segment hits (phi) vs trigger quality",7,-0.5,6.5,10,0.5,10.5);
      setQLabels((digiHistos[dtCh.rawId()])[histoTag],1);
      return ;
    }

  }

}

void DTLocalTriggerTask::bookWheelHistos(DQMStore::IBooker & ibooker, int wh, string histoTag) {

  stringstream wheel; wheel << wh;
  
  bool isTM = histoTag.substr(0,2) == "TM";
  string histoType = histoTag.substr(4,histoTag.find("_",4)-4); //DDU case
  if(isTM)  histoType = histoTag.substr(3,histoTag.find("_",3)-3);

  ibooker.setCurrentFolder(topFolder(isTM) + "Wheel" + wheel.str() + "/");

  string histoName = histoTag + "_W" + wheel.str();

  LogTrace("DTDQM|DTMonitorModule|DTLocalTriggerTask") << "[DTLocalTriggerTask]: booking " << topFolder(isTM)
						       << "Wheel" << wheel.str() << "/" << histoName << endl;

  if( histoType.find("BXDiff") != string::npos ){
    MonitorElement *me = ibooker.bookProfile2D(histoName,"DDU-TM BX Difference",12,1,13,4,1,5,0.,20.);
    me->setAxisTitle("Sector",1);
    me->setAxisTitle("station",2);
    (wheelHistos[wh])[histoTag] = me;
    return ;
  }

}

void DTLocalTriggerTask::runTMAnalysis(std::vector<L1MuDTChambPhDigi> const* phTrigs,
					 std::vector<L1MuDTChambThDigi> const* thTrigs ) {

  string histoType ;
  string histoTag ;

  // define best quality trigger segment (phi and theta)
  // in any station start from 1 and zero is kept empty
  for (int i=0;i<5;++i){
    for (int j=0;j<6;++j){
      for (int k=0;k<13;++k){
	phcode_best[j][i][k] = -1;
	thcode_best[j][i][k] = -1;
      }
    }
  }

  vector<L1MuDTChambPhDigi>::const_iterator iph  = phTrigs->begin();
  vector<L1MuDTChambPhDigi>::const_iterator iphe = phTrigs->end();
  for(; iph !=iphe ; ++iph) {

    int phwheel = iph->whNum();
    int phsec   = iph->scNum() + 1; // SM The track finder goes from 0 to 11. I need them from 1 to 12 !!!!!
    int phst    = iph->stNum();
    int phbx    = iph->bxNum();
    int phcode  = iph->code();
    int phi1st  = iph->Ts2Tag();
    // FIXME: workaround for TM data with station ID
    if(phst == 0) {
      tm_IDDataErrorPlot->Fill(phwheel);
      continue;
    }

    if(phcode>phcode_best[phwheel+3][phst][phsec] && phcode<7) {
      phcode_best[phwheel+3][phst][phsec]=phcode;
      iphbest[phwheel+3][phst][phsec] = &(*iph);
    }

    DTChamberId dtChId(phwheel,phst,phsec);

    float x     = trigGeomUtils->trigPos(&(*iph));
    float angle = trigGeomUtils->trigDir(&(*iph));
    uint32_t indexCh = dtChId.rawId();

    map<string, MonitorElement*> &innerME = digiHistos[indexCh];

    if (tpMode) {
      innerME.find("TM_BXvsQual"+trigsrc)->second->Fill(phcode,phbx-phi1st);    // SM BX vs Qual Phi view (1st tracks)
      innerME.find("TM_QualvsPhirad"+trigsrc)->second->Fill(x,phcode);          // SM Qual vs radial angle Phi view
    }
    else {
      innerME.find("TM_BXvsQual"+trigsrc)->second->Fill(phcode,phbx-phi1st);    // SM BX vs Qual Phi view (1st tracks)
      innerME.find("TM_Flag1stvsQual"+trigsrc)->second->Fill(phcode,phi1st);    // SM Qual 1st/2nd track flag Phi view
      if (detailedAnalysis) {
	innerME.find("TM_QualvsPhirad"+trigsrc)->second->Fill(x,phcode);          // SM Qual vs radial angle Phi view
	innerME.find("TM_QualvsPhibend"+trigsrc)->second->Fill(angle,phcode);     // SM Qual vs bending Phi view
      }
    }

  }

  if (doTMTheta) {
    int thcode[7];
    vector<L1MuDTChambThDigi>::const_iterator ith  = thTrigs->begin();
    vector<L1MuDTChambThDigi>::const_iterator ithe = thTrigs->end();
    for(; ith != ithe; ++ith) {
      int thwheel = ith->whNum();
      int thsec   = ith->scNum() + 1; // SM The track finder goes from 0 to 11. I need them from 1 to 12 !!!!!
      int thst    = ith->stNum();
      int thbx    = ith->bxNum();

      for (int pos=0; pos<7; pos++) {
	thcode[pos] = ith->code(pos);

	if(thcode[pos]>thcode_best[thwheel+3][thst][thsec] ) {
	  thcode_best[thwheel+3][thst][thsec]=thcode[pos];
	  ithbest[thwheel+3][thst][thsec] = &(*ith);
	}
      }

      DTChamberId dtChId(thwheel,thst,thsec);
      uint32_t indexCh = dtChId.rawId();

      map<string, MonitorElement*> &innerME = digiHistos[indexCh];

      for (int pos=0; pos<7; pos++) { //SM fill position for non zero position bit in theta view
	if(thcode[pos]>0){
	  innerME.find("TM_PositionvsBX"+trigsrc)->second->Fill(thbx,pos);          // SM BX vs Position Theta view
	}
      }
    }
  }


  // Fill Quality plots with best TM triggers in phi & theta
  if (!tpMode) {
    for (int st=1;st<5;++st){
      for (int wh=-2;wh<3;++wh){
	for (int sc=1;sc<13;++sc){
	  if (phcode_best[wh+3][st][sc]>-1 && phcode_best[wh+3][st][sc]<7){
	    DTChamberId id(wh,st,sc);
	    uint32_t indexCh = id.rawId();
	    map<string, MonitorElement*> &innerME = digiHistos[indexCh];
	    innerME.find("TM_BestQual"+trigsrc)->second->Fill(phcode_best[wh+3][st][sc]);  // Best Qual Trigger Phi view
	  }
	}
      }
    }
  }

}

void DTLocalTriggerTask::runDDUAnalysis(Handle<DTLocalTriggerCollection>& trigsDDU){

  DTLocalTriggerCollection::DigiRangeIterator detUnitIt;

  for (int i=0;i<5;++i){
    for (int j=0;j<6;++j){
      for (int k=0;k<13;++k){
	dduphcode_best[j][i][k] = -1;
	dduthcode_best[j][i][k] = -1;
      }
    }
  }

  for (detUnitIt=trigsDDU->begin();
       detUnitIt!=trigsDDU->end();
       ++detUnitIt){

    const DTChamberId& id = (*detUnitIt).first;
    const DTLocalTriggerCollection::Range& range = (*detUnitIt).second;
    uint32_t indexCh = id.rawId();
    map<string, MonitorElement*> &innerME = digiHistos[indexCh];

    int wh = id.wheel();
    int sec = id.sector();
    int st = id.station();

    for (DTLocalTriggerCollection::const_iterator trigIt = range.first;
	 trigIt!=range.second;
	 ++trigIt){

      int bx = trigIt->bx();
      int quality = trigIt->quality();
      int thqual = trigIt->trTheta();
      int flag1st = trigIt->secondTrack() ? 1 : 0;

      // check if SC data exist: fill for any trigger
      if( quality>-1 && quality<7 ) {	  // it is a phi trigger

	if(quality>dduphcode_best[wh+3][st][sec]) {
	  dduphcode_best[wh+3][st][sec]=quality;
	  iphbestddu[wh+3][st][sec] = &(*trigIt);
	}

	if(tpMode) {
	  innerME.find("DDU_BXvsQual"+trigsrc)->second->Fill(quality,bx-flag1st);     // SM BX vs Qual Phi view
	}
	else {
	  innerME.find("DDU_BXvsQual"+trigsrc)->second->Fill(quality,bx-flag1st);     // SM BX vs Qual Phi view
	  innerME.find("DDU_Flag1stvsQual"+trigsrc)->second->Fill(quality,flag1st); // SM Quality vs 1st/2nd track flag Phi view
	}
      }
      if( thqual>0 && !tpMode ) {  // it is a theta trigger

	if(thqual>dduthcode_best[wh+3][st][sec] ) {
	  dduthcode_best[wh+3][st][sec]=thqual;
	}

	innerME.find("DDU_ThetaBXvsQual"+trigsrc)->second->Fill(thqual,bx);     // SM BX vs Qual Theta view
      }
    }

    // Fill Quality plots with best ddu triggers in phi & theta
    if (!tpMode) {
      if (dduphcode_best[wh+3][st][sec]>-1 &&
	  dduphcode_best[wh+3][st][sec]<7){

	innerME.find("DDU_BestQual"+trigsrc)->second->Fill(dduphcode_best[wh+3][st][sec]);  // Best Qual Trigger Phi view
      }
      if (dduthcode_best[wh+3][st][sec]>0){
	innerME.find("DDU_ThetaBestQual"+trigsrc)->second->Fill(dduthcode_best[wh+3][st][sec]); // Best Qual Trigger Theta view
      }
    }
  }
}


void DTLocalTriggerTask::runSegmentAnalysis(Handle<DTRecSegment4DCollection>& segments4D){

  DTRecSegment4DCollection::const_iterator track;

  // Find best tracks & good tracks
  memset(track_ok,false,450*sizeof(bool));

  DTRecSegment4DCollection::id_iterator chamberId;
  vector<const DTRecSegment4D*> best4DSegments;

  // Preliminary loop finds best 4D Segment and high quality ones
  for (chamberId = segments4D->id_begin(); chamberId != segments4D->id_end(); ++chamberId){

    DTRecSegment4DCollection::range  range = segments4D->get(*chamberId);
    const DTRecSegment4D* tmpBest=0;
    int tmpdof = 0;
    int dof = 0;

    for ( track = range.first; track != range.second; ++track){

      if( (*track).hasPhi() ) {

	dof = (*track).phiSegment()->degreesOfFreedom();
	if ( dof>tmpdof ){
	  tmpBest = &(*track);
	  tmpdof = dof;

	  int wheel = (*track).chamberId().wheel();
	  int sector = (*track).chamberId().sector();
	  int station = (*track).chamberId().station();
	  if (sector==13){
	    sector=4;
	  }
	  else if (sector==14){
	    sector=10;
	  }
	  track_ok[wheel+3][station][sector] = (!track_ok[wheel+3][station][sector] && dof>=2);
	}

      }
    }
    if (tmpBest) best4DSegments.push_back(tmpBest);
  }

  vector<const DTRecSegment4D*>::const_iterator btrack;

  for ( btrack = best4DSegments.begin(); btrack != best4DSegments.end(); ++btrack ){

    if( (*btrack)->hasPhi() ) { // Phi component

      int wheel    = (*btrack)->chamberId().wheel();
      int station  = (*btrack)->chamberId().station();
      int sector   = (*btrack)->chamberId().sector();
      int scsector = 0;
      float x_track, y_track, x_angle, y_angle;
      trigGeomUtils->computeSCCoordinates((*btrack),scsector,x_track,x_angle,y_track,y_angle);
      int nHitsPhi = (*btrack)->phiSegment()->degreesOfFreedom()+2;

      DTChamberId dtChId(wheel,station,sector);      // get chamber for LUTs histograms (Sectors 1 to 14)
      uint32_t indexCh = dtChId.rawId();
      map<string, MonitorElement*> &innerMECh = digiHistos[indexCh];

      DTChamberId dtChIdSC = DTChamberId(wheel,station,scsector);  // get chamber for histograms SC granularity (sectors 1 to 12)
      indexCh = dtChIdSC.rawId();
      map<string, MonitorElement*> &innerME = digiHistos[indexCh];

      if (useDDU &&
	  dduphcode_best[wheel+3][station][scsector] > -1 &&
	  dduphcode_best[wheel+3][station][scsector] < 7 ) {

	// SM hits of the track vs quality of the trigger
	innerME.find("DDU_HitstkvsQualtrig"+trigsrc)->second->Fill(dduphcode_best[wheel+3][station][scsector],nHitsPhi);

      }

      if (useTM &&
	  phcode_best[wheel+3][station][scsector] > -1 &&
	  phcode_best[wheel+3][station][scsector] < 7 ) {

	innerME.find("TM_HitstkvsQualtrig"+trigsrc)->second->Fill(phcode_best[wheel+3][station][scsector],nHitsPhi);

	if (phcode_best[wheel+3][station][scsector]>3 && nHitsPhi>=7){

	  float x_trigger     = trigGeomUtils->trigPos(iphbest[wheel+3][station][scsector]);
	  float angle_trigger = trigGeomUtils->trigDir(iphbest[wheel+3][station][scsector]);
	  trigGeomUtils->trigToSeg(station,x_trigger,x_angle);

	  innerMECh.find("TM_PhitkvsPhitrig"+trigsrc)->second->Fill(x_trigger,x_track);
	  innerMECh.find("TM_PhibtkvsPhibtrig"+trigsrc)->second->Fill(angle_trigger,x_angle);
	  innerMECh.find("TM_PhiResidual"+trigsrc)->second->Fill(x_trigger-x_track);
	  innerMECh.find("TM_PhibResidual"+trigsrc)->second->Fill(angle_trigger-x_angle);
	}
      }


      if (useTM) {

	// check for triggers elsewhere in the sector
	bool trigFlagTM =false;
	for (int ist=1; ist<5; ist++){
	  if (ist!=station &&
	      phcode_best[wheel+3][ist][scsector]>=2 &&
	      phcode_best[wheel+3][ist][scsector]<7 &&
	      track_ok[wheel+3][ist][scsector]==true){
	    trigFlagTM = true;
	    break;
	  }
	}

	if (trigFlagTM && fabs(x_angle)<40. && nHitsPhi>=7){

	  // position vs angle of track for reconstruced tracks (denom. for trigger efficiency)
	  innerME.find("TM_TrackPosvsAngle"+trigsrc)->second->Fill(x_angle,x_track);
	  if (phcode_best[wheel+3][station][scsector] >= 2 && phcode_best[wheel+3][station][scsector] < 7) {
	    innerME.find("TM_TrackPosvsAngleandTrig"+trigsrc)->second->Fill(x_angle,x_track);
	    if (phcode_best[wheel+3][station][scsector] > 4){  //HH & HL Triggers
	      innerME.find("TM_TrackPosvsAngleandTrigHHHL"+trigsrc)->second->Fill(x_angle,x_track);
	    }
	  }

	}

	if ((*btrack)->hasZed() && trigFlagTM && fabs(y_angle)<40. && (*btrack)->zSegment()->degreesOfFreedom()>=1){

	  // position va angle of track for reconstruced tracks (denom. for trigger efficiency) along theta direction
	  innerME.find("TM_TrackThetaPosvsAngle"+trigsrc)->second->Fill(y_angle,y_track);
	  if (thcode_best[wheel+3][station][scsector] > 0) {
	    innerME.find("TM_TrackThetaPosvsAngleandTrig"+trigsrc)->second->Fill(y_angle,y_track);
	  }

	}
      }

      if (useDDU) {

	// check for triggers elsewhere in the sector
	bool trigFlagDDU =false;
	for (int ist=1; ist<5; ist++){
	  if (ist!=station &&
	      dduphcode_best[wheel+3][ist][scsector]>=2 &&
	      dduphcode_best[wheel+3][ist][scsector]<7 &&
	      track_ok[wheel+3][ist][scsector]==true){
	    trigFlagDDU = true;
	    break;
	  }
	}

	if (trigFlagDDU && fabs(x_angle)<40. && nHitsPhi>=7){

	  // position vs angle of track for reconstruced tracks (denom. for trigger efficiency)
	  innerME.find("DDU_TrackPosvsAngle"+trigsrc)->second->Fill(x_angle,x_track);
	  if (dduphcode_best[wheel+3][station][scsector] >= 2 && dduphcode_best[wheel+3][station][scsector] < 7) {
	  innerME.find("DDU_TrackPosvsAngleandTrig"+trigsrc)->second->Fill(x_angle,x_track);
	    if (dduphcode_best[wheel+3][station][scsector] > 4){  //HH & HL Triggers
	      innerME.find("DDU_TrackPosvsAngleandTrigHHHL"+trigsrc)->second->Fill(x_angle,x_track);
	    }
	  }

	}

	if ((*btrack)->hasZed() && trigFlagDDU && fabs(y_angle)<40. && (*btrack)->zSegment()->degreesOfFreedom()>=1){

	  // position va angle of track for reconstruced tracks (denom. for trigger efficiency) along theta direction
	  innerME.find("DDU_TrackThetaPosvsAngle"+trigsrc)->second->Fill(y_angle,y_track);
	  if (dduthcode_best[wheel+3][station][scsector] > 0) {
	    innerME.find("DDU_TrackThetaPosvsAngleandTrig"+trigsrc)->second->Fill(y_angle,y_track);
	    if (dduthcode_best[wheel+3][station][scsector] == 3) {
	      innerME.find("DDU_TrackThetaPosvsAngleandTrigH"+trigsrc)->second->Fill(y_angle,y_track);
	    }
	  }

	}
      }
    }
  }

}


void DTLocalTriggerTask::runDDUvsTMAnalysis(string& trigsrc){

  string histoType ;
  string histoTag ;

  for (int st=1;st<5;++st){
    for (int wh=-2;wh<3;++wh){
      for (int sc=1;sc<13;++sc){
	if ( (phcode_best[wh+3][st][sc]>-1 && phcode_best[wh+3][st][sc]<7) ||
	     (dduphcode_best[wh+3][st][sc]>-1 && dduphcode_best[wh+3][st][sc]<7) ){
	  DTChamberId id(wh,st,sc);
	  uint32_t indexCh = id.rawId();
	  map<string, MonitorElement*> &innerME = digiHistos[indexCh];

	  innerME.find("COM_QualDDUvsQualTM"+trigsrc)->second->Fill(phcode_best[wh+3][st][sc],dduphcode_best[wh+3][st][sc]);
	  if ( (phcode_best[wh+3][st][sc]>-1 && phcode_best[wh+3][st][sc]<7) &&
	       (dduphcode_best[wh+3][st][sc]>-1 && dduphcode_best[wh+3][st][sc]<7) ){
	    int bxDDU = iphbestddu[wh+3][st][sc]->bx() - iphbestddu[wh+3][st][sc]->secondTrack();
	    int bxTM = iphbest[wh+3][st][sc]->bxNum() - iphbest[wh+3][st][sc]->Ts2Tag();
	    (wheelHistos[wh]).find("COM_BXDiff"+trigsrc)->second->Fill(sc,st,bxDDU-bxTM);
	  }
	}
      }
    }
  }

}

void DTLocalTriggerTask::setQLabels(MonitorElement* me, short int iaxis){

  TH1* histo = me->getTH1();
  if (!histo) return;

  TAxis* axis=0;
  if (iaxis==1) {
    axis=histo->GetXaxis();
  }
  else if(iaxis==2) {
    axis=histo->GetYaxis();
  }
  if (!axis) return;

  string labels[7] = {"LI","LO","HI","HO","LL","HL","HH"};
  int istart = axis->GetXmin()<-1 ? 2 : 1;
  for (int i=0;i<7;i++) {
    axis->SetBinLabel(i+istart,labels[i].c_str());
  }

}

void DTLocalTriggerTask::triggerSource(const edm::Event& e) {


  if (!isLocalRun){

    Handle<LTCDigiCollection> ltcdigis;
    e.getByToken(ltcDigiCollectionToken_, ltcdigis);

    for (std::vector<LTCDigi>::const_iterator ltc_it = ltcdigis->begin(); ltc_it != ltcdigis->end(); ltc_it++){

      size_t otherTriggerSum=0;
      for (size_t i = 1; i < 6; i++) {
	otherTriggerSum += size_t((*ltc_it).HasTriggered(i));
      }
      if ((*ltc_it).HasTriggered(0) && otherTriggerSum == 0)
	trigsrc = "_DTonly";
      else if (!(*ltc_it).HasTriggered(0))
	trigsrc = "_NoDT";
      else if ((*ltc_it).HasTriggered(0) && otherTriggerSum > 0)
	trigsrc = "_DTalso";

    }
    return;
  }

  trigsrc = "";
  return;

}

// Local Variables:
// show-trailing-whitespace: t
// truncate-lines: t
// End:
