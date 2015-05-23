BUILT_TARGET="Release"

if [ "$#" == 0 ]; then
	BUILD_TARGET="Release"
else
	BUILD_TARGET="$1"
fi

cd project
cmake -DCMAKE_BUILD_TYPE="$BUILD_TARGET" -G "CodeBlocks - Unix Makefiles" ../
