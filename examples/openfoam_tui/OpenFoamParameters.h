#pragma once

#include <string>
#include <vector>
#include <map>

namespace OpenFoamTUI {

struct BoundaryCondition {
    std::string patchName;
    std::string type;
    std::map<std::string, std::string> values; // e.g., {"value", "(0 0 0)"}
};

class OpenFoamParameters {
public:
    // General Case Setup
    std::string caseName = "";
    std::string solverType = "icoFoam";
    std::string flowType = "Incompressible"; // "Incompressible" or "Compressible"
    std::string meshPath = "/path/to/your/polyMesh";

    // Initial and Boundary Conditions
    double U_x = 0.0;
    double U_y = 0.0;
    double U_z = 0.0;
    double p_init = 0.0;
    double T_init = 300.0; // Only for compressible

    // Physical Properties
    double nu_value = 1e-6; // Kinematic Viscosity (for incompressible)
    std::string thermoModel = "perfectGas"; // For compressible

    // Turbulence Model
    std::string flowRegime = "Laminar"; // "Laminar" or "Turbulent"
    std::string turbulenceModel = "kEpsilon"; // e.g., kEpsilon, kOmegaSST, Smagorinsky
    double k_init = 0.01;
    double epsilon_init = 0.01;
    double omega_init = 0.01;

    // Solver Control
    double startTime = 0.0;
    double endTime = 1.0;
    double deltaT = 0.001;
    double writeInterval = 0.1;

    // Post-processing
    bool generateParaview = false;
    bool runFoamToVTK = false;
    std::string customPostScriptPath = "";

    // Methods for file operations
    void writeOpenFoamFiles(const std::string& output_dir) const;
    bool readOpenFoamFiles(const std::string& case_dir);
};

} // namespace OpenFoamTUI
