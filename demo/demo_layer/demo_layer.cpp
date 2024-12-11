/**
 * @Author: Xia Yunkai
 * @Date:   2024-09-06 23:56:14
 * @Last Modified by:   Xia Yunkai
 * @Last Modified time: 2024-11-22 20:51:30
 */
#include <iostream>

#include "basis/base_layer.h"

#include <imgui.h>
#include <implot.h>
#include "app/app.h"
#include "events/scene_object_events.h"
#include "event_bus/subscriber.h"
using namespace event_bus;
using namespace events;
class DemoLayer : public ui::BaseLayer, public Subscriber
{
public:
    DemoLayer() : BaseLayer("DemoLayer") {}
    virtual void Update(float deltaTime) override
    {
        // ImGui::Begin("Hello, ImGui!");
        // ImGui::Text("This is a simple example of ImGui integration.");
        ImPlot::ShowDemoWindow();
        ImGui::ShowDemoWindow();

        // ImGui::End();
    }
    virtual bool Init() override
    {
        Subscriber::Subscribe(TypeIdentifier<PlotRealtimeDoubleData>::name());
        Subscriber::Subscribe(TypeIdentifier<PlotTimeSeriesDoubleDataMapEvent>::name());
        return true;
    }
    virtual void FilesDropCallback(int count, const char **paths) override
    {
        for (int i = 0; i < count; i++)
        {
            std::cout << "drop " << paths[i] << std::endl;
        }
    }
    virtual void Shutdown() override
    {
    }
    virtual void FilesOpenCallback(const std::vector<std::string> &paths) override
    {

        for (const auto &path : paths)
        {
            std::cout << "open " << path << std::endl;
        }
    }
    virtual void OnNewEvent(Event *evt) override
    {
        if (typeid(*evt) == typeid(PlotRealtimeDoubleData))
        {
            auto plot_evt = static_cast<PlotRealtimeDoubleData *>(evt);
            std::cout << "reveive  PlotRealtimeDoubleData" << std::endl;
        }
        else if (typeid(*evt) == typeid(PlotTimeSeriesDoubleDataMapEvent))
        {
            auto plot_double_evt = static_cast<PlotTimeSeriesDoubleDataMapEvent *>(evt);
            std::cout << "reveive  PlotRealtimeDoubleData" << std::endl;
        }
    }
};

using namespace std;

int main(int argc, char const *argv[])
{

    app::App app;
    app.Init();
    std::shared_ptr<DemoLayer> demo = std::make_shared<DemoLayer>();
    app.AddLayer(demo);
    while (app.IsRunning())
    {
        app.Update();
    }

    return 0;
}
