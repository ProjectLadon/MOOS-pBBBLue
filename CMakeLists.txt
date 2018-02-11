#--------------------------------------------------------
# The CMakeLists.txt for:                       pBBBlue
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  BBBlue.cpp
  BBBlue_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pBBBlue ${SRC})

TARGET_LINK_LIBRARIES(pBBBlue
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

