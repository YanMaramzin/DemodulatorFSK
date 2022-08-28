cmake_minimum_required(VERSION 3.0)
if(UNITTEST AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
	if(NOT TEST_OUTPUT_PATH)
		set(TEST_OUTPUT_PATH ${CMAKE_BINARY_DIR}/test)
	endif()

	file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h)

	add_executable(${PROJECT_NAME}_t ${SRC})

	target_link_libraries(${PROJECT_NAME}_t ${PROJECT_NAME} gtest)

	set_target_properties(${PROJECT_NAME}_t PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_PATH})
	add_test(NAME ${PROJECT_NAME}_t COMMAND ${PROJECT_NAME}_t WORKING_DIRECTORY ${TEST_OUTPUT_PATH})

	if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/config/conf.cmake)
		include(${CMAKE_CURRENT_SOURCE_DIR}/test/config/conf.cmake)
	endif()
endif()
