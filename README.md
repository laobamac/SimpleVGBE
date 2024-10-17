# SimpleVGBE

Intel 2.5G Ethernet driver for macOS. Based on I225 firmware in LinuxKernel V6.4rc.

macOS下英特尔2.5G网卡驱动

移植自Ubuntu下的网卡驱动：I225 firmware in LinuxKernel V6.4rc

## 支持列表
* I225 卡
* I226 卡

## 支持情况
* IPV4、IPV6自动获取（DHCP）
* TCP校验和
* TSO和TSO6
* 网线热插拔&插拔后重分配
* 支持Sequoia
* 强制速率受`igc_ethtool_set_link_ksettings()`影响无法生效

## 鸣谢
* LinuxKernel的[IGC](https://github.com/torvalds/linux)
