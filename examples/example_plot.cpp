#include "tuikit.h"
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <thread>

using namespace TUIKIT;

// Left: live curves pushed by a worker thread. Right: a CSV file followed while it grows,
// and a static plot with fixed ranges. Keys act on the graph having the focus.
int main() {
    std::ofstream log_file("debug_log.txt");  // keep std::cerr away from the terminal
    std::cerr.rdbuf(log_file.rdbuf());
    setenv("COLORTERM", "truecolor", 1);

    TUIApp app("TUIPlot demo");

    auto convergence = plot("Live: convergence (follow tail)");
    convergence->setMinimumSize(30, 12);
    convergence->xAxis().setLabel("iteration").setFollowTail(300);
    convergence->yAxis().setLabel("residual");
    auto ux = convergence->addSeries("Ux");
    ux->setLineStyle(TUIPlotLineStyle::Dashed).setMarker(TUIPlotMarker::Circle).setMarkerStride(25);
    auto p = convergence->addSeries("p");
    p->setLineWidth(2).setMarker(TUIPlotMarker::Square).setMarkerStride(25);
    auto k = convergence->addSeries("k");
    k->setLineStyle(TUIPlotLineStyle::Dotted).setColor(ftxui::Color::RGB(255, 128, 0)).setCapacity(300);

    auto cpu_plot = plot("CSV tail: CPU load (fixed range)");
    cpu_plot->setMinimumSize(30, 12);
    cpu_plot->xAxis().setLabel("time [s]").setFollowTail(30.0).setMinorGrid(true);
    cpu_plot->yAxis().setLabel("cpu [%]").setRange(0.0, 100.0).setMinorGrid(true);
    auto cpu = cpu_plot->addSeries("cpu");

    auto fixed = plot("Static: samples vs model (fixed ranges)");
    fixed->setMinimumSize(30, 12);
    fixed->xAxis().setLabel("x").setRange(-1.0, 7.0).setTickFormat("%.1f").setMinorGrid(true);
    fixed->yAxis().setLabel("y").setRange(-1.5, 1.5).setTickFormat("%.1f");
    auto samples = fixed->addSeries("samples");
    samples->setLineStyle(TUIPlotLineStyle::None).setMarker(TUIPlotMarker::Triangle);
    auto model = fixed->addSeries("model");
    const auto damped = [](double x) { return std::cos(x) * std::exp(-0.15 * x); };
    for (int i = 0; i < 12; ++i) samples->append(i * 6.0 / 11.0, damped(i * 6.0 / 11.0));
    for (int i = 0; i <= 300; ++i) model->append(-1.0 + 8.0 * i / 300.0, damped(-1.0 + 8.0 * i / 300.0));

    // Full-width layout: one plot per row, each stretched to the whole terminal width.
    auto root = vbox();
    root->addWidget(convergence);
    root->addWidget(cpu_plot);
    root->addWidget(fixed);
    root->addWidget(label(std::string("up/down: change graph   ") + TUIPlot::kHelp));
    app.setMainWidget(root);
    convergence->get_ftxui_component()->TakeFocus();  // no widget has the focus at startup: give the keys to the first graph

    const std::string path = "example_plot.csv";
    std::ofstream csv(path);
    TUICSVTailReader reader(path);
    reader.setColumns("time", "cpu");  // header names: resolved when the first line is read
    const auto produce = [&](int i) {
        ux->append(i, std::pow(10.0, -i / 400.0) * (1.0 + 0.3 * std::sin(i * 0.15)));
        p->append(i, std::pow(10.0, -i / 550.0) * (1.0 + 0.2 * std::cos(i * 0.1)));
        k->append(i, 0.1 * std::pow(10.0, -i / 800.0) * (1.0 + 0.5 * std::sin(i * 0.07)));
        csv << i * 0.1 << ";" << 50.0 + 40.0 * std::sin(i * 0.12) << ";extra\n";
    };
    csv << "time;cpu;extra\n";  // header: the separator is detected from the first line
    for (int i = 0; i < 300; ++i) produce(i);  // history so both windows start full
    csv.flush();

    std::atomic<bool> stop{false};
    std::thread producer([&] {
        std::vector<std::pair<double, double>> rows;
        for (int i = 300; !stop; ++i) {
            produce(i);
            csv.flush();
            if (reader.poll(rows)) {
                for (const auto& [time, load] : rows) cpu->append(time, load);
            }
            app.PostEvent(ftxui::Event::Custom);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        csv.close();
        std::remove(path.c_str());
    });

    const int status = app.exec();
    stop = true;
    producer.join();
    return status;
}
