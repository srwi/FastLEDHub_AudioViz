#include <CLI/CLI.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <math.h>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

#include "networking.h"
#include "audio.h"


NetworkingController networking;
AudioController audioController;

std::vector<int> triangle(int width, int height)
{
    const int bar_width = width / SPECTRUM_LINES;
    std::vector<int> output(width);
    for (int bar = 0; bar < SPECTRUM_LINES; ++bar)
    {
        for (int i = 0; i < bar_width; ++i)
        {
            output[bar * bar_width + i] = audioController.getSpectrum()[bar] / 255 * height;
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

    CLI::App app("FastLEDHubSpectrum");

    app.set_help_flag();
    auto help = app.add_flag("-h,--help", "Print help");

    auto visualize = app.add_flag("-v,--visualize", "Visualize spectrum in terminal");

    std::vector<std::string> uris;
    app.add_option("-u,--uris", uris, "Websocket URIs")->check(CLI::ValidIPV4);

    float beta{ 0 };
    app.add_option("-b,--beta", beta, "Low pass filter beta coefficient")->check(CLI::Range(0., 1.));

    float interval{ 0 };
    app.add_option("-i,--interval", interval, "Websocket send interval")->check(CLI::PositiveNumber);

    int device{ 0 };
    app.add_option("-d,--device", device, "Audio device index")->check(CLI::Number);

    try
    {
        app.parse(argc, argv);
        if (*help)
        {
            throw CLI::CallForHelp();
        }
    }
    catch (const CLI::Error& e)
    {
        return app.exit(e);
    }

    for (std::string uri : uris)
    {
        std::string address = "ws://" + uri + ":81";
        networking.addConnection(address);
    }

    audioController.begin(device, beta, interval, audioCallback);

    if (*visualize)
    {
        using namespace ftxui;
        using namespace std::chrono_literals;

        std::string reset_position;

        for (;;) {
            auto screen = Screen::Create(Dimension::Full());

            int width = screen.dimx() - screen.dimx() % SPECTRUM_LINES;
            auto document = graph(std::ref(triangle)) | size(WIDTH, EQUAL, width) | border | hcenter;

            Render(screen, document);
            std::cout << reset_position;
            screen.Print();
            reset_position = screen.ResetPosition();
            const auto sleep_time = 1.001s;
            std::this_thread::sleep_for(sleep_time);
        }
    }
    else
    {
        while (1);
    }

    return 0;
}