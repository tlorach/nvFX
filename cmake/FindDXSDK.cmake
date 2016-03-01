

# Try to find DirectX SDK.
# Once done this will define
#
# DXSDK_FOUND
# DXSDK_INCLUDE_DIR
# DXSDK_LIBRARY_DIR
# DXSDK_LIBRARIES
# DXSDK_LOCATION
#
# Also will define

if (WIN32)
    find_path(DXSDK_INCLUDE_DIR
        NAMES
            D3D11.h D3Dcompiler.h
        PATHS
            ${DXSDK_LOCATION}/Include
            $ENV{DXSDK_LOCATION}/Include
            ${DXSDK_ROOT}/Include
            $ENV{DXSDK_ROOT}/Include
            "C:/Program Files (x86)/Microsoft DirectX SDK*/Include"
            "C:/Program Files/Microsoft DirectX SDK*/Include"
    )

    find_path(DXSDK_LIBRARY_DIR
            d3d11.lib
        PATHS
            ${DXSDK_LOCATION}/Lib/x64
            $ENV{DXSDK_LOCATION}/Lib/x64
            ${DXSDK_ROOT}/Lib/x64
            $ENV{DXSDK_ROOT}/Lib/x64
            "C:/Program Files (x86)/Microsoft DirectX SDK*/Lib/x64"
            "C:/Program Files/Microsoft DirectX SDK*/Lib/x64"
    )

	## d3dx11 gxguid - remove these two libraries
    foreach(DX_LIB d3d11 d3dcompiler d3dx11 dxguid)

        find_library(DXSDK_${DX_LIB}_LIBRARY
            NAMES 
                ${DX_LIB}.lib
            PATHS
                ${DXSDK_LIBRARY_DIR}
        )

        list(APPEND DXSDK_LIBRARIES ${DXSDK_${DX_LIB}_LIBRARY})


    endforeach(DX_LIB)

endif ()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(DXSDK DEFAULT_MSG
    DXSDK_INCLUDE_DIR
    DXSDK_LIBRARY_DIR
    DXSDK_LIBRARIES
)

mark_as_advanced(
    DXSDK_INCLUDE_DIR
    DXSDK_LIBRARY_DIR
    DXSDK_LIBRARIES
)

