#!/bin/bash
if [ -z "${ARCHITECTURE}" ]; then
    ARCHITECTURE="x64"
fi

if [ "${ARCHITECTURE}" != "x64" ] && [ "${ARCHITECTURE}" != "x86" ]; then
    ARCHITECTURE="x64"
fi

export Platform="${ARCHITECTURE}"

readonly VS_PATH="/D/ProgramFiles/Microsoft Visual Studio/18/Community"
readonly MSVC_VERSION="14.51.36231"
readonly WIN_KITS_10="/C/Program Files (x86)/Windows Kits/10"
readonly WINSDK_VERSION="10.0.26100.0"

if [ "${ARCHITECTURE}" == "x64" ]; then
    readonly FRAMEWORK_DIR="/C/Windows/Microsoft.NET/Framework64/v4.0.30319"
else
    readonly FRAMEWORK_DIR="/C/Windows/Microsoft.NET/Framework/v4.0.30319"
fi

########## PATH ##########

readonly PATH_ARRAY=(
    "${VS_PATH}/Common7/IDE/"
    # "${VS_PATH}/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin"
    "${VS_PATH}/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja"
    "${VS_PATH}/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer"
    "${VS_PATH}/Common7/IDE/CommonExtensions/Microsoft/TestWindow"
    "${VS_PATH}/Common7/IDE/VC/Linux/bin/ConnectionManagerExe"
    "${VS_PATH}/Common7/IDE/VC/VCPackages"
    "${VS_PATH}/Common7/Tools/"
    "${VS_PATH}/MSBuild/Current/Bin/amd64"
    "${VS_PATH}/MSBuild/Current/bin/Roslyn"
    "${VS_PATH}/Team Tools/DiagnosticsHub/Collector"
    "${VS_PATH}/VC/vcpkg"
    "${FRAMEWORK_DIR}"
    "${VS_PATH}/VC/Tools/MSVC/${MSVC_VERSION}/bin/Host${ARCHITECTURE}/${ARCHITECTURE}"
    "${WIN_KITS_10}/bin/${WINSDK_VERSION}/${ARCHITECTURE}"
    "${WIN_KITS_10}/bin/${ARCHITECTURE}"
)

for ELEMENT in "${PATH_ARRAY[@]}"; do
    export PATH="${ELEMENT}:${PATH}"
done

########## INCLUDE ##########

readonly INCLUDE_ARRAY=(
    "${VS_PATH}/VC/Tools/MSVC/${MSVC_VERSION}/include"
    "${VS_PATH}/VC/Tools/MSVC/${MSVC_VERSION}/ATLMFC/include"
    "${VS_PATH}/VC/Auxiliary/VS/include"
    "${WIN_KITS_10}/include/${WINSDK_VERSION}/ucrt"
    "${WIN_KITS_10}/include/${WINSDK_VERSION}/um"
    "${WIN_KITS_10}/include/${WINSDK_VERSION}/shared"
    "${WIN_KITS_10}/include/${WINSDK_VERSION}/winrt"
    "${WIN_KITS_10}/include/${WINSDK_VERSION}/cppwinrt"
)

for ELEMENT in "${INCLUDE_ARRAY[@]}"; do
    export INCLUDE="${ELEMENT}:${INCLUDE}"
done

########## LIB ####

readonly LIB_ARRAY=(
    "${VS_PATH}/VC/Tools/MSVC/${MSVC_VERSION}/ATLMFC/lib/${ARCHITECTURE}"
    "${VS_PATH}/VC/Tools/MSVC/${MSVC_VERSION}/lib/${ARCHITECTURE}"
    "${WIN_KITS_10}/lib/${WINSDK_VERSION}/ucrt/${ARCHITECTURE}"
    "${WIN_KITS_10}/lib/${WINSDK_VERSION}/um/${ARCHITECTURE}"
)

for ELEMENT in "${LIB_ARRAY[@]}"; do
    export LIB="${ELEMENT}:${LIB}"
done

########## LIBPATH ##########

readonly LIBPATH_ARRAY=(
    "${VS_PATH}/VC/Tools/MSVC/${MSVC_VERSION}/lib/x86/store/references"
    "${WIN_KITS_10}/References/${WINSDK_VERSION}"
    "${WIN_KITS_10}/UnionMetadata/${WINSDK_VERSION}"
    "${FRAMEWORK_DIR}"
    "${VS_PATH}/VC/Tools/MSVC/${MSVC_VERSION}/ATLMFC/lib/${ARCHITECTURE}"
    "${VS_PATH}/VC/Tools/MSVC/${MSVC_VERSION}/lib/${ARCHITECTURE}"
)

for ELEMENT in "${LIBPATH_ARRAY[@]}"; do
    export LIBPATH="${ELEMENT}:${LIBPATH}"
done
