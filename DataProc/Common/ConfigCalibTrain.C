/*

 Macro to initialize: 
 - the OCDB (run number required as input argument)
 - the geometry (expected to be in the current directory)
 to run the Calibration train.
 
 Example:
 .L $ALIDPG_ROOT/DataProc/Common/ConfigCalibTrain.C
 ConfigCalibTrain(129160,"raw://");

*/

void ConfigCalibTrain(Int_t run, const char *ocdb="raw://"){

  // OCDB
  printf("setting run to %d\n",run);
  if (gSystem->AccessPathName("OCDB.root", kFileExists)==0) {  
    Printf("ConfigCalibTrain: using OCDB snapshot");
    AliCDBManager::Instance()->SetDefaultStorage("local://");
    AliCDBManager::Instance()->SetRaw(kFALSE);
    AliCDBManager::Instance()->SetSnapshotMode("OCDB.root");
  }
  else {
    Printf("ConfigCalibTrain: NOT using OCDB snapshot");
    AliCDBManager::Instance()->SetDefaultStorage(ocdb);
  }
  AliCDBManager::Instance()->SetRun(run); 
  
  // magnetic field
  if ( !TGeoGlobalMagField::Instance()->GetField() ) {
    printf("Loading field map...\n");
    AliGRPManager grpMan;
    if( !grpMan.ReadGRPEntry() ) { 
      printf("Cannot get GRP entry\n"); 
    }
    if( !grpMan.SetMagField() ) { 
      printf("Problem with magnetic field setup\n"); 
    }
  }

  // geometry
  printf("Loading geometry...\n");
  AliGeomManager::LoadGeometry();
  if( !AliGeomManager::ApplyAlignObjsFromCDB("GRP ITS TPC TRD TOF HMPID") ) {
    printf("Problem with align objects\n"); 
  }

   if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {        
     gROOT->LoadMacro("localOCDBaccessConfig.C");
    gInterpreter->ProcessLine("localOCDBaccessConfig();");
   }
   
}
