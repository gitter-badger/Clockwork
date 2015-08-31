#!/usr/bin/env bash
if [ -d $(dirname $0)/../share/Clockwork/Resources ]; then OPT1="-pp ../share/Clockwork/Resources"; fi
if [ $# -eq 0 ]; then OPT2="-w -s"; fi
$(dirname $0)/ClockworkPlayer Scripts/Editor.as $OPT1 $OPT2 $@
