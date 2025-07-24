#include "fluxInfo.hh"

fluxInfo::fluxInfo()
{
	kE_down = 0;
	kE_up = 0;
	//ang = 0;
}


fluxInfo::fluxInfo(G4double kE_down_, G4double kE_up_)
{
	kE_down = kE_down_;
	kE_up = kE_up_;
	//ang = ang_;
	//G4cout << kE << " " << particleName << G4endl;
}

// Overload + operator to add two Box objects.
fluxInfo fluxInfo::operator+=(const fluxInfo& b) 
{
         this->kE_down = this->kE_down + b.kE_down;
         this->kE_up = this->kE_up + b.kE_up;
         return *this;
}

/*fluxInfo fluxInfo::operator+=(const G4double& d) 
{
         this->kE_down = this->kE_down + d;
	G4cout << "in operator += " << G4endl;
         return *this;
}*/


fluxInfo::~fluxInfo() {}
