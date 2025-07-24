//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// ISRun.hh
//
// Author: Haegin Han and adapted by Sylvain Blunier
// Reference: ICRP Publication 145. Ann. ICRP 49(3), 2020.
// Geant4 Contributors: J. Allison and S. Guatelli
//

#ifndef ISRun_h
#define ISRun_h 1

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

#include "geometry.hh"
#include "generator.hh"

#include "HitInfo.hh"
//#include "ReadCSV.hh"

//typedef std::map<G4int, std::pair<G4double, G4double>> EMAP;
typedef std::map<G4int,G4double> EMAP;
//typedef std::map<G4int,G4int> NP; // Each entry maps ebin -> NZ

// *********************************************************************
// This is G4Run class that sums up energy deposition from each event.
// The sum of the square of energy deposition was also calculated to
// produce the relative error of the dose.
// -- RecordEvent: Sum up the energy deposition and the square of it.
//                 The sums for each organ were saved as the form of
//                 std::map.
// -- Merge: Merge the data calculated in each thread.
// *********************************************************************


class ISRun : public G4Run 
{
public:
 explicit ISRun();
 virtual ~ISRun();

 virtual void RecordEvent(const G4Event*);
 void ConstructMFD(const G4String& mfdName);
 //virtual void Merge(const G4Run*);

 EMAP* GetEDEMap() {return &fEDEMap;};
 EMAP* GetDoseMap() {return &fDoseMap;};

private:
 G4double ISmass;
 
 //std::map<G4int, G4double> wT;
 //std::vector<G4double> GCRParticleWeights;
 //G4double total_flux;
 //std::map<G4int, G4String> organsGrouped;
 //std::map<G4int, G4int> mapGroupedOrgans;
 EMAP fEDEMap, fDoseMap;
 //NP Nparts;

 // Binning of primaries
 G4double minlogE, maxlogE;
 G4int NbinsE;

 G4int id_event,id_entry,n_event_record;//,N_groups;
 G4double mission_factor; 

 //G4int samplesize;

 G4double rsource;
 //const MyPrimaryGenerator *generator;


};
#endif
