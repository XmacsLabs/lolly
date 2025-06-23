# 如何测试
对于开发者来说，一共有两种测试方式，其一为单元测试，其二为集成测试。

## 配置

```
xmake config --enable_tests=true
xmake build
```

## 列出所有target
下面的命令行可以列出所有target，其中以`_test`结尾的是C++的单元测试。
``` shell
chmod +x ./bin/list_tests.sh
./bin/list_test.sh
analyze_test            fast_search_test        hashset_test            md5_test                rel_hashmap_test        string_view_test        unicode_test
array_test              file_test               hashtree_test           ntuple_test             shared_lib_test         subprocess_test         url_test
base64_test             file_url_test           http_test               numeral_test            sha_test                sys_utils_test
blackbox_test           hashfunc_test           iterator_test           parse_string_test       string_test             tm_ostream_test
fast_alloc_test         hashmap_test            list_test               promise_test            string_u16_test         tm_timer_test
```

## 单元测试
如上面显示，单元测试形如xxx_test，可以运行下列指令进行测试（运行前请进行构建，详细见[xmake](https://xmake.io)）：

运行单个单元测试：
```
xmake test tests/xxx_test
```

运行所有单元测试：
```
xmake test
```

