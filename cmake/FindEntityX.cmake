find_path(ENTITYX_INCLUDE_DIR entityx/Entityx.h
   PATHS
   /usr/local/include
   ${ENTITYX_ROOT}
)

find_library(ENTITYX_LIBRARY
   NAMES entityx libentityx
   HINTS
   /usr/local/lib
   ${ENTITYX_ROOT}/lib
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(EntityX  DEFAULT_MSG
                                  ENTITYX_LIBRARY ENTITYX_INCLUDE_DIR)
