cmake_minimum_required(VERSION 3.17)
project(EvenDerech3_ex5)


set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread" )

add_executable(EvenDerech3_ex5 main.cpp MainTrain_ex6.cpp anomaly_detection_util.cpp timeseries.cpp SimpleAnomalyDetector.cpp minCircle.cpp HybridAnomalyDetector.cpp CLI.cpp Server.cpp)