# How to Perform Testing
For developers, there are two testing methods available: unit testing and integration testing.

## Configuration

```
xmake config --enable_tests=true
xmake build
```

## Listing all Targets
The following command line can list all targets. Targets ending with `_test` are C++ unit tests.
``` shell
chmod +x ./bin/list_tests.sh
./bin/list_test.sh
analyze_test            fast_search_test        hashset_test            md5_test                rel_hashmap_test        string_view_test        unicode_test
array_test              file_test               hashtree_test           ntuple_test             shared_lib_test         subprocess_test         url_test
base64_test             file_url_test           http_test               numeral_test            sha_test                sys_utils_test
blackbox_test           hashfunc_test           iterator_test           parse_string_test       string_test             tm_ostream_test
fast_alloc_test         hashmap_test            list_test               promise_test            string_u16_test         tm_timer_test
```

## Unit Tests
As shown above, unit tests are in the format of xxx_test. You can run the following command to perform the tests (ensure you have built the code beforehand, see [xmake](https://xmake.io) for details):

Run only one test:
```
xmake test tests/xxx_test
```

Run all tests:
```
xmake test
```