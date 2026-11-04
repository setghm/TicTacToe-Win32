# TicTacToe Win32

Classic Tic-Tac-Toe game. Win32 application.

Two players needed.

## Game preview

![Screenshot 2025-01-24 122133](https://github.com/user-attachments/assets/275a1490-89b8-43ba-9a1b-bffc250d523f)

## Build the game

You need the MinGW toolchain installed in your system properly added to your PATH. Make sure your installation supports 64-bit targets.

I recommend you to install the [CodeBlocks](https://www.codeblocks.org/) distribution that includes MinGW.

```shell
make clean
make
```

### Create the MSIX package

The MSIX package can be used for both distribute the game and to publish it to Microsoft Store.

Create the package:

```powershell
# Must be executed from the VS Developer Powershell.
.\ms-store-build.ps1
```

Install for local testing:

```powershell
Set-Location dist\package
Add-AppxPackage -Register AppxManifest.xml
```

## Third party

`res/GameFont.ttf`

**Font Family**: Soup of justice

**Author**: Omen Type

**Upstream URL**: https://www.dafont.com/soup-of-justice.font
