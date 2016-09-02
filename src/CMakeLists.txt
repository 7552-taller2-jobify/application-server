add_executable (Server main.cpp Server.cpp)

target_link_libraries (Server LINK_PUBLIC Mongoose)
