#!/bin/bash
CPU_JOB_NUM=$(grep processor /proc/cpuinfo | awk '{field=$NF};END{print field+1}')
echo "-------------make clean------------"
make clean
echo "------------build uboot-TP --------"         
make iriver_mx100_n_config
make -j$CPU_JOB_NUM
cat ./sd_fusing/c110.signedBL1_bin u-boot.bin > u-boot-config_fused.bin

