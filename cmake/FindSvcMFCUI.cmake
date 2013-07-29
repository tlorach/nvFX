# Try to find SvcMFCUI project dll and include file
#
unset(SVCMFCUI_DLL CACHE)
unset(SVCMFCUI_INCLUDE_DIR CACHE)
unset(SVCMFCUI_FOUND CACHE)

find_path( SVCMFCUI_INCLUDE_DIR ISvcUI.h
  ${SVCMFCUI_LOCATION}/include
  $ENV{SVCMFCUI_LOCATION}/include
  ${PROJECT_SOURCE_DIR}/../SvcMFCUI/include
  ${SVCMFCUI_LOCATION}/include
  $ENV{SVCMFCUI_LOCATION}/include
  ${PROJECT_SOURCE_DIR}/../SvcMFCUI/include
  ${SVCMFCUI_LOCATION}
  $ENV{SVCMFCUI_LOCATION}
)

set( SVCMFCUI_FOUND "NO" )

if(SVCMFCUI_INCLUDE_DIR)

  if(ARCH STREQUAL "x86")
      file(GLOB SVCMFCUI_DLLS "${SVCMFCUI_INCLUDE_DIR}/../dll/SvcMFCUI_*.dll")
      list(LENGTH SVCMFCUI_DLLS NUMDLLS)
      if(NUMDLLS EQUAL 0)
        file(GLOB SVCMFCUI_DLLS "${SVCMFCUI_INCLUDE_DIR}/SvcMFCUI_*.dll")
      endif()
  else()
      file(GLOB SVCMFCUI_DLLS "${SVCMFCUI_INCLUDE_DIR}/../dll/SvcMFCUI64_*.dll")
      list(LENGTH SVCMFCUI_DLLS NUMDLLS)
      if(NUMDLLS EQUAL 0)
        file(GLOB SVCMFCUI_DLLS "${SVCMFCUI_INCLUDE_DIR}/SvcMFCUI64_*.dll")
      endif()
  endif()

  list(LENGTH SVCMFCUI_DLLS NUMDLLS)
  if(NUMDLLS EQUAL 0)
    message(WARNING "dll for the User Interface not found. Please compile SvcMFCUI" )
    set (SVCMFCUI_DLL "NOTFOUND")
  else()
    list(GET SVCMFCUI_DLLS 0 SVCMFCUI_DLL)
  endif()

    if(SVCMFCUI_DLL)
      set( SVCMFCUI_FOUND "YES" )
      set( SVCMFCUI_HEADERS "${SVCMFCUI_INCLUDE_DIR}/ISvcUI.h" "${SVCMFCUI_INCLUDE_DIR}/ISvcBase.h")
    endif(SVCMFCUI_DLL)
else(SVCMFCUI_INCLUDE_DIR)
  message(WARNING "
      SvcMFC service for UI not found. 
      You can install it from https://github.com/tlorach/SvcMFCUI )... and compile it first.
      You can also set SVCMFCUI_LOCATION in cmake or as Env. variable.
      The SvcMFCUI folder you would specify with SVCMFCUI_LOCATION should contain:
      - dll folder: containing the SvcMFCUI[64_]*.dll
      - include folder: containing the include files
      OR this folder could directly contain the dll and headers, put together
      For now, samples will run without additional UI. But that's okay ;-)"
  )
endif(SVCMFCUI_INCLUDE_DIR)
include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SVCMFCUI DEFAULT_MSG
    SVCMFCUI_INCLUDE_DIR
    SVCMFCUI_DLL
)
# I duno why I have to rewrite the variable here...
SET(SVCMFCUI_DLL ${SVCMFCUI_DLL} CACHE PATH "path")

mark_as_advanced( SVCMFCUI_FOUND )
