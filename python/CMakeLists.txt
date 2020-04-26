set(wrapped_libary "QtColorWidgets")
set(bindings_library "py_color_widgets")
set(wrapped_header ${CMAKE_CURRENT_SOURCE_DIR}/bindings.hpp)
set(typesystem_file ${CMAKE_CURRENT_SOURCE_DIR}/typesystem.xml)
set(bindings_module "color_widgets")

find_package(PythonLibs 3 REQUIRED)
set(shiboken_include_dir "/usr/include/shiboken2")
set(shiboken_shared_libraries "/usr/lib/x86_64-linux-gnu/libshiboken2.cpython-38-x86_64-linux-gnu.so")

set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)

set(generated_sources
    ${CMAKE_CURRENT_BINARY_DIR}/${bindings_library}/${bindings_library}_module_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${bindings_library}/${bindings_module}_colordialog_wrapper.cpp
)

set(shiboken_options --generator-set=shiboken --enable-parent-ctor-heuristic
    --enable-return-value-heuristic --use-isnull-as-nb_nonzero
    --avoid-protected-hack
    -I${CMAKE_CURRENT_SOURCE_DIR}
    -T${CMAKE_BINARY_DIR}/env/lib/python3.8/site-packages/PySide2/typesystems/
    --output-directory=${CMAKE_CURRENT_BINARY_DIR}
    # TODO figure out how to generate the -I flags
    -I/usr/include/x86_64-linux-gnu/qt5/
    -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets/
    -I/usr/include/x86_64-linux-gnu/qt5/QtCore/
    -I${CMAKE_SOURCE_DIR}/include
)

set(generated_sources_dependencies ${wrapped_header} ${typesystem_file})

#
# # Add custom target to run shiboken to generate the binding cpp files.
add_custom_command(
    OUTPUT ${generated_sources}
    COMMAND shiboken2
    ${shiboken_options} ${wrapped_header} ${typesystem_file}
    DEPENDS ${generated_sources_dependencies}
    IMPLICIT_DEPENDS CXX ${wrapped_header}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running generator for ${typesystem_file}."
)
add_custom_target(
    "venv"
    COMMAND virtualenv env -p python3
    COMMAND ${CMAKE_BINARY_DIR}/env/bin/pip install pyside2
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_custom_target(
    "shiboken"
    COMMAND shiboken2
    ${shiboken_options} ${wrapped_header} ${typesystem_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running generator for ${typesystem_file}."
)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(/usr/include/x86_64-linux-gnu/qt5/)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtWidgets/)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtGui)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtCore/)
include_directories(${CMAKE_BINARY_DIR}/env/lib/python3.8/site-packages/PySide2/include/)
include_directories(${CMAKE_BINARY_DIR}/env/lib/python3.8/site-packages/PySide2/include/QtWidgets/)
include_directories(${CMAKE_BINARY_DIR}/env/lib/python3.8/site-packages/PySide2/include/QtGui/)
include_directories(${CMAKE_BINARY_DIR}/env/lib/python3.8/site-packages/PySide2/include/QtCore/)

set(${bindings_library}_sources ${generated_sources})
add_library(${bindings_library} MODULE ${${bindings_library}_sources})
#
# # Apply relevant include and link flags.
target_include_directories(${bindings_library} PRIVATE ${PYTHON_INCLUDE_DIRS})
target_include_directories(${bindings_library} PRIVATE ${shiboken_include_dir})
target_include_directories(${bindings_library} PRIVATE ${CMAKE_SOURCE_DIR})
#
target_link_libraries(${bindings_library} PRIVATE ${shiboken_shared_libraries})
target_link_libraries(${bindings_library} PRIVATE ${PYTHON_LIBRARIES})
target_link_libraries(${bindings_library} PRIVATE ${TARGET_NAME})
#
# # Adjust the name of generated module.
set_property(TARGET ${bindings_library} PROPERTY PREFIX "")
set_property(TARGET ${bindings_library} PROPERTY OUTPUT_NAME "${bindings_library}")
