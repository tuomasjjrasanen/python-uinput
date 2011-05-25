FIND_PATH(libudev_INCLUDE_DIR libudev.h)

FIND_LIBRARY(libudev_LIBRARY NAMES udev)

SET(libudev_LIBRARIES ${libudev_LIBRARY})
SET(libudev_INCLUDE_DIRS ${libudev_INCLUDE_DIR})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libudev  DEFAULT_MSG
                                  libudev_LIBRARY libudev_INCLUDE_DIR)

MARK_AS_ADVANCED(libudev_INCLUDE_DIR libudev_LIBRARY)
