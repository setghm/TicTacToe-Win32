$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot

$appName = 'TicTacToe'
$distDir = Join-Path $PSScriptRoot 'dist'
$packDir = Join-Path $distDir 'package'
$msixDir = Join-Path $PSScriptRoot 'msix'
$buildDir = Join-Path $PSScriptRoot 'build'
$exeFile = Join-Path $buildDir "$appName.exe"

function Require-Command {
    param([string]$name, [string]$hint)
    if (-not (Get-Command $Name -ErrorAction SilentlyContinue)) {
        Write-Error "$name not found. $hint"
        exit 1
    }
}

$mingwHint = "Make sure you've added MinGW to your PATH."
$vsHint = "Make sure you're running the program using the VS Developer PowerShell."

Require-Command 'make' $mingwHint
Require-Command 'gcc' $mingwHint
Require-Command 'windres' $mingwHint
Require-Command 'makeappx' $vsHint
Require-Command 'makepri' $vsHint

if (-not (Test-Path $msixDir)) {
    Write-Error "msix directory not found at $msixDir"
    eixt 1
}

Write-Host "Performing a clean build..."
make clean
$global:LASTEXITCODE = 0

make
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed."
    exit 1
}
if (-not (Test-Path $exeFile)) {
    Write-Error "$exeFile was not produced. Check your Makefile."
    exit 1
}

if (Test-Path $distDir) {
    Remove-Item $distDir -Recurse -Force
}
New-Item $distDir -ItemType Directory
Copy-Item $msixDir $packDir -Recurse
Copy-Item $exeFile $packDir

Write-Host "Indexing MSIX resources..."
Push-Location $packDir
makepri createconfig /cf priconfig.xml /dq en-US /pv 10.0.0 /o
if ($LASTEXITCODE -ne 0) {
    Write-Error "makepri createconfig failed."
    exit 1
}
makepri new /pr . /cf priconfig.xml /mn AppxManifest.xml /of resources.pri /o
if ($LASTEXITCODE -ne 0) {
    Write-Error "makepri new failed."
    exit 1
}
Remove-Item priconfig.xml
Pop-Location

Write-Host "Creating the MSIX package..."
$msixFile = Join-Path $distDir "$appName.msix"
makeappx pack /d $packDir /p $msixFile /o
if ($LASTEXITCODE -ne 0) {
    Write-Error 'makeappx failed.'
    exit 1
}

Write-Host "Done: $msixFile"
