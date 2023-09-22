# 如何格式化代码
使用 Elvish 执行：
```
elvish bin/format
```
## clang-format
将 clang-format 固定在 `16.0.x` 版本。

### macOS
```
brew install clang-format
```
### Windows
```
scoop install llvm@16.0.6
```

### Linux
在 Linux 上，我们需要手动安装 llvm。

例如：
```
xrepo install "llvm==16.0.5"
```