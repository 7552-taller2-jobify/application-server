add_library (Mongoose mongoose.c)

target_include_directories (Mongoose PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
