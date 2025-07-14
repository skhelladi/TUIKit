#include "OpenFoamParameters.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace OpenFoamTUI
{

    namespace fs = std::filesystem;

    void OpenFoamParameters::writeOpenFoamFiles(const std::string &output_dir) const
    {
        std::cout << "Writing OpenFoam files to: " << output_dir << std::endl;

        fs::path case_path = output_dir;
        fs::create_directories(case_path);
        fs::create_directories(case_path / "0");
        fs::create_directories(case_path / "constant");
        fs::create_directories(case_path / "system");

        // Write controlDict
        std::ofstream controlDictFile(case_path / "system" / "controlDict");
        controlDictFile << 
R"(/*--------------------------------*- C++ -*----------------------------------*\
| =========                 |                                                   |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox             |
|  \\    /   O peration     | Version:  v2312                                   |
|   \\  /    A nd           | Website:  www.OpenFOAM.com                        |
|    \\/     M anipulation  |                                                   |
*-------------------------------------------------------------------------------*/
FoamFile
{
    format      ascii;
    class       dictionary;
    location    "system";
    object      controlDict;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

application     )" << solverType
                        << R"(;

startFrom       startTime;

startTime       )" << startTime
                        << R"(;

stopAt          endTime;

endTime         )" << endTime
                        << R"(;

deltaT          )" << deltaT
                        << R"(;

writeControl    timeStep;

writeInterval   )" << writeInterval
                        << R"(;

purgeWrite      0;

writeFormat     ascii;

writePrecision  6;

writeCompression off;

runTimeModifiable true;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
)";
        controlDictFile.close();

        // Write transportProperties
        std::ofstream transportPropertiesFile(case_path / "constant" / "transportProperties");
        transportPropertiesFile << 
R"(/*--------------------------------*- C++ -*----------------------------------*\
| =========                 |                                                   |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox             |
|  \\    /   O peration     | Version:  v2312                                   |
|   \\  /    A nd           | Website:  www.OpenFOAM.com                        |
|    \\/     M anipulation  |                                                   |
*-------------------------------------------------------------------------------*\
FoamFile
{
    format      ascii;
    class       dictionary;
    location    "constant";
    object      transportProperties;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

)" << (flowType == "Incompressible" ? "nu              nu [0 2 -1 0 0 0 0] " + std::to_string(nu_value) + ";" : "")
                                << "\n";
        transportPropertiesFile.close();

        // Write 0/U
        std::ofstream UFile(case_path / "0" / "U");
        UFile << R"(FoamFile
{
    format      ascii;
    class       volVectorField;
    object      U;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

dimensions      [0 1 -1 0 0 0 0];

internalField   uniform ()"
              << U_x << " " << U_y << " " << U_z << R"();

boundaryField
{
    // Placeholder for boundary conditions
    defaultFaces
    {
        type            empty;
    }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
)";
        UFile.close();

        // Write 0/p
        std::ofstream pFile(case_path / "0" / "p");
        pFile << R"(FoamFile
{
    format      ascii;
    class       volScalarField;
    object      p;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

dimensions      [0 2 -2 0 0 0 0];

internalField   uniform )"
              << p_init << R"(;

boundaryField
{
    // Placeholder for boundary conditions
    defaultFaces
    {
        type            empty;
    }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
)";
        pFile.close();

        // Write 0/T (if compressible)
        if (flowType == "Compressible")
        {
            std::ofstream TFile(case_path / "0" / "T");
            TFile << R"(FoamFile
{
    format      ascii;
    class       volScalarField;
    object      T;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

dimensions      [0 0 0 1 0 0 0];

internalField   uniform )"
                  << T_init << R"(;

boundaryField
{
    // Placeholder for boundary conditions
    defaultFaces
    {
        type            empty;
    }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
)";
            TFile.close();
        }

        } // namespace OpenFoamTUI
    
    } // namespace OpenFoamTUI

    // Move the definition of readOpenFoamFiles outside of writeOpenFoamFiles
    namespace OpenFoamTUI
    {
        bool OpenFoamParameters::readOpenFoamFiles(const std::string &case_dir)
        {
            std::cout << "Reading OpenFoam files from: " << case_dir << std::endl;

            fs::path case_path = case_dir;
            if (!fs::exists(case_path) || !fs::is_directory(case_path))
            {
                std::cerr << "Error: Case directory not found: " << case_dir << std::endl;
                return false;
            }

            caseName = case_path.filename().string();

            // Read controlDict
            fs::path controlDictPath = case_path / "system" / "controlDict";
            if (fs::exists(controlDictPath))
            {
                std::ifstream controlDictFile(controlDictPath);
                std::string line;
                while (std::getline(controlDictFile, line))
                {
                    if (line.find("application") != std::string::npos)
                    {
                        size_t start = line.find("application") + std::string("application").length();
                        size_t end = line.find(";", start);
                        solverType = line.substr(start, end - start);
                        solverType.erase(0, solverType.find_first_not_of(" \t")); // Trim leading whitespace
                        solverType.erase(solverType.find_last_not_of(" \t") + 1); // Trim trailing whitespace
                    }
                    else if (line.find("startTime") != std::string::npos && line.find("startFrom") == std::string::npos)
                    {
                        size_t start = line.find("startTime") + std::string("startTime").length();
                        size_t end = line.find(";", start);
                        startTime = std::stod(line.substr(start, end - start));
                    }
                    else if (line.find("endTime") != std::string::npos)
                    {
                        size_t start = line.find("endTime") + std::string("endTime").length();
                        size_t end = line.find(";", start);
                        endTime = std::stod(line.substr(start, end - start));
                    }
                    else if (line.find("deltaT") != std::string::npos)
                    {
                        size_t start = line.find("deltaT") + std::string("deltaT").length();
                        size_t end = line.find(";", start);
                        deltaT = std::stod(line.substr(start, end - start));
                    }
                    else if (line.find("writeInterval") != std::string::npos)
                    {
                        size_t start = line.find("writeInterval") + std::string("writeInterval").length();
                        size_t end = line.find(";", start);
                        writeInterval = std::stod(line.substr(start, end - start));
                    }
                }
            }

            // Read transportProperties
            fs::path transportPropertiesPath = case_path / "constant" / "transportProperties";
            if (fs::exists(transportPropertiesPath))
            {
                std::ifstream transportPropertiesFile(transportPropertiesPath);
                std::string line;
                while (std::getline(transportPropertiesFile, line))
                {
                    if (line.find("nu") != std::string::npos)
                    {
                        size_t start = line.find("nu") + std::string("nu").length();
                        size_t end = line.find(";", start);
                        nu_value = std::stod(line.substr(start, end - start));
                        flowType = "Incompressible"; // Assume incompressible if nu is found
                    }
                }
            }

            std::cout << "Case parameters read: " << caseName << std::endl;
            return true;
        }
    } // namespace OpenFoamTUI
