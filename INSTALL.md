# Installation Instructions

This document details the steps necessary to build a copy of Inazuma Eleven 3
(JP) using this repository.

## Table of Contents

- [1. Setting Up Your Development Environment](#1-setting-up-your-development-environment)
  - [Windows Subsystem for Linux](#windows-subsystem-for-linux)
    - [New Installs](#new-installs)
    - [Existing Installs](#existing-installs)
    - [Install Build Dependencies](#install-build-dependencies)
  - [Windows with MSYS2](#windows-with-msys2)
  - [macOS](#macos)
  - [Linux](#linux)
    - [Debian (and derivatives, e.g., Ubuntu, Mint)](#debian-and-derivatives-eg-ubuntu-mint)
    - [Arch Linux (and derivatives, e.g., Manjaro, Endeavour)](#arch-linux-and-derivatives-eg-manjaro-endeavour)
    - [Fedora (and derivatives, e.g., AlmaLinux, Red Hat Enterprise Linux)](#fedora-and-derivatives-eg-almalinux-red-hat-enterprise-linux)
- [2. Downloading the Repository](#2-downloading-the-repository)
- [3. Building the Repository](#3-building-the-repository)
- [4. Debugger Support](#4-debugger-support)
- [Troubleshooting FAQ](#troubleshooting-faq)
  - [My Build is Failing After Merging from Main](#my-build-is-failing-after-merging-from-main)

## 1. Setting Up Your Development Environment

### Windows Subsystem for Linux

> [!IMPORTANT]
> If you intend to store your project on the Windows file system (or do not know
> what that means), then use these instructions, which will guide you through
> installing WSL version 1. If you intend to use WSL version 2, then instead
> follow the instructions for [Linux](#linux).

#### New Installs

Follow these instructions if you do not have an existing install of WSL.

1. Open [Windows PowerShell as Administrator](https://i.imgur.com/QKmVbP9.png).
Paste (Right Click or Shift+Insert) the following command:

    ```powershell
    wsl --install -d Ubuntu
    ```

2. Once the process finishes, you will be prompted to restart your machine.
Accept.

3. After rebooting, reopen PowerShell and run the following command to downgrade
WSL to version 1:

    ```powershell
    wsl --set-version Ubuntu 1
    ```

    WSL version 1 is preferred for most WSL users due to its increased performance
when accessing files in the Windows file system.

4. Open `Ubuntu` from your Start menu.

5. `Ubuntu` will set up its own installation when it runs for the first time. Once
finished, it will ask for a username and password as input.

    > [!NOTE]
    > When typing the password, there will be no visible response; this is normal,
    > and the terminal is still reading your input.

6. Update `Ubuntu`'s package registry:

    ```bash
    sudo apt update && sudo apt upgrade
    ```

7. [`Install build dependencies`](#install-build-dependencies).

#### Existing Installs

Follow these instructions if you have an existing install of WSL, specifically
`Ubuntu`.

Older versions of `Ubuntu` (e.g., `20.04`) ship with an outdated version of
Python, which is not supported. To remedy this, you can upgrade your existing
install to a more recent version of `Ubuntu`:

1. Run the following inside `Ubuntu`:

    ```bash
    sudo apt upgrade && sudo apt full-upgrade
    ```

2. Open PowerShell and run the following commands to restart `Ubuntu`:

    ```powershell
    wsl -t Ubuntu
    wsl -d Ubuntu
    ```

3. Re-open `Ubuntu` and run the following to start a system upgrade:

    ```bash
    sudo do-release-upgrade
    ```

    This process may take a long time.

4. Once `Ubuntu` is done upgrading, update `Ubuntu`'s package registry:

    ```bash
    sudo apt update && sudo apt upgrade
    ```

5. [`Install build dependencies`](#install-build-dependencies).

#### Install Build Dependencies

1. Run the following to install build dependencies from the `Ubuntu` package
registry:

    ```bash
    sudo apt install bison flex g++ gcc-arm-none-eabi git lib32z1 make ninja-build pkg-config python3 p7zip
    ```

2. [Download the repository](#2-downloading-the-repository).

### Windows with MSYS2

If you are unable to run Windows Subsystem for Linux due to performance reasons
or lacking virtualization requirements, then MSYS2 may be an option for you.

1. Download the MSYS2 installer from [the official website](https://www.msys2.org/)
and install it on your system.

2. Once the installation is complete, a terminal should automatically pop up.
To update your package registry, enter the following command:

    ```bash
    pacman -Syu
    ```

    Press 'Y' when prompted to confirm the update. This process may take a few
minutes. Once completed, the terminal will automatically close.

3. Re-open an MSYS terminal (the pink icon) from your Start Menu, then enter
the following commands to install necessary build dependencies:

    ```bash
    echo 'export PATH=${PATH}:/mingw64/bin' >> ~/.bashrc
    source ~/.bashrc
    pacman -S bison flex gcc git make ninja python mingw-w64-x86_64-arm-none-eabi-gcc p7zip
    ```

    Press 'Y' when prompted to confirm the installation.

4. [Download the repository](#2-downloading-the-repository).

### macOS

1. Apple bundles a number of the requisite utilities into Xcode Command Line Tools;
to install these, run:

    ```zsh
    xcode-select --install
    ```

2. Install [Homebrew](https://brew.sh/).

3. Run the following commands to install additional dependencies:

    ```zsh
    brew update
    brew install gcc@14 ninja libpng pkg-config arm-none-eabi-gcc xz
    brew install --cask wine-stable
    ```

4. You may need to authorize the Wine installation to satisfy macOS security
requirements. To do this, open the Applications folder in Finder and locate the
Wine Stable application. Control-Click on this icon to open the context menu,
then Control-Click on Open and grant the requested permissions.

5. If your macOS installation is Monterey (12) or earlier, then you may also need
GNU `coreutils` installed to run the build scripts:

    ```zsh
    brew install coreutils
    ```

6. [Download the repository](#2-downloading-the-repository).

> [!TIP]
> You can run a persistent Wine server in the background to speed up builds.
>
> To do this, open the Wine Stable application from your Applications folder.
> In the terminal window that opens, run the following command:
>
> ```zsh
> wineserver -p
> ```
>
> When trying to build the repository, the first invocation of `make` may hang
> on compiling a file. If that happens, hit Control+C to interrupt the build,
> and run `make` again.
>
> If you need to stop the Wine server, run the following command:
>
> ```zsh
> wineserver -k
> ```

### Linux

> [!NOTE]
> Precise packages to be installed will vary by Linux distribution and
> package registry. A handful of common distributions are listed below for
> convenience.

Once you have installed all of the listed dependencies, proceed to [downloading
the repository](#2-downloading-the-repository).

#### Debian (and derivatives, e.g., Ubuntu, Mint)

1. Enable 32-bit installations using `dpkg`:

    ```bash
    sudo dpkg --add-architecture i386 && sudo apt update
    ```

2. Install the following packages via `apt`:

    ```bash
    sudo apt install bison flex g++ gcc-arm-none-eabi git make ninja-build pkg-config wget python3 xz-utils nasm libc6:i386
    ```

#### Arch Linux (and derivatives, e.g., Manjaro, Endeavour)

1. Enable the [multilib repository](https://wiki.archlinux.org/title/Multilib).

2. Install dependencies via `pacman`:

    ```bash
    sudo pacman -S arm-none-eabi-gcc bison flex gcc git make ninja python wget xz lib32-glibc
    ```

#### Fedora (and derivatives, e.g., AlmaLinux, Red Hat Enterprise Linux)

```bash
sudo dnf install arm-none-eabi-gcc-cs bison flex gcc-c++ git make ninja-build python3 wget2 xz glibc32
```

## 2. Downloading the Repository

From your terminal, navigate to the path in which you will store the repository.
Users of WSL 1 should ensure that their target is on the Windows file drive.

```bash
git clone https://github.com/CacaBueno64/ie3ogres
cd ie3ogres
```

## 2a. Install Twl library headers:

The repository needs the headers for the Twl system libraries to build. Copy the `include` folder from the following versions into the corresponding directories under `lib`:

* [TwlSDK 5.4](https://twlsdk.randommeaninglesscharacters.com/download/TwlSDK/TwlSDK-5_4-20091225a.zip), plus the [patch1](https://twlsdk.randommeaninglesscharacters.com/download/TwlSDK/TwlSDK-5_4-patch1-20100204.zip) (overwrite the headers in 5.4's include folder with the ones from patch1's)
* [TwlSystem 2.2.0](https://twlsdk.randommeaninglesscharacters.com/download/TwlSystem/TwlSystem-2_2_0-20090805.zip)
* [TwlWiFi 3.0](https://twlsdk.randommeaninglesscharacters.com/download/TwlWiFi/TwlWiFi-3_0-20081027.zip) plus the [patch3](https://twlsdk.randommeaninglesscharacters.com/download/TwlWiFi/TwlWiFi-3_3-20090731.zip) (overwrite the headers in 3.0's include folder with the ones from patch3's)
* [TwlDWC 5.3 patch1](https://twlsdk.randommeaninglesscharacters.com/download/TwlDWC/TwlDWC-5_3-patch1-20091029-jp.zip)

## 3. Building the Repository

To build the ROM, run:

```bash
make
```

If everything works, then the following ROM should be built:

- [build/ie3ogre.jp.nds](https://datomatic.no-intro.org/index.php?page=show_record&s=28&n=5421) `sha1: 9380f1d7ee75e7c6ef24f1b76c88ecdc85088957`

## Troubleshooting FAQ

### My Build is Failing After Merging from Main

It is likely that your subprojects are out of date; update them with the following
command:

```bash
make update
```

And then try rebuilding.
