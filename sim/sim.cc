/*
 * sim.cc
 */
#define VERSION_STR "Version: V0.1.0"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

//#include "G4RunManagerFactory.hh"
#include "G4RunManager.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"

#include "G4PhysListFactory.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//#include "Randomize.hh"

//#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalParameters.hh"
#include "G4OpticalPhysics.hh"
//#include "FTFP_BERT.hh"

#include "SimCfg.hh"
#include "IOManager.hh"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

using namespace sim;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  // -----------------------------------------------------
  // Set and parse program options
  // eg. ./Sim --help
  po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("vis", "interactive mode")
        ("mac", po::value<std::string>(), "macro file")
        ("version", "simulation version information")
        ("config", po::value<std::string>(), "input configuration file")
  ;
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
  if (vm.count("version")) {
    std::cout << VERSION_STR << "\n";
    return 1;
  }

  /////////////////////////
  // Load configuration file
  std::string config_filename;
  // custom config file
  if (vm.count("config")) {
    config_filename = vm["config"].as<std::string>();
  }
  // default config file
  else {
    config_filename = "../config/config.json";
  }
  SimCfg & config = SimCfg::Instance(config_filename);

  // -----------------------------------------------------
  // Set up IOManager and copy config to IOManager
  IOManager & ioManager = IOManager::Instance("out.json");
  ioManager.data_json["config"] = config.getCfg();
  // -----------------------------------------------------

  // Detect interactive mode and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if (vm.count("vis")) { ui = new G4UIExecutive(argc, argv); }

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  //use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  //
  auto* runManager = new G4RunManager;
    //G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  G4PhysListFactory physListFactory;
  G4VModularPhysicsList* physicsList = physListFactory.GetReferencePhysList("FTFP_BERT");

  // ----------------------------------
  // if scint_option is enabled
  if (config.getBool("/sim_options/scint_option")) {
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    auto opticalParams               = G4OpticalParameters::Instance();

    opticalParams->SetWLSTimeProfile("delta");

    opticalParams->SetScintTrackSecondariesFirst(true);

    opticalParams->SetCerenkovMaxPhotonsPerStep(100);
    opticalParams->SetCerenkovMaxBetaChange(10.0);
    opticalParams->SetCerenkovTrackSecondariesFirst(true);

    physicsList->RegisterPhysics(opticalPhysics);
  }
  runManager->SetUserInitialization(physicsList);
  // ----------------------------------

  // TODO: check for correct physics list
  //G4PhysListFactory physListFactory;
  //G4VModularPhysicsList* physicsList = physListFactory.GetReferencePhysList("G4EmStandardPhysics_option4");
  //physicsList->SetVerboseLevel(1);
  //runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName;
    if (vm.count("mac")) {
      fileName = vm["mac"].as<std::string>();
      UImanager->ApplyCommand(command+fileName);
    }
    else{
      G4cout << "ERROR -> No macro file specified" << G4endl;
    }
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
