add_executable (Server main.cpp Server.cpp DataBase.cpp RequestAdministrator.cpp Logger.cpp URLMapper.cpp Attendant.cpp AttendantsHandler.cpp Profile.cpp ProfileHandler.cpp RegexMatcher.cpp)

target_link_libraries (Server LINK_PUBLIC Mongoose leveldb pthread curl)

add_library (DataBase DataBase.cpp)

target_include_directories(DataBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
