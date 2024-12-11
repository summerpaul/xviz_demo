/**
 * @Author: Xia Yunkai
 * @Date:   2024-07-05 18:54:33
 * @Last Modified by:   Xia Yunkai
 * @Last Modified time: 2024-11-23 10:34:54
 */
#include <stdint.h>

#ifndef __DEMO_H__
#define __DEMO_H__
#include "app/app.h"
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "event_bus/subscriber.h"
#include "events/scene_object_events.h"
using namespace event_bus;
using namespace events;
class Demo : public Subscriber
{
public:
    Demo();
    ~Demo();
    void Run();

private:
    void DrawProcess();

    void TimeSeriesDoubleDataDemo();

    void DrawTransformDemo();

    void DrawPathDemo();

    void DrawPolygonDemo();

    void DrawCircleDemo();

    void DrawPoseDemo();

    void DrawPointCloudDemo();

    void DrawPathArrayDemo();

    void DrawPolygonArrayDemo();

    void DrawCirclehArrayDemo();

    void DrawPoseArrayDemo();

    void RealtimeDoubleDataDemo();

    virtual void OnNewEvent(Event *evt) override;

private:
    std::unique_ptr<app::App> m_pApp;
    std::thread m_thread;
    bool m_running;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_firstDraw = true;
};

#endif /* __DEMO_H__ */
