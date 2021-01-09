configure_file(definitions.h.in ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h)

add_executable(${PROJECT_NAME}
   main.cpp
   CNF.cpp
   Clause.cpp
   Logger.cpp
   Utility.cpp
   Model.cpp
   CommandLineParser.cpp
   solvers/Solver.cpp
   solvers/clasp.cpp
   procedures/Procedure.cpp
   procedures/BackboneSimplification.cpp
   procedures/BipartitionAndElimination.cpp
   procedures/BooleanConstraintPropagation.cpp
   procedures/OccurrenceSimplification.cpp
   procedures/Vivification.cpp
)

target_link_libraries(${PROJECT_NAME}
   pthread
)

install(TARGETS ${PROJECT_NAME}
   RUNTIME DESTINATION bin
)
