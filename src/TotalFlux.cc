#include "TotalFlux.hh"

TotalFlux::TotalFlux()
{
	// Initialize data collection
	iNbin = 100; //bins.GetNbins();
	ilogemin = 1; //logMeV //bins.GetMinE(); //1 eV
	ilogemax = 5; //logMeV //bins.GetMaxE(); //100 GeV

	oNbin = 3000; //bins.GetNbins();
	ologemin = -5; //logMeV //bins.GetMinE(); //1 eV
	ologemax = 5; //logMeV //bins.GetMaxE(); //100 GeV

	//nIncident = 0;
}

TotalFlux::TotalFlux(G4String csvSource)
{
	this->FirstUse();
}


TotalFlux::~TotalFlux()
{}

void TotalFlux::FirstUse()
{
	iNbin = 100; //bins.GetNbins();
	ilogemin = 1; //logMeV //bins.GetMinE(); //1 eV
	ilogemax = 5; //logMeV //bins.GetMaxE(); //100 GeV

	// Initialize data collection
	oNbin = 3000;// bins.GetNbins();
	ologemin = -5; //bins.GetMinE(); //100 KeV
	ologemax = 5; //bins.GetMaxE(); //100 GeV
	
	//nIncident = 0;
}

//void TotalFlux::IterIncident(ikE)
//{
//	G4double logprimkE = log10(ikE);
//	G4int iebin = iNbin*(logprimkE-ilogemin)/(ilogemax-ilogemin);
//	nIncident++;
//	fluxes[iebin]
//}


// Update total flux considering energy of primary, secondary and secondary identity
// If energies out of spectra boundaries, abort update and leave function
bool TotalFlux::Update(G4double ikE, G4double okE_down, G4double okE_up)
{

	//G4cout << "start flux update " << G4endl;
	G4double logprimkE = log10(ikE);	
	G4int iebin = iNbin*(logprimkE-ilogemin)/(ilogemax-ilogemin);	
	//G4cout << ikE << " " << okE_down << " " << okE_up << G4endl;
	if (ofluxes_down.count(iebin)==0) 
	{
		ofluxes_down[iebin]=ParticleSpectra();
		ofluxes_up[iebin]=ParticleSpectra();
		flux_index.push_back(iebin);
	}
	
	if (okE_down>0) {
		//G4cout << "okE_down " << okE_down << G4endl;
		G4double logsegkE_down = log10(okE_down);	
		G4int oebin_down = oNbin*(logsegkE_down-ologemin)/(ologemax-ologemin);	
		ofluxes_down[iebin].Update(oebin_down);
	}
	
	if (okE_up>0) {
		//G4cout << "okE_up " << okE_up << G4endl;
		G4double logsegkE_up = log10(okE_up);	
		G4int oebin_up = oNbin*(logsegkE_up-ologemin)/(ologemax-ologemin);	
		ofluxes_up[iebin].Update(oebin_up);
	}

	//G4cout << "end update fluxes" << G4endl;
	
	
	//G4double logsegkE_up = log10(okE_up);
	

	//if (logsegkE_down<ologemin) {return false;}
	//if (logsegkE_up<ologemin) {return false;}
	//if (logprimkE<ilogemin) {return false;}
	

	//G4int oebin_up = oNbin*(logsegkE_up-ologemin)/(ologemax-ologemin);

	//if (oebin<0) return false;
	//if (oebin>=Nbin)
	//{
	//	G4cout << "Warning ibin too high energy particle for the energy range provided, ignoring" << G4endl; 
	//	return false;
	//}

	//G4cout << ikE << " " << okE << G4endl;


	/*if (ofluxes_down.count(iebin)==0) {ofluxes_down[iebin]=ParticleSpectra(oNbin);}
	ofluxes_down[iebin].Update(oebin_down);

	if (ofluxes_up.count(iebin)==0) {ofluxes_up[iebin]=ParticleSpectra(oNbin);}
	ofluxes_up[iebin].Update(oebin_up);*/
	
	//G4cout << ofluxes_down[iebin].GetBin(oebin_down) << " " << ofluxes_up[iebin].GetBin(oebin_up) << G4endl;
	
	return true;
}

/*void TotalFlux::Print()
{

	std::map<G4int, ParticleSpectra>::iterator it;
	for (auto const& x : ofluxes)
	{
		std::cout << x.first << std::endl; 
		ParticleSpectra spe = x.second; // string's value 
		spe.print();
	}
	G4cout << "nIncident " << nIncident << G4endl;
}*/

