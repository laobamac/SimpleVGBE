# SimpleVGBE

Intel 2.5G Ethernet driver for macOS. Based on I225 firmware in LinuxKernel V6.5.

macOS下英特尔2.5G网卡驱动

~~移植自Ubuntu下的网卡驱动：I225 firmware in LinuxKernel V6.5rc~~
20250220已同步到I225 Firmware in LinuxKernel V6.5

正在计划移植V6.11、V6.8内核中的驱动，高中学业繁忙，请等待更新

传送门：LinuxKernel V6.5 [torvalds/linux](https://github.com/torvalds/linux/releases/tag/v6.5)

## 支持列表
* I225 卡
* I226 卡
* 具体硬件ID参考[igc_hw](https://github.com/laobamac/SimpleVGBE/blob/main/SimpleVGBE/firmware/igc_hw.h)

## 支持情况
* IPV4、IPV6自动获取（DHCP）
* TCP校验和
* TSO和TSO6
* 网线热插拔&插拔后重分配
* 支持Sequoia
* 强制速率受`igc_ethtool_set_link_ksettings()`影响无法生效

## 鸣谢
* [LinuxKernel](https://github.com/torvalds/linux) 的IGC驱动
* @Shaneee的[AppleIGB](https://github.com/Shaneee/AppleIGB)
* @weachy帮助测试I226-V&I226-LM
* @win10Q帮助测试I226-V
