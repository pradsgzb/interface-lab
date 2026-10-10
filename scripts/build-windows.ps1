[CmdletBinding()]
param(
    [switch]$WithQt,
    [string]$QtPrefix = "",
    [ValidateSet("Debug", "Release")][string]$Configuration = "Debug"
)
$ErrorActionPreference = "Stop"
$RepoDir = Split-Path -Parent $PSScriptRoot
$Folder = if ($WithQt) { "build-qt" } else { "build" }
$BuildDir = Join-Path $RepoDir $Folder
$QtOption = if ($WithQt) { "ON" } else { "OFF" }
$CMakeArgs = @("-S", $RepoDir, "-B", $BuildDir, "-DCMAKE_BUILD_TYPE=$Configuration", "-DINTERFACE_LAB_QT=$QtOption")
if ($QtPrefix) {
    $CMakeArgs += "-DCMAKE_PREFIX_PATH=$QtPrefix"
    # Qt DLLs must be discoverable when CTest starts GUI/web executables.
    $env:PATH = (Join-Path $QtPrefix "bin") + [IO.Path]::PathSeparator + $env:PATH
}
& cmake @CMakeArgs
if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed." }
& cmake --build $BuildDir --config $Configuration --parallel 2
if ($LASTEXITCODE -ne 0) { throw "Build failed." }
& ctest --test-dir $BuildDir -C $Configuration --output-on-failure
if ($LASTEXITCODE -ne 0) { throw "Tests failed." }
Write-Host "Run: & `"$BuildDir\bin\interface_lab.exe`" --all"
