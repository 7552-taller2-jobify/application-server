file(GLOB Clases
	"*.h"
	"*.cpp"
)

add_executable (Server ${Clases})

target_link_libraries (Server LINK_PUBLIC Mongoose leveldb pthread curl)

add_library (DataBase DataBase.cpp)

target_include_directories(DataBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
