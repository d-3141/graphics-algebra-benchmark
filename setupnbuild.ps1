$setupOutput = &./setup.ps1
$toolchainOption = $setupOutput -match '-DCMAKE_TOOLCHAIN_FILE=.*vcpkg.cmake'
$toolchainOptionAndPath = $toolchainOption.Split('"')[1]
Write-Output $toolchainOptionAndPath

if ("$toolchainOptionAndPath") {
    $buildDirectory = "build"
    $sourceDirectory = "."
    cmake -B $buildDirectory -S $sourceDirectory "$toolchainOptionAndPath"
    cmake --build build
}
else {
    Write-Error "Could not extract the CMake toolchain option."
    return
}
