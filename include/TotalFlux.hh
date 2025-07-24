#ifndef TOTALFLUX_HH
#define TOTALFLUX_HH

#include "ParticleSpectra.hh"
//#include "Bins.hh"

// Total outgoing flux associated to one incident particle
// Should also be assiociated to one material and one width of this material

class TotalFlux
{

	public:
		TotalFlux();
		TotalFlux(G4String csvSource);
		~TotalFlux();
		
		void FirstUse();
		bool Update(G4double ikE, G4double okE_down, G4double okE_up);
		void Print();
		//void ToCSV(G4String fileName) const;
		//void IterIncident();


		G4int get_iNbin() {return iNbin;}
		G4int get_oNbin() {return oNbin;}
		std::vector <G4int> GetIndices() {return flux_index;}

		std::map<G4int,ParticleSpectra> GetDownFlux() {return ofluxes_down;}
		std::map<G4int,ParticleSpectra> GetUpFlux() {return ofluxes_up;}
		
		//std::vector <G4double> GetEdges(G4String scale="log");
	
	private:
		G4int iNbin, oNbin;
		G4double ilogemax, ilogemin; //maximum and minimum used energies
		G4double ologemax, ologemin; //maximum and minimum used energies

		std::vector <G4int> flux_index;
		//std::vector<G4double> oangles;
		//int Nbin;
		//G4double logemax, logemin; //maximum and minimum used energies
		//G4int nIncident; // Number of incident particle per ibin
		//Bins bins;
		std::map<G4int,ParticleSpectra> ofluxes_down; //Flux for neutrons going towards the surface
		std::map<G4int,ParticleSpectra> ofluxes_up; //Flux for albedo neutrons


};

#endif
