add_executable (Server main.cpp Server.cpp DataBase.cpp RequestAdministrator.cpp Logger.cpp Profile.cpp)

target_link_libraries (Server LINK_PUBLIC Mongoose leveldb pthread)

add_library (DataBase DataBase.cpp)

target_include_directories(DataBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
