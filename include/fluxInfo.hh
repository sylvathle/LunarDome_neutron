#ifndef FLUXINFO_HH
#define FLUXINFO_HH

#include "G4AnalysisManager.hh"

class fluxInfo
{
	public:
		fluxInfo();
		fluxInfo(G4double kE_down_, G4double kE_up_);
		~fluxInfo();
		
		fluxInfo operator+=(const fluxInfo& b);
		//fluxInfo operator+=(const G4double& d);
	
		G4double kE_down ;
		G4double kE_up ;
		//G4double ang ;
};

#endif
