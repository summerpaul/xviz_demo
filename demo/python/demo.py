# -*- coding: utf-8 -*-
# @Author: Xia Yunkai
# @Date:   2024-12-11 19:37:38
# @Last Modified by:   Xia Yunkai
# @Last Modified time: 2024-12-12 16:10:29


from sys import path
import time
import threading
import math

path.append("E:/my_project/xviz_demo/bin")
import PyXviz
step = 3.1415926 * 0.01
# 创建可视化程序
app = PyXviz.App()
app.Init()
start_time = time.time()
def draw_thread():
    while app.IsRunning():
        scene_path = PyXviz.ScenePath()
        scene_path.header.name = "scene_path"
        scene_path.header.info = "scene_path"

        pt = PyXviz.Point()

        for i in range(1, 10): 
            pt.x = i * 2
            pt.y = i * 3
            scene_path.append(pt)


        app.AddPath(scene_path,False);

        scene_polygon = PyXviz.ScenePolygon()
        scene_polygon.header.name = "scene_polygon"
        scene_polygon.header.info = "scene_polygon"

        scene_polygon.append(PyXviz.Point(0, 0,0))
        scene_polygon.append(PyXviz.Point(1, 0,0))
        scene_polygon.append(PyXviz.Point(1, 1,0))
        scene_polygon.append(PyXviz.Point(0, 1,0))
        scene_polygon.options.color = PyXviz.Color(255,0,0,255)
        scene_polygon.filled = True

        app.AddPolygon(scene_polygon,True)
        # # 随机生成0到10的数字
        # random_num = int(time.time() * 1000) % 10

        # doubleData = PyXviz.DoubleValueMap()
        # doubleData.insert("a",random_num)
        # doubleData.insert("b",2* random_num)
        
        # app.AddTimeSeriesDoubleData(time.time() - start_time,doubleData)
        
        # stringData = PyXviz.MapStringDouble()
        # stringData.insert("haha","haha")
        # stringData.insert("hehe","hehe")
        # stringData.insert("hihi","hihi")
        # app.AddStringData(stringData)
        # sin_a =  PyXviz.DoubleVector()
        # cos_a =  PyXviz.DoubleVector()
        

        # for i in range(100):
        #     sin_a.append(i)
        #     # cos_a.append(math.cos(i * step))
        # app.AddRealtimeData("a",sin_a, cos_a)
        time.sleep(0.1)

# 启动draw_thread线程
thread = threading.Thread(target=draw_thread)
thread.start()

# 渲染线程
while app.IsRunning():
    app.Update()
print("stop")