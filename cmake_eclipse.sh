#!/usr/bin/env bash


$(dirname $0)/cmake_generic.sh $@ -G "Eclipse CDT4 - Unix Makefiles"

# After importing the generated project into Eclipse workspace, set the [Build], [Subprojects], and [Targets] linked resources property as 'Derived'
# to exclude the derived resources in those folders from being listed in the Open Resource and Search Result list

# vi: set ts=4 sw=4 expandtab:
