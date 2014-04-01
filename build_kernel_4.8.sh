#!/bin/bash

# location
if [ "${1}" != "" ]; then
	export KERNELDIR=`readlink -f ${1}`;
else
	export KERNELDIR=`readlink -f .`;
fi;

export PARENT_DIR=`readlink -f ..`
export INITRAMFS_TMP=/tmp/initramfs_source;
export INITRAMFS_SOURCE=`readlink -f ..`/Ramdisk
export PACKAGEDIR=$KERNELDIR/READY-JB
#Enable FIPS mode
export USE_SEC_FIPS_MODE=true
export ARCH=arm
export CROSS_COMPILE=$PARENT_DIR/../arm-eabi-4.8/bin/arm-eabi-
export KERNEL_CONFIG=alucard_defconfig;

chmod -R 777 /tmp;

time_start=$(date +%s.%N)

# check xml-config for "STweaks"-app
XML2CHECK="${INITRAMFS_SOURCE}/res/customconfig/customconfig.xml";
xmllint --noout $XML2CHECK;
if [ $? == 1 ]; then
	echo "xml-Error: $XML2CHECK";
	exit 1;
fi;

if [ -d $INITRAMFS_TMP ]; then
	echo "removing old temp initramfs_source";
	rm -rf $INITRAMFS_TMP;
fi;

# copy new config
cp $KERNELDIR/.config $KERNELDIR/arch/arm/configs/$KERNEL_CONFIG;

# remove all old modules before compile
for i in `find $KERNELDIR/ -name "*.ko"`; do
	rm -f $i;
done;

# copy initramfs files to tmp directory
cp -ax $INITRAMFS_SOURCE $INITRAMFS_TMP;

# clear git repository from tmp-initramfs
if [ -d $INITRAMFS_TMP/.git ]; then
	rm -rf $INITRAMFS_TMP/.git;
fi;

# clear mercurial repository from tmp-initramfs
if [ -d $INITRAMFS_TMP/.hg ]; then
	rm -rf $INITRAMFS_TMP/.hg;
fi;

# remove empty directory placeholders from tmp-initramfs
for i in `find $INITRAMFS_TMP -name EMPTY_DIRECTORY`; do
	rm -f $i;
done;

# copy config
if [ ! -f $KERNELDIR/.config ]; then
	cp $KERNELDIR/arch/arm/configs/$KERNEL_CONFIG $KERNELDIR/.config;
fi;

# read config
. $KERNELDIR/.config;

# get version from config
GETVER=`grep 'Alucard-*-V' .config |sed 's/Alucard-//g' | sed 's/.*".//g' | sed 's/-A.*//g'`;

echo "Remove old zImage"
# remove previous zImage files
if [ -e $PACKAGEDIR/boot.img ]; then
	rm $PACKAGEDIR/boot.img;
fi;

if [ -e $KERNELDIR/arch/arm/boot/zImage ]; then
	rm $KERNELDIR/arch/arm/boot/zImage;
fi;

HOST_CHECK=`uname -n`
NAMBEROFCPUS=$(expr `grep processor /proc/cpuinfo | wc -l` + 1);
echo $HOST_CHECK

echo "Making kernel";
make -j${NAMBEROFCPUS} || exit 1;

echo "Copy modules to Package"
for i in `find $KERNELDIR -name '*.ko'`; do
	cp -av $i $INITRAMFS_TMP/lib/modules/;
done;

for i in `find $INITRAMFS_TMP/lib/modules/ -name '*.ko'`; do
	${CROSS_COMPILE}strip --strip-unneeded $i;
done;

chmod 644 $INITRAMFS_TMP/lib/modules/*;

if [ -e $KERNELDIR/arch/arm/boot/zImage ]; then
	echo "Copy zImage to Package"
	cp arch/arm/boot/zImage $PACKAGEDIR/zImage

	echo "Make boot.img"
	./mkbootfs $INITRAMFS_TMP | gzip > $PACKAGEDIR/ramdisk.gz
	#./mkbootimg --kernel $PACKAGEDIR/zImage --ramdisk $PACKAGEDIR/ramdisk.gz --cmdline "console=null androidboot.hardware=qcom user_debug=31 msm_rtb.filter=0x3F ehci-hcd.park=3" -o $PACKAGEDIR/boot.img --base "0x80200000" --ramdiskaddr "0x82200000"
	./mkbootimg --cmdline 'console = null androidboot.hardware=qcom user_debug=31 androidboot.selinux=permissive' --kernel $PACKAGEDIR/zImage --ramdisk $PACKAGEDIR/ramdisk.gz --base 0x80200000 --pagesize 2048 --ramdisk_offset 0x02000000 --output $PACKAGEDIR/boot.img
	cd $PACKAGEDIR

	if [ -e ramdisk.gz ]; then
		rm ramdisk.gz;
	fi;

	if [ -e zImage ]; then
		rm zImage;
	fi;

	echo "Remove old Package Zip Files"
	for i in `find $PACKAGEDIR/ -name '*.zip'`; do
	 rm $i;
	done;

	FILENAME=Kernel-Alucard-${GETVER}-`date +"[%H-%M]-[%d-%m]-AOSP-EUR-KK4.4-SGIV-PWR-CORE"`.zip
	zip -r $FILENAME .;

	time_end=$(date +%s.%N)
	echo -e "${BLDYLW}Total time elapsed: ${TCTCLR}${TXTGRN}$(echo "($time_end - $time_start) / 60"|bc ) ${TXTYLW}minutes${TXTGRN} ($(echo "$time_end - $time_start"|bc ) ${TXTYLW}seconds) ${TXTCLR}"

	FILESIZE=$(stat -c%s "$FILENAME")
	echo "Size of $FILENAME = $FILESIZE bytes."
	
	cd $KERNELDIR
else
	echo "KERNEL DID NOT BUILD! no zImage exist"
fi;
