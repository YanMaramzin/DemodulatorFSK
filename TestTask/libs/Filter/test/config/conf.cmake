set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/config)
set(DST_DIR ${TEST_OUTPUT_PATH})

add_custom_target(${PROJECT_NAME}_tconf 

)

add_dependencies(${PROJECT_NAME}_t ${PROJECT_NAME}_tconf)
