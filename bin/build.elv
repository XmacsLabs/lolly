set paths = [
    C:/Windows/System32
    C:$E:HOMEPATH/scoop/apps/mingw/current/bin
    C:$E:HOMEPATH/scoop/shims 
    C:/hostedtoolcache/windows/xmake/2.8.1/x64
]
echo $paths

xmake build --yes -vD --all
