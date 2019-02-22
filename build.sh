echo $NDKROOT
rm -R Build
set -e
mkdir Build
cd Build
NTV=4.9
STL=gnustl_static
if [ "clang" = "$1" ]; then
	NTV=clang
	# STL=c++_static
	STL=gnustl_static
fi
if [ x"gnu" = x"$2" ]; then
	STL=gnustl_static
fi
echo "NTV=$NTV"
cmake .. -DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=$NTV -DCMAKE_ANDROID_NDK=$NDKROOT -DCMAKE_ANDROID_STL_TYPE=$STL -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI=armeabi-v7a
make -j4
make install
