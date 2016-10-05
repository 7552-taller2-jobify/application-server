add_executable (Server main.cpp Server.cpp DataBase.cpp RequestAdministrator.cpp Logger.cpp URLMapper.cpp Attendant.cpp AttendantsHandler.cpp Profile.cpp ProfileHandler.cpp RegexMatcher.cpp Request.cpp)

target_link_libraries (Server LINK_PUBLIC Mongoose leveldb pthread curl)

add_library (Sources main.cpp Server.cpp DataBase.cpp RequestAdministrator.cpp Logger.cpp URLMapper.cpp Attendant.cpp AttendantsHandler.cpp Profile.cpp ProfileHandler.cpp RegexMatcher.cpp)

target_link_libraries (Sources LINK_PUBLIC Mongoose leveldb pthread curl)

target_include_directories(Sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
