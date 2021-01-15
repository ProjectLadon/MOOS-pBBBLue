#--------------------------------------------------------
# The CMakeLists.txt for:                       pBBBlue
# Author(s):
#--------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(XXD /usr/bin/xxd)
SET(SUDO /usr/bin/sudo)
SET(CHOWN /bin/chown)
SET(CHMOD /bin/chmod)

# List of schema files
set(JSON_SCHEMA_FILES
${CMAKE_CURRENT_SOURCE_DIR}/schema/configuration_schema.json
${CMAKE_CURRENT_SOURCE_DIR}/schema/imu_quaternion_schema.json
${CMAKE_CURRENT_SOURCE_DIR}/schema/imu_tuple_schema.json
${CMAKE_CURRENT_SOURCE_DIR}/schema/motor_input_schema.json)

# List of schema files
set(JSON_HEADER_FILES
${CMAKE_CURRENT_SOURCE_DIR}/schema/configuration_schema.h
${CMAKE_CURRENT_SOURCE_DIR}/schema/imu_quaternion_schema.h
${CMAKE_CURRENT_SOURCE_DIR}/schema/imu_tuple_schema.h
${CMAKE_CURRENT_SOURCE_DIR}/schema/motor_input_schema.h)

FOREACH(LOOP_SCHEMA ${JSON_SCHEMA_FILES})
	get_filename_component(LOOP_DIR ${LOOP_SCHEMA} DIRECTORY)
	get_filename_component(LOOP_FILE_WE ${LOOP_SCHEMA} NAME_WE)
	SET(LOOP_HEADER ${LOOP_DIR}/${LOOP_FILE_WE}.h)
	add_custom_command(
		OUTPUT ${LOOP_HEADER}
		COMMAND	cd ${LOOP_DIR} && ${XXD} -i ${LOOP_FILE_WE}.json ${LOOP_FILE_WE}.h
		DEPENDS ${LOOP_SCHEMA})
ENDFOREACH(LOOP_SCHEMA)

SET(SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/BBBlue.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BBBlue_Info.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/GPIO.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Outputs.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/PWM.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Servo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/IMU.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Inputs.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Configuration.cpp
  "${JSON_HEADER_FILES}"
)

ADD_EXECUTABLE(pBBBlue ${SRC})

TARGET_LINK_LIBRARIES(pBBBlue
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   roboticscape
   pthread)

ADD_CUSTOM_TARGET(bbblue_permissions ALL
    ${SUDO} ${CHOWN} root:root ${EXECUTABLE_OUTPUT_PATH}/pBBBlue
COMMAND ${SUDO} ${CHMOD} u+s ${EXECUTABLE_OUTPUT_PATH}/pBBBlue)

find_program(CTAGS ctags)
if (CTAGS)
	FIND_FILE(MAKE_CTAGS make_ctags.sh ../..)
    ADD_CUSTOM_TARGET(bbblue_ctags ALL
        ${MAKE_CTAGS} .
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS pBBBlue
    )
endif(CTAGS)
