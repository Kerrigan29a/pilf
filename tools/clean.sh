#!/usr/bin/env bash

git clean -f -d -x
git submodule foreach git clean -f -d -x
