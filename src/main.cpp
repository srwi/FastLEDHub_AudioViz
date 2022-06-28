#include <CLI/CLI.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <vector>

#include "networking.h"
#include "audio.h"


NetworkingController networking;
AudioController audioController;

std::vector<int> spectrum_graph(int width, int height)
{
    const int bar_width = width / SPECTRUM_LINES;
    std::vector<int> output(width);
    for (int bar = 0; bar < SPECTRUM_LINES; ++bar)
    {
        for (int i = 0; i < bar_width; ++i)
        {
            output[bar * bar_width + i] = audioController.getSpectrum()[bar] * height;
        }
    }
    return output;
}

void audioCallback(std::vector<uint8_t> data)
{
    // Insert spectrum data indicator
    data.insert(data.begin(), 30);

    networking.broadcastData(data);
}

int main(int argc, char** argv)
{
    CLI::App app("FastLEDHub-AudioViz");

    auto visualize = app.add_flag("-v,--visualize", "Visualize spectrum in terminal");
    auto microphone = app.add_flag("-m,--microphone", "Use microphone to capture audio (default: system audio)");
    std::vector<std::string> ips;
    app.add_option("devices", ips, "List of target ip addresses")->check(CLI::ValidIPV4)->required();
    float period = 10;
    app.add_option("-p,--period", period, "Data transmission period (default: 10 [ms])")->check(CLI::PositiveNumber);
    float beta = 0.25;
    app.add_option("-b,--beta", beta, "Low pass filter beta coefficient (default: 0.25)")->check(CLI::Range(0., 1.));

    CLI11_PARSE(app, argc, argv);

    for (std::string ip : ips)
    {
        std::string uri = "ws://" + ip + ":81";
        networking.addConnection(uri);
    }

    int device = *microphone ? -2 : -3;
    if (!audioController.begin(device, beta, period, audioCallback))
        return 1;

    if (*visualize)
    {
        using namespace ftxui;
        using namespace std::chrono_literals;

        std::string reset_position;

        for (;;)
        {
            auto screen = Screen::Create(Dimension::Full());

            int width = screen.dimx() - screen.dimx() % SPECTRUM_LINES;
            auto document = graph(std::ref(spectrum_graph)) | size(WIDTH, EQUAL, width) | border | hcenter;

            Render(screen, document);
            std::cout << reset_position;
            screen.Print();
            reset_position = screen.ResetPosition();
            const auto sleep_time = 0.017s;
            std::this_thread::sleep_for(sleep_time);
        }
    }
    else
    {
        std::cin.get();
    }

    return 0;
}