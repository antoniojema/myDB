file(GLOB
    HeaderList
    "*.h"
)

file(GLOB
    SRCList
    "*.cpp"
)

add_library(
    "common"
    ${HeaderList}
    ${SRCList}
    "CMakeLists.txt"
)

source_group("/" FILES ${SRCList} ${HeaderList})
source_group("/CMakeLists/" FILES "CMakeLists.txt")
