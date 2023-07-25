set paths = [
    C:/Windows/System32
    C:$E:HOMEPATH/scoop/apps/mingw/current/bin
    C:$E:HOMEPATH/scoop/shims 
    C:/hostedtoolcache/windows/xmake/2.8.1/x64
    C:/Program Files/Git/bin
    C:/Program Files/CMake/bin
]
echo $paths

xmake config --policies=build.ccache --yes -vD --plat=mingw --mingw=C:$E:HOMEPATH/scoop/apps/mingw/current
