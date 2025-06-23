#!/usr/bin/env bash

find tests -type f -name '*_test.cpp' \
  | sed -E 's!.*/([^/]+)\.cpp$!\1!' \
  | sort \
  | column