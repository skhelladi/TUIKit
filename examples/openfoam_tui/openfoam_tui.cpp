#include "tuikit.h"
#include <iostream>
#include <string>
#include <fstream>
#include "OpenFoamParameters.h"

using namespace TUIKIT;
using namespace OpenFoamTUI;

int main()
{
    std::ofstream cerr_log("debug_log_openfoam_tui.txt");
    std::streambuf *old_cerr_buf = std::cerr.rdbuf();
    std::cerr.rdbuf(cerr_log.rdbuf());

    TUIApp app("OpenFoam TUI");

    OpenFoamParameters params;

    // UI Elements
    auto case_name_input = textfield(params.caseName);
    
    std::vector<std::string> solver_types = {"icoFoam", "simpleFoam", "pisoFoam", "rhoSimpleFoam"};
    auto solver_type_combobox = combobox(solver_types, 0); // Default to icoFoam

    std::vector<std::string> flow_types = {"Incompressible", "Compressible"};
    auto flow_type_radiobox = radiobox(flow_types, 0); // Default to Incompressible

    auto mesh_path_input = textfield(params.meshPath);

    // Initial and Boundary Conditions
    auto U_x_input = doublespinbox("U_x", params.U_x, -100.0, 100.0, 0.1);
    auto U_y_input = doublespinbox("U_y", params.U_y, -100.0, 100.0, 0.1);
    auto U_z_input = doublespinbox("U_z", params.U_z, -100.0, 100.0, 0.1);

    auto p_init_input = doublespinbox("p_init", params.p_init, -100000.0, 100000.0, 100.0);

    auto T_init_input = doublespinbox("T_init", params.T_init, 0.0, 1000.0, 1.0);

    // Fluid Properties
    auto nu_value_input = doublespinbox("Kinematic Viscosity (nu)", params.nu_value, 1e-10, 1.0, 1e-7);
    std::vector<std::string> thermo_models = {"perfectGas", "rhoConst"};
    auto thermo_model_combobox = combobox(thermo_models, 0); // Default to perfectGas

    // Turbulence Model
    std::vector<std::string> flow_regimes = {"Laminar", "Turbulent"};
    auto flow_regime_radiobox = radiobox(flow_regimes, 0); // Default to Laminar

    std::vector<std::string> turbulence_models = {"kEpsilon", "kOmegaSST", "Smagorinsky"};
    auto turbulence_model_combobox = combobox(turbulence_models, 0); // Default to kEpsilon
    auto turbulence_model_label = label("Turbulence Model:");

    auto k_init_label = label("k (initial):");
    auto epsilon_init_label = label("epsilon (initial):");
    auto omega_init_label = label("omega (initial):");

    auto k_init_input = doublespinbox("", params.k_init, 0.0, 1.0, 0.001);
    auto epsilon_init_input = doublespinbox("", params.epsilon_init, 0.0, 1.0, 0.001);
    auto omega_init_input = doublespinbox("", params.omega_init, 0.0, 1.0, 0.001);

    // Solver Control
    auto start_time_input = doublespinbox("Start Time", params.startTime, 0.0, 1000.0, 0.1);
    auto end_time_input = doublespinbox("End Time", params.endTime, 0.0, 1000.0, 0.1);
    auto delta_t_input = doublespinbox("Delta T", params.deltaT, 0.0, 1.0, 0.0001);
    auto write_interval_input = doublespinbox("Write Interval", params.writeInterval, 0.0, 100.0, 0.01);

    // Post-processing
    auto generate_paraview_checkbox = checkbox("Generate ParaView files (.foam)");
    auto run_foamToVTK_checkbox = checkbox("Run foamToVTK");
    auto custom_post_script_path = textfield("/path/to/custom_script.sh");
    auto run_custom_script_button = button("Run Custom Script");

    auto generate_button = button("Generate OpenFoam Files");
    generate_button->setIcon(ICON::NewFile);
    // The TUITextArea does not currently support a direct 'read-only' mode or dynamic expansion.
    // Setting a large fixed height to simulate taking up more space.
    auto log_output_label = textarea("Status: Ready", "", 20); // 20 lines height, no label
    log_output_label->setReadOnly(true);
    auto scrollable_log = scrollable_container(log_output_label, 10); // Wrap in scrollable container with height 10

    // Toolbar
    auto toolbar_widget = toolbar();
    toolbar_widget->addButton("New", [&app] { Notification::show(app, "New clicked!", "Info", 2000); }, ICON::NewFile);
    toolbar_widget->addButton("Open", [&app, &params, case_name_input, solver_type_combobox, flow_type_radiobox, mesh_path_input, U_x_input, U_y_input, U_z_input, p_init_input, T_init_input, nu_value_input, thermo_model_combobox, flow_regime_radiobox, turbulence_model_combobox, k_init_input, epsilon_init_input, omega_init_input, start_time_input, end_time_input, delta_t_input, write_interval_input, generate_paraview_checkbox, run_foamToVTK_checkbox, custom_post_script_path, log_output_label, solver_types, flow_types, thermo_models, flow_regimes, turbulence_models] {
        // For simplicity, let's assume a fixed path for now or prompt user
        std::string case_dir = "./my_openfoam_case"; // Example path
        if (params.readOpenFoamFiles(case_dir)) {
            // TUIKIT widgets do not currently support direct setting of values after creation.
            // To fully update the UI, one would typically need to recreate the widgets
            // or modify TUIKIT to expose setter methods (e.g., setText, setSelected, setChecked).
            // For now, we will just update the status label.

            log_output_label->setText("Status: Loaded case " + params.caseName + ". UI elements not updated due to TUIKIT limitations.");
            Notification::show(app, "Case loaded successfully! (UI not fully updated)", "Success", 2000);
        } else {
            log_output_label->setText("Status: Failed to load case from " + case_dir);
            Notification::show(app, "Failed to load case!", "Error", 2000);
        }
    }, ICON::Open);
    toolbar_widget->addButton("Save", [&app] { Notification::show(app, "Save clicked!", "Info", 2000); }, ICON::Save);
    toolbar_widget->addButton("Quit", [&app] { app.exit(); }, ICON::Quit);

    // Layout
    auto main_layout = vbox();
    main_layout->addWidget(toolbar_widget);

    auto tab_widget = tabwidget();

    // General Setup Tab
    auto general_setup_tab_content = vbox();
    general_setup_tab_content->addWidget(label("Case Name:"));
    general_setup_tab_content->addWidget(case_name_input);
    general_setup_tab_content->addWidget(label("Solver Type:"));
    general_setup_tab_content->addWidget(solver_type_combobox);
    general_setup_tab_content->addWidget(label("Flow Type:"));
    general_setup_tab_content->addWidget(flow_type_radiobox);
    general_setup_tab_content->addWidget(label("Mesh Path:"));
    general_setup_tab_content->addWidget(mesh_path_input);
    tab_widget->addTab("General Setup", general_setup_tab_content, ICON::Home);

    // Initial and Boundary Conditions Tab
    auto ibc_tab_content = vbox();
    ibc_tab_content->addWidget(label("Velocity (U) Initial Value:"));
    ibc_tab_content->addWidget(U_x_input);
    ibc_tab_content->addWidget(U_y_input);
    ibc_tab_content->addWidget(U_z_input);
    ibc_tab_content->addWidget(label("Pressure (p) Initial Value:"));
    ibc_tab_content->addWidget(p_init_input);
    ibc_tab_content->addWidget(label("Temperature (T) Initial Value:"));
    ibc_tab_content->addWidget(T_init_input);
    tab_widget->addTab("Initial & BCs", ibc_tab_content, ICON::Tasks);

    // Fluid Properties Tab
    auto fluid_properties_tab_content = vbox();
    fluid_properties_tab_content->addWidget(label("Kinematic Viscosity (nu):"));
    fluid_properties_tab_content->addWidget(nu_value_input);
    fluid_properties_tab_content->addWidget(label("Thermodynamic Model:"));
    fluid_properties_tab_content->addWidget(thermo_model_combobox);
    tab_widget->addTab("Fluid Properties", fluid_properties_tab_content, ICON::Tasks);

    // Turbulence Model Tab
    auto turbulence_tab_content = vbox();
    turbulence_tab_content->addWidget(label("Flow Regime:"));
    turbulence_tab_content->addWidget(flow_regime_radiobox);
    turbulence_tab_content->addWidget(label("Turbulence Model:"));
    turbulence_tab_content->addWidget(turbulence_model_combobox);
    turbulence_tab_content->addWidget(k_init_label);
    turbulence_tab_content->addWidget(k_init_input);
    turbulence_tab_content->addWidget(epsilon_init_label);
    turbulence_tab_content->addWidget(epsilon_init_input);
    turbulence_tab_content->addWidget(omega_init_label);
    turbulence_tab_content->addWidget(omega_init_input);
    tab_widget->addTab("Turbulence", turbulence_tab_content, ICON::Info);

    connect(turbulence_model_combobox, [&](int selected_index) {
        std::string selected_model = turbulence_models[selected_index];
        if (selected_model == "kEpsilon") {
            k_init_label->setText("k (initial):");
            epsilon_init_label->setText("epsilon (initial):");
            omega_init_label->setText("");
        } else if (selected_model == "kOmegaSST") {
            k_init_label->setText("k (initial):");
            epsilon_init_label->setText("omega (initial):");
            omega_init_label->setText("");
        } else if (selected_model == "Smagorinsky") {
            k_init_label->setText("");
            epsilon_init_label->setText("");
            omega_init_label->setText("");
        }
        app.request_redraw();
    });

    // Solver Control Tab
    auto solver_control_tab_content = vbox();
    solver_control_tab_content->addWidget(label("Start Time:"));
    solver_control_tab_content->addWidget(start_time_input);
    solver_control_tab_content->addWidget(label("End Time:"));
    solver_control_tab_content->addWidget(end_time_input);
    solver_control_tab_content->addWidget(label("Delta T:"));
    solver_control_tab_content->addWidget(delta_t_input);
    solver_control_tab_content->addWidget(label("Write Interval:"));
    solver_control_tab_content->addWidget(write_interval_input);
    tab_widget->addTab("Solver Control", solver_control_tab_content, ICON::Tasks);

    // Post-processing Tab
    auto post_processing_tab_content = vbox();
    post_processing_tab_content->addWidget(generate_paraview_checkbox);
    post_processing_tab_content->addWidget(run_foamToVTK_checkbox);
    post_processing_tab_content->addWidget(label("Custom Post-processing Script Path:"));
    post_processing_tab_content->addWidget(custom_post_script_path);
    post_processing_tab_content->addWidget(run_custom_script_button);
    tab_widget->addTab("Post-processing", post_processing_tab_content, ICON::Info);

    main_layout->addWidget(tab_widget);
    main_layout->addWidget(generate_button);
    main_layout->addWidget(groupbox("Simulation Log", scrollable_log));

    app.setMainWidget(main_layout);

    // Connect signals and slots
    connect(generate_button, [&]
            {
        params.caseName = case_name_input->text();
        params.meshPath = mesh_path_input->text();
        params.solverType = solver_type_combobox->selectedText();
        params.flowType = flow_type_radiobox->selectedText();
        
        params.U_x = U_x_input->value();
        params.U_y = U_y_input->value();
        params.U_z = U_z_input->value();
        params.p_init = p_init_input->value();
        params.T_init = T_init_input->value();

        params.nu_value = nu_value_input->value();
        params.thermoModel = thermo_model_combobox->selectedText();

        params.flowRegime = flow_regime_radiobox->selectedText();
        params.turbulenceModel = turbulence_model_combobox->selectedText();
        params.k_init = k_init_input->value();
        params.epsilon_init = epsilon_init_input->value();
        params.omega_init = omega_init_input->value();

        params.startTime = start_time_input->value();
        params.endTime = end_time_input->value();
        params.deltaT = delta_t_input->value();
        params.writeInterval = write_interval_input->value();

        params.generateParaview = generate_paraview_checkbox->checked();
        params.runFoamToVTK = run_foamToVTK_checkbox->checked();
        params.customPostScriptPath = custom_post_script_path->text();

        if (params.caseName.empty()) {
            log_output_label->setText("Status: Please enter a case name.");
        } else if (params.meshPath.empty()) {
            log_output_label->setText("Status: Please enter a mesh path.");
        }
        else {
            std::string status_text = "Status: Generating files for case: " + params.caseName +
                                      "\nSolver Type: " + params.solverType +
                                      "\nFlow Type: " + params.flowType +
                                      "\nMesh Path: " + params.meshPath +
                                      "\n\nInitial Conditions:" +
                                      "\n  U = (" + std::to_string(params.U_x) + ", " + std::to_string(params.U_y) + ", " + std::to_string(params.U_z) + ")" +
                                      "\n  p = " + std::to_string(params.p_init) +
                                      "\n  T = " + std::to_string(params.T_init) +
                                      "\n\nPhysical Properties:" +
                                      "\n  nu = " + std::to_string(params.nu_value) +
                                      "\n  Thermodynamic Model: " + params.thermoModel +
                                      "\n\nTurbulence Model:" +
                                      "\n  Flow Regime: " + params.flowRegime +
                                      "\n  Turbulence Model: " + (params.flowRegime == "Turbulent" ? params.turbulenceModel : "N/A") +
                                      "\n  k = " + std::to_string(params.k_init) +
                                      "\n  epsilon = " + std::to_string(params.epsilon_init) +
                                      "\n  omega = " + std::to_string(params.omega_init) +
                                      "\n\nSolver Control:" +
                                      "\n  Start Time: " + std::to_string(params.startTime) +
                                      "\n  End Time: " + std::to_string(params.endTime) +
                                      "\n  Delta T: " + std::to_string(params.deltaT) +
                                      "\n  Write Interval: " + std::to_string(params.writeInterval) +
                                      "\n\nPost-processing:" +
                                      "\n  Generate ParaView: " + (params.generateParaview ? "Yes" : "No") +
                                      "\n  Run foamToVTK: " + (params.runFoamToVTK ? "Yes" : "No") +
                                      "\n  Custom Script: " + (params.customPostScriptPath.empty() ? "N/A" : params.customPostScriptPath);

            log_output_label->setText(status_text);
            params.writeOpenFoamFiles(params.caseName);
            Notification::show(app, "Files generated for " + params.caseName, "Success", 3000);
        }
             });

    connect(run_custom_script_button, [&app, &log_output_label, custom_post_script_path] {
        std::string script_path = custom_post_script_path->text();
        if (script_path.empty()) {
            Notification::show(app, "No custom script path provided.", "Warning", 3000);
            log_output_label->setText("Status: No custom script path provided.");
        } else {
            Notification::show(app, "Running custom script: " + script_path, "Info", 3000);
            log_output_label->setText("Status: Running custom script: " + script_path);
            // In a real application, you would execute the script here.
        }
    });

    app.setOnExit([
                  &
                  ]
                  {
        std::cerr << "Application is exiting. Goodbye from OpenFoam TUI!" << std::endl;
        std::cerr.rdbuf(old_cerr_buf); });

    int result = app.exec();

    std::cerr.rdbuf(old_cerr_buf);

    return result;
}
