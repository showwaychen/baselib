echo $NDKROOT
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../android.toolchain.cmake \
          -DANDROID_ABI="armeabi-v7a" \
		  -DANDROID_NATIVE_API_LEVEL=15 \
		  -DANDROID_NDK=$NDKROOT \
		  -DANDROID_STL=gnustl_static \
		  -DCMAKE_BUILD_TYPE=Release