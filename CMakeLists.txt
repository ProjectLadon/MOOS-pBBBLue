#--------------------------------------------------------
# The CMakeLists.txt for:                       pBBBlue
# Author(s):
#--------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(XXD
  /usr/bin/xxd)

# List of schema files
set(JSON_SCHEMA_FILES
${CMAKE_CURRENT_SOURCE_DIR}/schema/mixer_schema.json)

# List of schema files
set(JSON_HEADER_FILES
${CMAKE_CURRENT_SOURCE_DIR}/schema/mixer_schema.h)

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
  BBBlue.cpp
  BBBlue_Info.cpp
  main.cpp
  GPIO.cpp
  Outputs.cpp
  PWM.cpp
  Servo.cpp
  IMU.cpp
  Inputs.cpp
  Configuration.cpp
  "${JSON_HEADER_FILES}"
)

ADD_EXECUTABLE(pBBBlue ${SRC})

TARGET_LINK_LIBRARIES(pBBBlue
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)
