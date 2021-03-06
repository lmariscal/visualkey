set(VISUALKEY_SOURCES
  ${VISUALKEY_SOURCES}
  visualkey.cc
  window.cc
  mono_script.cc
  iomanager.cc
  mesh.cc
  shader.cc
  image.cc
  audio.cc
  utils.cc
  light.cc
  text.cc
)

if(NOT DEFINED OpenGL_GL_PREFERENCE)
  set(OpenGL_GL_PREFERENCE "GLVND")
endif()

find_package(OpenGL REQUIRED)

if(NOT MONO_ROOT)
  if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(MONO_ROOT "C:/Program Files/Mono")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
      set(MONO_ROOT "C:/Program Files (x86)/Mono")
    endif()
  endif()
endif()

if(NOT MONO_INCLUDE)
  if(WIN32)
    set(MONO_INCLUDE ${MONO_ROOT}/include/mono-2.0)
  elseif(LINUX)
    set(MONO_INCLUDE "/usr/include/mono-2.0")
  elseif(APPLE)
  endif()
endif()

if(LINUX)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    link_directories(${CMAKE_SOURCE_DIR}/third/irrklang/bin/linux-gcc-64)
    file(GLOB IRRKLANG_DLLS LIST_DIRECTORIES false "${CMAKE_SOURCE_DIR}/third/irrklang/bin/linux-gcc-64/*.so")
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    link_directories(${CMAKE_SOURCE_DIR}/third/irrklang/bin/linux-gcc)
    file(GLOB IRRKLANG_DLLS LIST_DIRECTORIES false "${CMAKE_SOURCE_DIR}/third/irrklang/bin/linux-gcc/*.so")
  endif()
elseif(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    link_directories(${CMAKE_SOURCE_DIR}/third/irrklang/lib/Winx64-visualStudio)
    file(GLOB IRRKLANG_DLLS LIST_DIRECTORIES false "${CMAKE_SOURCE_DIR}/third/irrklang/bin/winx64-visualStudio/*.dll")
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    link_directories(${CMAKE_SOURCE_DIR}/third/irrklang/lib/Win32-visualStudio)
    file(GLOB IRRKLANG_DLLS LIST_DIRECTORIES false "${CMAKE_SOURCE_DIR}/third/irrklang/bin/win32-visualStudio/*.dll")
  endif()
endif()

if(MONO_LINK)
  link_directories(${MONO_LINK})
else()
  if(WIN32)
    link_directories(${MONO_ROOT}/lib)
  endif()
endif()

if(NOT MONO_LIBS)
  if(WIN32)
    set(MONO_LIBS "mono-2.0-sgen")
  elseif(LINUX)
    set(MONO_LIBS "monosgen-2.0")
  elseif(APPLE)
  endif()
endif()

set(ENV{MONO_PATH} ${CMAKE_BINARY_DIR}/dist)
message("MONO_PATH $ENV{MONO_PATH}")

add_executable(visualkey ${VISUALKEY_SOURCES})

set_target_properties(visualkey PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)

foreach(dll_i ${IRRKLANG_DLLS})
  file(COPY "${dll_i}" DESTINATION "${CMAKE_BINARY_DIR}/dist/bin")
endforeach()

set_target_properties(visualkey PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)
set_target_properties(visualkey PROPERTIES OUTPUT_NAME visualkey)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(visualkey PRIVATE VISUALKEY_DEBUG=1)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  target_compile_definitions(visualkey PRIVATE VISUALKEY_64=1)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
  target_compile_definitions(visualkey PRIVATE VISUALKEY_32=1)
endif()

if(LINUX)
  target_compile_definitions(visualkey PRIVATE _REENTRANT=1)
endif()

target_link_libraries(visualkey PRIVATE glfw)
target_link_libraries(visualkey PRIVATE glm)
target_link_libraries(visualkey PRIVATE freetype)
target_link_libraries(visualkey PRIVATE IrrKlang)
target_link_libraries(visualkey PRIVATE OpenGL::GL)
target_link_libraries(visualkey PRIVATE ${MONO_LIBS})

target_include_directories(visualkey PRIVATE ${CMAKE_SOURCE_DIR}/third/glad)
target_include_directories(visualkey PRIVATE ${CMAKE_SOURCE_DIR}/third/stb)
target_include_directories(visualkey PRIVATE ${CMAKE_SOURCE_DIR}/third/irrklang/include)
target_include_directories(visualkey PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(visualkey PRIVATE ${MONO_INCLUDE})

# C#

file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/dist/mono)
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/dist/lib)
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/dist/etc)

# Copy bin (Windows)

if(WIN32)
  file(COPY "${MONO_ROOT}/bin" DESTINATION "${CMAKE_BINARY_DIR}")
  file(RENAME "${CMAKE_BINARY_DIR}/bin" "${CMAKE_BINARY_DIR}/dist/mono")
endif()

# Compile C# api

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/api)
file(GLOB_RECURSE VISUALKEY_CS_SOURCES LIST_DIRECTORIES false "${CMAKE_SOURCE_DIR}/csharp/*.cs")
if(WIN32)
  set(MCS "${CMAKE_BINARY_DIR}/dist/mono/mcs")
else()
  set(MCS "mcs")
endif()
message("MCS ${MCS}")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/api/Debug)
add_custom_target(visualkey_cs_debug ALL DEPENDS ${CMAKE_BINARY_DIR}/dist/api/Debug/VisualKey.dll)
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/dist/api/Debug/VisualKey.dll;${CMAKE_BINARY_DIR}/dist/api/Debug/VisualKey.dll.mdb;${CMAKE_BINARY_DIR}/dist/api/Debug/VisualKey.xml
  COMMAND ${MCS} ${VISUALKEY_CS_SOURCES} /target:library /debug /doc:${CMAKE_BINARY_DIR}/dist/api/Debug/VisualKey.xml /out:${CMAKE_BINARY_DIR}/dist/api/Debug/VisualKey.dll /nologo /w:3
  DEPENDS ${VISUALKEY_CS_SOURCES}
  VERBATIM
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/api/Release)
add_custom_target(visualkey_cs_release ALL DEPENDS ${CMAKE_BINARY_DIR}/dist/api/Release/VisualKey.dll)
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/dist/api/Release/VisualKey.dll;${CMAKE_BINARY_DIR}/dist/api/Release/VisualKey.dll.mdb;${CMAKE_BINARY_DIR}/dist/api/Release/VisualKey.xml
  COMMAND ${MCS} ${VISUALKEY_CS_SOURCES} /target:library /debug /doc:${CMAKE_BINARY_DIR}/dist/api/Release/VisualKey.xml /optimize /out:${CMAKE_BINARY_DIR}/dist/api/Release/VisualKey.dll /nologo /w:4
  DEPENDS ${VISUALKEY_CS_SOURCES}
  VERBATIM
)

add_dependencies(visualkey visualkey_cs_debug)
add_dependencies(visualkey visualkey_cs_release)

# etc/

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/etc)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/etc/fonts)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/etc/mono)

file(COPY "${CMAKE_SOURCE_DIR}/third/jetbrains_mono/JetBrainsMono-Regular.ttf" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/fonts")
file(COPY "${CMAKE_SOURCE_DIR}/third/jetbrains_mono/OFL.txt" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/fonts")
file(COPY "${CMAKE_SOURCE_DIR}/third/jetbrains_mono/AUTHORS.txt" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/fonts")

if(NOT MONO_ETC)
  if(WIN32)
    set(MONO_ETC "${MONO_ROOT}/etc/mono")
  elseif(LINUX)
    if(EXISTS "/etc/mono")
      set(MONO_ETC "/etc/mono")
    elseif(EXISTS "/usr/local/etc/mono")
      set(MONO_ETC "/usr/local/etc/mono")
    else()
      message(FATAL_ERROR "Failed to locate installed mono etc dir")
    endif()
  endif()
  message("MONO_ETC ${MONO_ETC}")
endif()

if(WIN32)
  file(COPY "${MONO_ETC}/2.0" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono")
  file(COPY "${MONO_ETC}/4.0" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono")
  file(COPY "${MONO_ETC}/4.5" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono")
  if(EXISTS "${MONO_ETC}/mconfig")
    file(COPY "${MONO_ETC}/mconfig" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono")
  endif()

  file(GLOB MONO_ETC_FILES LIST_DIRECTORIES false "${MONO_ETC}/*")
  foreach(file_i ${MONO_ETC_FILES})
    file(COPY "${file_i}" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono")
  endforeach()
else()
  file(COPY "${MONO_ETC}/2.0" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono" FOLLOW_SYMLINK_CHAIN)
  file(COPY "${MONO_ETC}/4.0" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono" FOLLOW_SYMLINK_CHAIN)
  file(COPY "${MONO_ETC}/4.5" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono" FOLLOW_SYMLINK_CHAIN)
  if(EXISTS "${MONO_ETC}/mconfig")
    file(COPY "${MONO_ETC}/mconfig" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono" FOLLOW_SYMLINK_CHAIN)
  endif()

  file(GLOB MONO_ETC_FILES LIST_DIRECTORIES false "${MONO_ETC}/*")
  foreach(file_i ${MONO_ETC_FILES})
    file(COPY "${file_i}" DESTINATION "${CMAKE_BINARY_DIR}/dist/etc/mono" FOLLOW_SYMLINK_CHAIN)
  endforeach()
endif()

# lib/

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib)

if(NOT MONO_LIB)
  if(WIN32)
    set(MONO_LIB "${MONO_ROOT}/lib")
  elseif(LINUX)
    if(EXISTS "/lib/mono")
      set(MONO_LIB "/lib")
    elseif(EXISTS "/usr/lib/mono")
      set(MONO_ETC "/usr/lib")
    else()
      message(FATAL_ERROR "Failed to locate installed mono lib dir")
    endif()
  endif()
  message("MONO_LIB ${MONO_LIB}")
endif()

if(WIN32)
  file(GLOB MONO_DLLS LIST_DIRECTORIES false "${CMAKE_BINARY_DIR}/dist/mono/*.dll")
  foreach(dll_i ${MONO_DLLS})
    file(COPY "${dll_i}" DESTINATION "${CMAKE_BINARY_DIR}/dist/bin")
  endforeach()
else()
  set(MONO_LIB_LIST "libmono-btls-shared;libmono-ee-interp;libmono-native;libMonoPosixHelper;libmono-profiler-aot;libmono-profiler-coverage;libmono-profiler-log;libMonoSupportW;libmono-native-compat")
  set(MONO_LIB_FILES "")
  if(APPLE)
    set(MONO_LIB_EXT "dylib")
  elseif(LINUX)
    set(MONO_LIB_EXT "so")
  endif()

  foreach(lib_i ${MONO_LIB_LIST})
    if(NOT EXISTS "${MONO_LIB}/${lib_i}.${MONO_LIB_EXT}")
      message("${lib_i}.${MONO_LIB_EXT} not found")
      continue()
    endif()
    if(NOT MONO_LIB_FILES STREQUAL "")
      set(MONO_LIB_FILES "${MONO_LIB_FILES};")
    endif()
    set(MONO_LIB_FILES "${MONO_LIB_FILES}${MONO_LIB}/${lib_i}.${MONO_LIB_EXT}")
  endforeach()

  foreach(lib_i ${MONO_LIB_FILES})
    file(COPY "${lib_i}" DESTINATION "${CMAKE_BINARY_DIR}/dist/lib" FOLLOW_SYMLINK_CHAIN)
  endforeach()
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib/mono/4.5)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib/mono/4.5)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib/mono/4.5/Facades)

set(MONO_LIB_FILES "")
if(WIN32)
  file(GLOB MONO_LIB_FILES LIST_DIRECTORIES false "${MONO_LIB}/mono/4.5/*")
else()
  file(GLOB MONO_LIB_FILES LIST_DIRECTORIES false "${MONO_LIB}/mono/4.5/*.dll")
endif()
foreach(lib_i ${MONO_LIB_FILES})
  if(WIN32)
    file(COPY "${lib_i}" DESTINATION "${CMAKE_BINARY_DIR}/dist/lib/mono/4.5")
  else()
    file(COPY "${lib_i}" DESTINATION "${CMAKE_BINARY_DIR}/dist/lib/mono/4.5" FOLLOW_SYMLINK_CHAIN)
  endif()
endforeach()

set(MONO_LIB_FILES "")
file(GLOB MONO_LIB_FILES LIST_DIRECTORIES false "${MONO_LIB}/mono/4.5/Facades/*.dll")
foreach(lib_i ${MONO_LIB_FILES})
  if(WIN32)
    file(COPY "${lib_i}" DESTINATION "${CMAKE_BINARY_DIR}/dist/lib/mono/4.5/Facades")
  else()
    file(COPY "${lib_i}" DESTINATION "${CMAKE_BINARY_DIR}/dist/lib/mono/4.5/Facades" FOLLOW_SYMLINK_CHAIN)
  endif()
endforeach()

# Template Copy

file(COPY ${CMAKE_SOURCE_DIR}/template DESTINATION ${CMAKE_BINARY_DIR}/dist)

# CPack

macro(install_symlink filepath sympath)
  install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${sympath})")
  install(CODE "message(\"-- Created symlink: ${sympath} -> ${filepath}\")")
endmacro(install_symlink)

macro(windows_install dir)
  set(DEPLOY_FILES_AND_DIRS "")
  set(DIRS_TO_DEPLOY "")
  set(FILES_TO_DEPLOY "")

  file(GLOB DEPLOY_FILES_AND_DIRS "${CMAKE_BINARY_DIR}/dist/${dir}/*")
  foreach(ITEM ${DEPLOY_FILES_AND_DIRS})
    if(IS_DIRECTORY "${ITEM}")
      list(APPEND DIRS_TO_DEPLOY "${ITEM}")
    else()
      list(APPEND FILES_TO_DEPLOY "${ITEM}")
    endif()
  endforeach()

  install(FILES ${FILES_TO_DEPLOY} DESTINATION ${dir})
  install(DIRECTORY ${DIRS_TO_DEPLOY} DESTINATION ${dir})
endmacro(windows_install)

if(LINUX)
  install(DIRECTORY ${CMAKE_BINARY_DIR}/dist/api DESTINATION etc/visualkey)
  install(DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin DESTINATION etc/visualkey)
  install(DIRECTORY ${CMAKE_BINARY_DIR}/dist/etc DESTINATION etc/visualkey)
  install(DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib DESTINATION etc/visualkey)
  install(DIRECTORY ${CMAKE_BINARY_DIR}/dist/template DESTINATION etc/visualkey)
  install_symlink(${CMAKE_INSTALL_PREFIX}/etc/visualkey/bin/visualkey ${CMAKE_INSTALL_PREFIX}/bin/visualkey)
elseif(WIN32)
  windows_install(api)
  windows_install(bin)
  windows_install(etc)
  windows_install(lib)
  windows_install(mono)
  windows_install(template)
  install(TARGETS visualkey DESTINATION bin)
endif()

include(InstallRequiredSystemLibraries)

SET(CPACK_OUTPUT_FILE_PREFIX pkg)
if(LINUX)
  set(CPACK_GENERATOR "TGZ;DEB;RPM")
elseif(WIN32)
  set(CPACK_GENERATOR "NSIS")
endif()
set(CPACK_DEBIAN_PACKAGE_DEPENDS "mono-complete")
set(CPACK_PACKAGE_CONTACT "Leonardo Mariscal <leo@ldmd.mx>")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_NSIS_MODIFY_PATH ON)

include(CPack)
