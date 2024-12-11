/**
 * @Author: Xia Yunkai
 * @Date:   2024-07-05 18:54:40
 * @Last Modified by:   Xia Yunkai
 * @Last Modified time: 2024-11-24 09:02:17
 */
#include <iostream>
#include "demo.h"
#include "utils/xviz_util.h"
#include "basis/xviz_time.h"
#include "events/scene_object_events.h"
#include "basis/defines.h"
#include <functional>
#include "xviz_log/xviz_log.h"
#include "imgui.h"

const char *items[] = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK"};
static int item_current = 0;

using namespace utils;
Demo::Demo() : m_running(false)
{
    m_pApp = std::make_unique<app::App>();
    m_pApp->Init();
    m_pApp->RegisterUiCallback("oooo", [&]()
                               {
				ImGui::Begin("222");
				ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
				ImGui::End(); });
    m_running = true;
    m_thread = std::thread(std::bind(&Demo::DrawProcess, this));

    Subscriber::Subscribe(TypeIdentifier<PlotRealtimeDoubleData>::name());
    Subscriber::Subscribe(TypeIdentifier<PlotTimeSeriesDoubleDataMapEvent>::name());
}
Demo::~Demo()
{
    m_running = false;
    if (m_thread.joinable())
        m_thread.join();
}
void Demo::Run()
{
    while (m_pApp->IsRunning())
    {
        m_mutex.lock();

        m_pApp->Update();
        m_mutex.unlock();
    }
}

void Demo::DrawProcess()
{
    while (m_running)
    {
        TimeSeriesDoubleDataDemo();
        DrawTransformDemo();
        DrawPathDemo();
        DrawPolygonDemo();
        DrawCircleDemo();
        DrawPoseDemo();
        DrawPointCloudDemo();
        DrawPathArrayDemo();
        DrawPolygonArrayDemo();
        DrawCirclehArrayDemo();
        DrawPoseArrayDemo();
        RealtimeDoubleDataDemo();
        if (m_firstDraw)
        {
            m_firstDraw = false;
        }

        m_cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Demo::TimeSeriesDoubleDataDemo()
{
    float a1 = RandomRange(0.0f, 10.0f);
    float a2 = RandomRange(5.0f, 10.0f);
    float a3 = RandomRange(0.0f, 100.0f);
    float a4 = RandomRange(50.0f, 100.0f);
    float a5 = RandomRange(-500.0f, 500.0f);

    float b1 = 2.0f * a1;
    float b2 = 2.0f * a2;
    float b3 = 2.0f * a3;
    float b4 = 2.0f * a4;
    float b5 = 2.0f * a5;

    auto double_datas = new events::PlotTimeSeriesDoubleDataMapEvent();
    double_datas->datas["a1"] = a1;
    double_datas->datas["a2"] = a2;
    double_datas->datas["a3"] = a3;
    double_datas->datas["a4"] = a4;
    double_datas->datas["a5"] = a5;
    double_datas->datas["b1"] = b1;
    double_datas->datas["b2"] = b2;
    double_datas->datas["b3"] = b3;
    double_datas->datas["b4"] = b4;
    double_datas->datas["b5"] = b5;
    double_datas->time = GetTimeNow();
    m_pApp->Publish(double_datas);
    m_pApp->SetCurPlotTime(GetTimeNow());
}

void Demo::DrawTransformDemo()
{

    auto tf_node1 = new events::TransformNodeEvent();
    tf_node1->frameId = BASE_LINK;
    tf_node1->parentFrameId = WORLD_FRAME;
    tf_node1->x = 5.0f;
    tf_node1->y = 5.0f;

    tf_node1->yaw = XVIZ_PI4;
    m_pApp->Publish(tf_node1);

    auto tf_node2 = new events::TransformNodeEvent();
    tf_node2->frameId = BASE_LINK2;
    tf_node2->parentFrameId = BASE_LINK;
    tf_node2->x = 5.0f;
    tf_node2->y = 5.0f;

    tf_node2->yaw = XVIZ_PI4;
    m_pApp->Publish(tf_node2);
}

void Demo::DrawPathDemo()
{
    ScenePath scene_path;
    scene_path.header.frameId = WORLD_FRAME;
    scene_path.header.name = "world_path";
    scene_path.options.color = XVIZ_YELLOW;
    for (float s = 0; s < XVIZ_2PI; s += 0.05f)
    {
        scene_path.points.emplace_back(Point{s, sinf(s), 0});
    }

    auto path1 = new events::ScenePathEvent();
    path1->path = scene_path;
    path1->optionsValid = m_firstDraw;
    m_pApp->Publish(path1);
    auto path2 = new events::ScenePathEvent();
    scene_path.header.frameId = BASE_LINK;
    scene_path.header.name = "baselink_path";
    scene_path.options.color = XVIZ_BROWN;
    path2->path = scene_path;
    path2->optionsValid = m_firstDraw;
    m_pApp->Publish(path2);

    auto path3 = new events::ScenePathEvent();
    scene_path.header.frameId = BASE_LINK2;
    scene_path.header.name = "baselink2_path";
    scene_path.isDashed = true;
    scene_path.options.color = XVIZ_LIGHT_CYAN;
    path3->path = scene_path;
    path3->optionsValid = m_firstDraw;
    m_pApp->Publish(path3);
}

void Demo::DrawPolygonDemo()
{
    Point p0{0.0f, 0.0f, 0.0f}, p1{2.5f, 0.0f, 0.0f}, p2{2.5f, 2.5f, 0.0f}, p3{0.0f, 2.5f, 0.0f};
    ScenePolygon polygon;
    polygon.header.frameId = WORLD_FRAME;
    polygon.header.name = "world_polygon";
    polygon.header.info = "world_polygon";
    polygon.vertices.push_back(p0);
    polygon.vertices.push_back(p1);
    polygon.vertices.push_back(p2);
    polygon.vertices.push_back(p3);
    polygon.options.color = XVIZ_GREEN;
    auto polygon_event = new events::ScenePolygonEvent();
    polygon_event->polygon = polygon;
    polygon_event->optionsValid = m_firstDraw;
    m_pApp->Publish(polygon_event);
    auto polygon_event2 = new events::ScenePolygonEvent();
    polygon.header.frameId = BASE_LINK;
    polygon.header.name = "base_link_polygon";
    polygon.header.info = "base_link_polygon";
    polygon_event2->polygon = polygon;
    polygon_event2->optionsValid = m_firstDraw;
    polygon.options.color = XVIZ_RED;
    polygon_event2->optionsValid = false;
    m_pApp->Publish(polygon_event2);
    auto polygon_event3 = new events::ScenePolygonEvent();
    polygon.header.frameId = BASE_LINK2;
    polygon.header.name = "base_link2_polygon";
    polygon.header.info = "base_link2_polygon";
    polygon.filled = true;
    polygon_event3->polygon = polygon;
    polygon_event3->optionsValid = m_firstDraw;
    polygon.options.color = XVIZ_BLUE;
    m_pApp->Publish(polygon_event3);
}

void Demo::DrawCircleDemo()
{

    SceneCircle circle;
    circle.header.frameId = WORLD_FRAME;
    circle.header.name = "world_cricle";
    circle.header.info = "world_circle";
    circle.center.x = 15.0f;
    circle.center.y = 15.0f;
    circle.radius = 5.0f;
    auto circle_evt1 = new events::SceneCircleEvent();
    circle_evt1->circle = std::move(circle);
    circle_evt1->optionsValid = m_firstDraw;
    m_pApp->Publish(circle_evt1);

    auto circle_evt2 = new events::SceneCircleEvent();
    circle.header.frameId = BASE_LINK;
    circle.header.name = "frame1_cricle";
    circle.header.info = "frame1_cricle";

    circle_evt2->circle = std::move(circle);
    circle_evt2->optionsValid = m_firstDraw;
    m_pApp->Publish(circle_evt2);

    auto circle_evt3 = new events::SceneCircleEvent();
    circle.header.frameId = BASE_LINK2;
    circle.header.name = "frame2_cricle";
    circle.header.info = "frame2_cricle";
    circle_evt3->circle = std::move(circle);
    circle_evt3->circle.options.color = XVIZ_BLUE;
    circle_evt3->circle.filled = true;
    circle_evt3->optionsValid = m_firstDraw;
    m_pApp->Publish(circle_evt3);
}

void Demo::DrawPoseDemo()
{
    ScenePose pose;
    pose.header.frameId = WORLD_FRAME;
    pose.header.name = "world_pose";
    pose.position.x = 10.0f;
    pose.position.y = 10.0f;
    pose.yaw = XVIZ_PI4;

    auto pose_event1 = new events::ScenePoseEvent();
    pose_event1->pose = std::move(pose);
    pose_event1->optionsValid = m_firstDraw;
    m_pApp->Publish(pose_event1);
    pose.header.name = "frame1_pose";
    pose.header.frameId = BASE_LINK;
    auto pose_event2 = new events::ScenePoseEvent();
    pose_event2->pose = std::move(pose);
    pose_event2->optionsValid = m_firstDraw;
    m_pApp->Publish(pose_event2);

    pose.header.name = "frame2_pose";
    pose.header.frameId = BASE_LINK2;
    auto pose_event3 = new events::ScenePoseEvent();
    pose_event3->pose = std::move(pose);
    pose_event3->optionsValid = m_firstDraw;
    m_pApp->Publish(pose_event3);
}

void Demo::DrawPointCloudDemo()
{
    ScenePointCloud cloud;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {

            cloud.points.emplace_back(Point{0.5f * i, 0.5f * j, 0});
        }
    }
    cloud.header.frameId = WORLD_FRAME;
    cloud.header.name = "world_pointcloud";
    auto cloud_evt1 = new events::ScenePointCloudEvent();
    cloud_evt1->pointCloud = cloud;
    m_pApp->Publish(cloud_evt1);
    cloud.header.frameId = BASE_LINK;
    cloud.header.name = "frame1_pointcloud";
    auto cloud_evt2 = new events::ScenePointCloudEvent();
    cloud_evt2->pointCloud = cloud;
    cloud_evt2->pointCloud.options.color = XVIZ_RED;
    m_pApp->Publish(cloud_evt2);
    cloud.header.frameId = BASE_LINK2;
    cloud.header.name = "frame2_pointcloud";
    auto cloud_evt3 = new events::ScenePointCloudEvent();
    cloud_evt3->pointCloud = cloud;
    cloud_evt3->pointCloud.options.color = XVIZ_BLUE;
    m_pApp->Publish(cloud_evt3);
}

void Demo::DrawPathArrayDemo()
{
    ScenePath path1, path2, path3;
    for (float i = 0; i < 5; i += 0.02f)
    {
        Point p;
        p.x = float(i);
        p.y = 2.0f * float(i);
        path1.points.emplace_back(p);
        p.y = 3.0f * float(i);
        path2.points.emplace_back(p);
        p.y = 5.0f * float(i);
        path3.points.emplace_back(p);
    }

    ScenePathArray paths;
    paths.paths.emplace_back(path1);
    paths.paths.emplace_back(path2);
    paths.paths.emplace_back(path3);

    auto paths_evt1 = new events::ScenePathArrayEvent();
    paths.header.frameId = WORLD_FRAME;
    paths.header.name = "world_paths";
    paths_evt1->paths = paths;
    m_pApp->Publish(paths_evt1);

    auto paths_evt2 = new events::ScenePathArrayEvent();
    paths.header.frameId = BASE_LINK;
    paths.header.name = "baselink_paths";
    paths_evt2->paths = paths;
    m_pApp->Publish(paths_evt2);

    auto paths_evt3 = new events::ScenePathArrayEvent();
    paths.header.frameId = BASE_LINK2;
    paths.header.name = "baselink2_paths";
    paths_evt3->paths = paths;
    m_pApp->Publish(paths_evt3);
}

void Demo::DrawPolygonArrayDemo()
{
    Point p0{1.0f, 1.0f, 0.0f}, p1{3.5f, 1.0f, 0.0f}, p2{3.5f, 3.5f, 0.0f}, p3{1.0f, 3.5f, 0.0f};
    ScenePolygon polygon1, polygon2, polygon3;
    polygon1.vertices.push_back(p0);
    polygon1.vertices.push_back(p1);
    polygon1.vertices.push_back(p2);
    polygon1.vertices.push_back(p3);
    polygon1.header.name = "polygon1";
    polygon1.header.info = "polygon1";
    p0.x += 5.0f;
    p1.x += 5.0f;
    p2.x += 5.0f;
    p3.x += 5.0f;
    polygon2.vertices.push_back(p0);
    polygon2.vertices.push_back(p1);
    polygon2.vertices.push_back(p2);
    polygon2.vertices.push_back(p3);
    polygon2.header.name = "polygon2";
    polygon2.header.info = "polygon2";
    p0.y += 5.0f;
    p1.y += 5.0f;
    p2.y += 5.0f;
    p3.y += 5.0f;
    polygon3.vertices.push_back(p0);
    polygon3.vertices.push_back(p1);
    polygon3.vertices.push_back(p2);
    polygon3.vertices.push_back(p3);
    polygon3.header.name = "polygon3";
    polygon3.header.info = "polygon3";
    polygon3.filled = true;
    polygon3.options.color = XVIZ_BLUE;
    polygon3.options.useOwnOptions = true;
    ScenePolygonArray polygons;
    polygons.polygons.push_back(polygon1);
    polygons.polygons.push_back(polygon2);
    polygons.polygons.push_back(polygon3);
    auto polygons_evt1 = new events::ScenePolygonArrayEvent();
    polygons.header.frameId = WORLD_FRAME;
    polygons.header.name = "world_polygons";
    polygons_evt1->polygons = polygons;
    polygons_evt1->optionsValid = m_firstDraw;
    m_pApp->Publish(polygons_evt1);
    auto polygons_evt2 = new events::ScenePolygonArrayEvent();
    polygons.header.frameId = BASE_LINK;
    polygons.header.name = "baselink_polygons";
    polygons_evt2->polygons = polygons;
    polygons_evt2->optionsValid = m_firstDraw;
    m_pApp->Publish(polygons_evt2);
    auto polygons_evt3 = new events::ScenePolygonArrayEvent();
    polygons.header.frameId = BASE_LINK2;
    polygons.header.name = "baselink2_polygons";
    polygons_evt3->polygons = polygons;
    polygons_evt3->optionsValid = m_firstDraw;
    m_pApp->Publish(polygons_evt3);
}

void Demo::DrawCirclehArrayDemo()
{
    SceneCircle circle1, circle2, circle3;
    circle1.center.x = 1.0f;
    circle1.center.y = 1.0f;
    circle1.radius = 1.0f;
    circle1.header.name = "circle1";
    circle1.header.info = "Circle1";
    circle2.center.x = 3.5f;
    circle2.center.y = 1.0f;
    circle2.radius = 1.0f;
    circle1.header.name = "circle2";
    circle2.header.info = "Circle2";
    circle3.center.x = 3.5f;
    circle3.center.y = 3.5f;
    circle3.radius = 1.0f;
    circle3.header.name = "circle3";
    circle3.header.info = "Circle3";
    circle3.filled = true;
    circle3.options.color = XVIZ_BROWN;
    circle3.options.useOwnOptions = true;
    SceneCircleArray circles;
    circles.circles.push_back(circle1);
    circles.circles.push_back(circle2);
    circles.circles.push_back(circle3);
    auto circles_evt1 = new events::SceneCircleArrayEvent();
    circles.header.frameId = WORLD_FRAME;
    circles.header.name = "world_circles";

    circles_evt1->circles = circles;
    circles_evt1->optionsValid = m_firstDraw;
    m_pApp->Publish(circles_evt1);
    auto circles_evt2 = new events::SceneCircleArrayEvent();
    circles.header.frameId = BASE_LINK;
    circles.header.name = "baselink_circles";
    circles_evt2->circles = circles;
    circles_evt2->optionsValid = m_firstDraw;
    m_pApp->Publish(circles_evt2);
    auto circles_evt3 = new events::SceneCircleArrayEvent();
    circles.header.frameId = BASE_LINK2;
    circles.header.name = "baselink2_circles";
    circles_evt3->circles = circles;
    circles_evt3->optionsValid = m_firstDraw;
    m_pApp->Publish(circles_evt3);
}

void Demo::DrawPoseArrayDemo()
{
    ScenePose pose1, pose2, pose3;
    pose1.position.x = 10.0f;
    pose1.position.y = 10.0f;
    pose1.yaw = XVIZ_PI4;
    pose2.position.x = 13.5f;
    pose2.position.y = 11.0f;
    pose2.yaw = XVIZ_PI4;
    pose3.position.x = 13.5f;
    pose3.position.y = 13.5f;
    pose3.yaw = XVIZ_PI4;
    pose3.options.color = XVIZ_GREEN;
    pose3.options.useOwnOptions = true;
    ScenePoseArray poses;
    poses.poses.push_back(pose1);
    poses.poses.push_back(pose2);
    poses.poses.push_back(pose3);
    auto poses_evt1 = new events::ScenePoseArrayEvent();
    poses.header.frameId = WORLD_FRAME;
    poses.header.name = "world_poses";
    poses_evt1->poses = poses;
    m_pApp->Publish(poses_evt1);
    auto poses_evt2 = new events::ScenePoseArrayEvent();
    poses.header.frameId = BASE_LINK;
    poses.header.name = "baselink_poses";
    poses_evt2->poses = poses;
    m_pApp->Publish(poses_evt2);
    auto poses_evt3 = new events::ScenePoseArrayEvent();
    poses.header.frameId = BASE_LINK2;
    poses.header.name = "baselink2_poses";
    poses_evt3->poses = poses;
    m_pApp->Publish(poses_evt3);
}

void Demo::RealtimeDoubleDataDemo()
{

    std::vector<double> xs1;
    std::vector<double> ys1;
    std::vector<double> ys2;

    for (float s = 0; s < XVIZ_2PI; s += 0.05f)
    {
        xs1.emplace_back(s);
        ys1.emplace_back(sinf(s));
        ys2.emplace_back(cosf(s));
    }

    auto double_data_evt = new events::PlotRealtimeDoubleData();
    double_data_evt->name = "sine";
    double_data_evt->xs = xs1;
    double_data_evt->ys = std::move(ys1);

    m_pApp->Publish(double_data_evt);

    auto double_data_evt2 = new events::PlotRealtimeDoubleData();
    double_data_evt2->name = "cosine";
    double_data_evt2->ys = std::move(ys2);
    m_pApp->Publish(double_data_evt2);

    std::vector<double> realtime_test_data;
    for (int i = 0; i < 10; i++)
    {
        realtime_test_data.emplace_back(i);
    }
    auto realtime_double_data_evt = new events::PlotRealtimeDoubleData();
    realtime_double_data_evt->name = "realtimeTestData";
    realtime_double_data_evt->xs = std::move(realtime_test_data);
    m_pApp->Publish(realtime_double_data_evt);
}

void Demo::OnNewEvent(Event *evt)
{
    if (typeid(*evt) == typeid(PlotRealtimeDoubleData))
    {
        auto plot_evt = static_cast<PlotRealtimeDoubleData *>(evt);
        XVIZ_WARN("reveive  PlotRealtimeDoubleData name is %s", plot_evt->name.data());
    }
    else if (typeid(*evt) == typeid(PlotTimeSeriesDoubleDataMapEvent))
    {
        auto plot_double_evt = static_cast<PlotTimeSeriesDoubleDataMapEvent *>(evt);
        XVIZ_INFO("reveive  PlotTimeSeriesDoubleDataMapEvent time is is %f", plot_double_evt->time);
    }
}
