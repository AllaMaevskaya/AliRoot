#ifndef ALIEMCALDIGITIZER_H
#define ALIEMCALDIGITIZER_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//_________________________________________________________________________
// Class that performs digitization of Summable digits from simulated data
//  
// In addition it performs mixing of summable digits from different events.
//
// For each event two branches are created in TreeD:
//   "EMCAL" - list of digits
//   "EMCALTRG" - list of trigger digits
//   "AliEMCALDigitizer" - AliEMCALDigitizer with all parameters used in digitization
//
//*-- Author: Sahal Yacoob (LBL)
// based on : AliPHOSDigit
// July     2003 Yves Schutz : NewIO 
// November 2003 Aleksei Pavlinov : Shish-Kebab geometry 
//
// July 2011 GCB: Digitizer modified to accomodate embedding. 
//                Time calibration added. Decalibration possibility of energy and time added
//_________________________________________________________________________ 


// --- ROOT system ---
class TClonesArray ; 
class TBrowser;

// --- Standard library ---

// --- AliRoot header files ---
#include "AliDigitizer.h"
#include "AliConfig.h"
class AliEMCALCalibData ;
class AliEMCALSDigitizer ;
class AliDigitizationInput ;

class AliEMCALDigitizer: public AliDigitizer {

public:
  AliEMCALDigitizer() ;            // ctor
  AliEMCALDigitizer(TString alirunFileNameFile, 
                    TString eventFolderName = AliConfig::GetDefaultEventFolderName()) ;  
  AliEMCALDigitizer(const AliEMCALDigitizer & dtizer) ;
  AliEMCALDigitizer(AliDigitizationInput * manager) ;
  virtual ~AliEMCALDigitizer() ;       

  void     Digitize(Int_t event);   // Make Digits from SDigits stored in fSDigits
  void     Digitize(Option_t *option);  // Supervising method

  Int_t    GetDigitThreshold()     const { return fDigitThreshold     ; }
  Float_t  GetPinNoise()           const { return fPinNoise           ; }
  Float_t  GetTimeNoise()          const { return fTimeNoise          ; }  
  Float_t  GetTimeResolution(const Float_t energy) const;
  Double_t GetTimeResolutionPar0() const { return fTimeResolutionPar0 ; }
  Double_t GetTimeResolutionPar1() const { return fTimeResolutionPar1 ; }
  Double_t GetTimeDelay()          const { return fTimeDelay          ; }
  Float_t  GetECAchannel()         const { return fADCchannelEC       ; }
  Float_t  GetECApedestal()        const { return fADCpedestalEC      ; }
  
  void     SetEventRange(Int_t first=0, Int_t last=-1) { fFirstEvent     = first ; 
                                                         fLastEvent      = last             ; }
  void     SetDigitThreshold(Int_t EMCThreshold)       { fDigitThreshold = EMCThreshold     ; }
  void     SetPinNoise(Float_t pinNoise )              { fPinNoise       = pinNoise         ; }
  void     SetTimeNoise(Float_t timeNoise )            { fTimeNoise      = timeNoise        ; }

  //General
  Int_t    GetDigitsInRun()  const { return fDigitsInRun; } 
  void     Print (Option_t * option = "") const ;
  void     Print1(Option_t * option)            ; // *MENU*
                   

  AliEMCALDigitizer & operator = (const AliEMCALDigitizer & /*rvalue*/)  {
    // assignement operator requested by coding convention but not needed
   Fatal("operator =", "not implemented") ;  
   return *this ; 
  }

private:

  Bool_t   Init();                   
  void     InitParameters() ; 
  void     PrintDigits(Option_t * option) ;
  void     Unload() ; 
  void     WriteDigits() ;         // Writes Digits the current event
  void     WriteDigits(TClonesArray* digits, const char* branchName = "EMTRG"); //
  Float_t  TimeOfNoise(void) ;     // Calculate time signal generated by noise
  
  void     CalibrateADCTime  (Float_t & adc ,   Float_t & time, const Int_t AbsId) ;
  void     DigitizeEnergyTime(Float_t & energy, Float_t & time, const Int_t AbsId) ;

  void     Digits2FastOR(TClonesArray*digitsTMP, TClonesArray* digitsTRG);
  void     DigitalFastOR(Double_t time, Double_t dE, Int_t timeSamples[], Int_t nSamples);	
	
private:
  
  Bool_t   fDefaultInit;           //! Says if the task was created by defaut ctor (only parameters are initialized)
  Int_t    fDigitsInRun ;          //! Total number of digits in one run
  Bool_t   fInit ;                 //! To avoid overwriting existing files

  Int_t    fInput ;                // Number of files to merge
  TString *fInputFileNames ;       //[fInput] List of file names to merge 
  TString *fEventNames ;           //[fInput] List of event names to merge

  Int_t    fDigitThreshold  ;      // Threshold for storing digits in EMC, ACD units
  Int_t    fMeanPhotonElectron ;   // number of photon electrons per GeV deposited energy 
  Float_t  fGainFluctuations ;     // correct fMeanPhotonElectron by the gain fluctuations
  Float_t  fPinNoise ;             // Electronics noise in EMC, APD
  Double_t fTimeNoise;             // Electronics noise in EMC, time
  Double_t fTimeDelay;             // Time delay to reproduce data delay
  Double_t fTimeResolutionPar0 ;   // Time resolution of FEE electronics
  Double_t fTimeResolutionPar1 ;   // Time resolution of FEE electronics
  Float_t  fADCchannelEC ;         // calibration width of one ADC channel in EC section (GeV)
  Float_t  fADCpedestalEC ;        // calibration pedestal for one ADC channel
  Float_t  fADCchannelECDecal ;    // decalibration width of one ADC channel in EC section (GeV)
  Float_t  fTimeChannel ;          // calibration time width for one channel  
  Float_t  fTimeChannelDecal ;     // calibration time width for one channel  
  Int_t    fNADCEC ;               // number of channels in EC section ADC

  TString  fEventFolderName;       // skowron: name of EFN to read data from in stand alone mode
  Int_t    fFirstEvent;            // first event to process
  Int_t    fLastEvent;             // last  event to process
	
  AliEMCALCalibData * fCalibData;  // Calibration data pointer
  AliEMCALSDigitizer* fSDigitizer; // SDigitization object
  
  ClassDef(AliEMCALDigitizer,13)  
};


#endif // AliEMCALDIGITIZER_H
