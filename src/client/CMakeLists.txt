file(GLOB
    HeaderList
    "*.h"
)

file(GLOB
    SRCList
    "*.cpp"
)

set(TargetName "Client")

add_executable(
    ${TargetName}
    ${HeaderList}
    ${SRCList}
    "CMakeLists.txt"
)

include_directories(${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${TargetName} "common")

source_group("/" FILES ${SRCList} ${HeaderList})
source_group("/CMakeLists/" FILES "CMakeLists.txt")