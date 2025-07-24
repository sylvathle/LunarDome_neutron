#include "run.hh"
//#include "ions.hh"

MyRunAction::MyRunAction()//: fNumOfEvent(0), fRunID(0)
{
	
	//organsGrouped = GetOrgansGroup("organsInfo.csv");
	//std::map<G4int, G4int> mapGroupedOrgans = generateNumberedMap(organsGrouped);
	
	std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* timeInfo = std::localtime(&currentTime);
	std::stringstream ss;
	ss << std::put_time(timeInfo, "%Y%m%d-%H%M%S");
        labelCSV = G4String(ss.str());

        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

        G4String rd_label = G4String(std::to_string(nanoseconds.count()%100000000));
        labelCSV = labelCSV + G4String("-") + rd_label;

	DefineCommands();
}

MyRunAction::~MyRunAction()
{
	delete fMessenger;
}


G4Run* MyRunAction::GenerateRun()
{
	const MyGeometry *detectorConstruction = static_cast<const MyGeometry*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	
	phantomType = detectorConstruction->GetPhantomType();
	iRun = new ISRun();
	return iRun;
}


void MyRunAction::UpdateFlux(G4double ikE, G4double okE_down, G4double okE_up)
{
	flux.Update(ikE,okE_down,okE_up);
}

// Prepare arrays to store the data
void MyRunAction::BeginOfRunAction(const G4Run* aRun)
{
	auto man = G4AnalysisManager::Instance();
	
	const MyGeometry *detectorConstruction = static_cast<const MyGeometry*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

        G4String rd_label = G4String(std::to_string(CLHEP::RandFlat::shootInt(1e9)));
	labelCSV = labelCSV + G4String("-") + rd_label;
	
	man->CreateNtuple("Doses","Doses");
	man->CreateNtupleIColumn(G4String("eBin"));
	man->CreateNtupleDColumn(G4String("EDE"));
	man->CreateNtupleDColumn(G4String("Dose"));
	man->FinishNtuple(0);


	man->CreateNtuple("N","N");
	man->CreateNtupleIColumn("ikE");
	man->CreateNtupleIColumn("N");
	man->FinishNtuple(1);
	

	man->CreateNtuple("InnerFlux","InnerFlux");
	man->CreateNtupleIColumn("ikE");
	man->CreateNtupleIColumn("okE");
	man->CreateNtupleIColumn("down");
	man->CreateNtupleIColumn("up");
	man->FinishNtuple(2);


}

void MyRunAction::EndOfRunAction(const G4Run* aRun)
{

	const MyPrimaryGenerator *generator = static_cast<const MyPrimaryGenerator*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
	
	auto man = G4AnalysisManager::Instance();

	for (int i=0;i<100;i++)
	{
		man->FillNtupleIColumn(1,0,i);
		man->FillNtupleIColumn(1,1,generator->GetNGenerated(i));
		man->AddNtupleRow(1);
	}

	
	auto downFlux = flux.GetDownFlux();
	auto upFlux = flux.GetUpFlux();
	std::vector <G4int> flux_indices = flux.GetIndices();

	G4int iNbin = flux.get_iNbin();
	G4int oNbin = flux.get_oNbin();

	//G4cout << iNbin << " " << oNbin << G4endl;


	for (G4int i_ibin=0;i_ibin<flux_indices.size();i_ibin++)
	{
		G4int ibin = flux_indices[i_ibin];
		auto flux_down_i = downFlux[ibin];
		auto flux_up_i = upFlux[ibin];
		auto list_oindices = downFlux[ibin].GetListIndex();
		auto list_oindices_up = upFlux[ibin].GetListIndex();

		std::vector <G4int> list_all_indices;
		for (G4int ii=0;ii<list_oindices.size();ii++)
		{
			list_all_indices.push_back(list_oindices[ii]);
		}


		for (G4int ii=0;ii<list_oindices_up.size();ii++)
		{
			for (G4int jj=0;jj<list_oindices.size();jj++)
			{
				if (list_oindices[jj]==list_oindices_up[ii]) {break;}
			}
			list_all_indices.push_back(list_oindices_up[ii]);
		}

		//G4cout << G4endl << "--------------------------" << G4endl;

		//G4cout << ibin << " " << list_oindices.size() << " " << list_oindices_up.size() << G4endl;
		//for (G4int i_obin=0;i_obin<list_oindices.size();i_obin++)
		//{G4cout << list_oindices[i_obin] << " ";}
		//G4cout << G4endl;

		//for (G4int i_obin=0;i_obin<list_oindices_up.size();i_obin++)
		//{G4cout << list_oindices_up[i_obin] << " ";}
		//G4cout << G4endl;

		//for (G4int iii=0;iii<list_all_indices.size();iii++)
		//{G4cout << list_all_indices[iii] << " ";}
		//G4cout << G4endl;

		for (G4int iii=0;iii<list_all_indices.size();iii++)
		{
			G4int obin = list_all_indices[iii];
			G4int f_down = flux_down_i.GetBin(obin);
			G4int f_up = flux_up_i.GetBin(obin);

			//G4cout << ibin << " " << obin << " " << f_down << " " << f_up << G4endl;
			if ( (f_down==0) && (f_up==0)) {continue;}
			man->FillNtupleIColumn(2,0,ibin);
			man->FillNtupleIColumn(2,1,obin);
			man->FillNtupleIColumn(2,2,f_down);
			man->FillNtupleIColumn(2,3,f_up);
			man->AddNtupleRow(2);
			
		}
	}
	
	//}

	man->Write();
	man->CloseFile();
}

void MyRunAction::SetResultsDirectory(G4String dir)
{
	const MyGeometry *detectorConstruction = static_cast<const MyGeometry*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	
	phantomType = detectorConstruction->GetPhantomType();

	auto man = G4AnalysisManager::Instance();
	result_dir = "../results/"+phantomType+G4String("/")+dir+G4String("/");
	
	createDirectories(result_dir);
	
	man->SetDefaultFileType("csv");
	man->OpenFile(result_dir+labelCSV+ G4String(".csv"));
}

void MyRunAction::DefineCommands()
{
	fMessenger = new G4GenericMessenger(this, "/SIM/scoring/","Set scoring folder");

	auto& resdirCmd = fMessenger->DeclareMethod("resDir",&MyRunAction::SetResultsDirectory,"Folder for results");
	resdirCmd.SetParameterName("resDir", true);
	resdirCmd.SetDefaultValue("../results/"+phantomType+G4String("/"));
}


/*void MyRunAction::PrintResult(std::ostream &out)
{

//if (phantomType!="ICRP145") {return;}
 // Print run result
 //
 using namespace std;
 EMAP edepMap = *fRun->GetEDEMap();

    out << G4endl
	 << "=====================================================================" << G4endl
	 << " Run #" << fRunID << " / Number of event processed : "<< fNumOfEvent    << G4endl
	 << "=====================================================================" << G4endl
	 << "organ ID| "
	 << setw(19) << "Organ Mass (g)"
         << setw(19) << "Dose (Gy/source)"
	 //<< setw(19) << "Relative Error" 
	 << setw(34) << "Name of organ" << G4endl;

    out.precision(3);
    auto massMap = fTetData->GetMassMap();
    for(auto itr : massMap){
		G4double meanDose    = edepMap[itr.first]  / itr.second / fNumOfEvent;
		//G4double squareDose =  (edepMap[itr.first].second)/ (itr.second*itr.second);
		//G4double variance    = ((squareDose/fNumOfEvent) - (meanDose*meanDose))/fNumOfEvent;
		G4double relativeE(1);
		//if(meanDose > 0) relativeE   = sqrt(variance)/meanDose;

		out << setw(8)  << itr.first << "| "
			<< setw(19) << fixed      << itr.second/g;
		out	<< setw(19) << scientific << meanDose/(joule/kg);
		//out	<< setw(19) << fixed      << relativeE ; 
		out	<< setw(34) << fTetData->GetMaterial(itr.first)->GetName() << G4endl;
	}
	out << "=====================================================================" << G4endl << G4endl;
}*/

void createDirectories(const std::string& path) {
    std::string currentPath = "";
    for (char c : path) {
        if (c != '/') {
            currentPath += c;
        } else {
            currentPath += '/';
            if (!std::filesystem::exists(currentPath)) {
                std::filesystem::create_directory(currentPath);
            }
        }
    }
    if (!std::filesystem::exists(currentPath)) {
        std::filesystem::create_directory(currentPath);
    }
}
