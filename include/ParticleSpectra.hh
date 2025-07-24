#ifndef PARTICLESPECTRA_HH
#define PARTICLESPECTRA_HH

#include "G4AnalysisManager.hh"
#include <map>

// Structure that inform the number of particle of a certain kind produced in after crossing the target.
// It should be associated to one specific input particle and a specific ibin energy
struct ispectra
{
	double bin; //The spectra of outgoing particle 
	std::vector<int> spectra; // Number of particle for each obin 
};


// Class informing the full spectra of a specific outgoing particle type associated with a specific incoming particle
class ParticleSpectra
{
	public:
		ParticleSpectra();
		ParticleSpectra(G4int Nbin_);
		~ParticleSpectra();
		
		void Update(G4int oebin) 
		{
			//G4cout << "Update Particle Spacetra " << oebin << G4endl;
			if (oflux.count(oebin)==0) 
			{
				list_index.push_back(oebin);
				oflux[oebin]=0;
			}
			oflux[oebin]++;
		}
		void print();
		
		// Get value of the energy bin
		G4int GetBin(G4int oebin) {return oflux[oebin];}
		
		std::vector <G4int> GetListIndex() {return list_index;}
		// Get value of the angular bin
		//G4int GetABin(G4int ibin, G4int abin) {return oflux[ibin].angles[abin];}
		
		void SetBin(G4int oebin, G4int N) {oflux[oebin] = N;}
		G4int GetNbin() {return Nbin;}

	private: 
		G4int Nbin;
		std::map <G4int,G4double> oflux; // One spectra for each ibin	
		std::vector <G4int> list_index;
		
};

#endif
