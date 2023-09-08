#!/usr/bin/env elvish

use platform

if $platform:is-windows {
  set paths = [
    C:/Windows/System32
    $E:ProgramFiles/Git/bin
    $E:ProgramFiles/CMake/bin
    $E:USERPROFILE/scoop/shims
    C:/hostedtoolcache/windows/xmake/2.8.2/x64
  ]
}

rm -rf .xmake
rm -rf build
xmake config --policies=build.ccache --yes -vD --plat=mingw --mode=releasedbg
