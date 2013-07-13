samsung-kernel-jfltexx
======================
Bare repositories for jfltexx (i9505)

www.broodplank.net



Basic Info:
----------

Branches:
- tw-4.2.2 = Stock i9505 TouchWiz Kernel from Samsung sources patched until latest linux kernel version
- aosp-4.2.2 = i9505g Google Edition Kernel (for AOSP 4.2.2 Google Edition and TouchWiz 4.2.2) (for i9505)


Cloning:
- git clone https://github.com/broodplank/samsung-kernel-jfltexx -b branchname


Forking:
- Click Fork at the right top of the page





Usage Info:
----------

Initializing first config:
- make VARIANT_DEFCONFIG=jf_eur_defconfig jf_defconfig


Configuration:
- xconfig (or menuconfig, or gconfig, or nconfig)


Building:
- export ARCH=arm
- export CROSS_COMPILE=/path/to/toolchain/bin/arm-eabi-
- make -jx (where x is the amount of cpu cores + 1)




Updating Info:
--------------

If you have forked the repository do this to fetch the updates from this repository:

First add it to the remote:
- git remote add bare https://github.com/broodplank/samsung-kernel-jfltexx
- git fetch bare

Then search for the commit(s) that you want to merge

For 1 commit: 
- git cherry-pick [commit hash here, this is the last part of a commits url]

(for example: https://github.com/broodplank/samsung-kernel-jfltexx/commit/612bbea08b67f16dd5628cb8cd8406973f8fe821
would be: 612bbea08b67f16dd5628cb8cd8406973f8fe821)


For a range of commits:
- git cherry-pick [commit a hash]^..[commit b hash]

Where commit a is the oldest commit and commit b the newest.

for example: git cherry-pick 0bff3bdba64c58ea6100d72763f2be3acc9659db^..612bbea08b67f16dd5628cb8cd8406973f8fe821

This would patch the sources from 3.4.40 to 3.4.52

