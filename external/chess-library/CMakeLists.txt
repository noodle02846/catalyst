if (NOT TARGET chess::chess)
    add_library(chess::chess IMPORTED INTERFACE GLOBAL)
    target_include_directories(chess::chess INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
endif()