add_executable(${PROJECT_NAME}
   main.cpp
   CNF.cpp
   Clause.cpp
   Logger.cpp
   Utility.cpp
   Model.cpp
   solvers/Solver.cpp
   solvers/clasp.cpp
)

target_compile_options(${PROJECT_NAME}
   PRIVATE
      -Wall
      -Wpedantic
      -Wextra
      -O3
)

target_link_libraries(${PROJECT_NAME}
   pthread
)

install(TARGETS ${PROJECT_NAME}
   RUNTIME DESTINATION bin
)
