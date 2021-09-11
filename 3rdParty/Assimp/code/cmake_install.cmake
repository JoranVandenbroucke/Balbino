# Install script for directory: D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/lib/Debug/assimp-vc142-mtd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/lib/Release/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/lib/MinSizeRel/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/lib/RelWithDebInfo/assimp-vc142-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/bin/Debug/assimp-vc142-mtd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/bin/Release/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/bin/MinSizeRel/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/bin/RelWithDebInfo/assimp-vc142-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/anim.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/aabb.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/ai_assert.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/camera.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/color4.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/color4.inl"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/config.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/defs.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Defines.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/cfileio.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/light.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/material.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/material.inl"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/matrix3x3.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/matrix3x3.inl"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/matrix4x4.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/matrix4x4.inl"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/mesh.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/pbrmaterial.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/postprocess.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/quaternion.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/quaternion.inl"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/scene.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/metadata.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/texture.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/types.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/vector2.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/vector2.inl"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/vector3.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/vector3.inl"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/version.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/cimport.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/importerdesc.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Importer.hpp"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/DefaultLogger.hpp"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/ProgressHandler.hpp"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/IOStream.hpp"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/IOSystem.hpp"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Logger.hpp"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/LogStream.hpp"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/NullLogger.hpp"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/cexport.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Exporter.hpp"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/DefaultIOStream.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/DefaultIOSystem.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/SceneCombiner.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/fast_atof.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/qnan.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/BaseImporter.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Hash.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/MemoryIOWrapper.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/ParsingUtils.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/StreamReader.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/StreamWriter.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/StringComparison.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/StringUtils.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/SGSpatialSort.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/GenericProperty.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/SpatialSort.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/SmoothingGroups.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/SmoothingGroups.inl"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/StandardShapes.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/RemoveComments.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Subdivision.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Vertex.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/LineSplitter.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/TinyFormatter.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Profiler.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/LogAux.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Bitmap.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/XMLTools.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/IOStreamBuffer.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/CreateAnimMesh.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/irrXMLWrapper.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/BlobIOSystem.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/MathFunctions.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Macros.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Exceptional.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Compiler/pushpack1.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Compiler/poppack1.h"
    "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/Debug/assimp-vc142-mtd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/joran/Documents/GitHub/Balbino/3rdParty/Assimp/code/RelWithDebInfo/assimp-vc142-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

