add_executable (Server main.cpp Server.cpp Sample.cpp DataBase.cpp RequestAdministrator.cpp)

target_link_libraries (Server LINK_PUBLIC Mongoose leveldb pthread)

add_library (Sample Sample.cpp DataBase.cpp)

target_include_directories(Sample PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
