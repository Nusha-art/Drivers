# Сборка ядра в buildroot
### Установка необходимых пакетов
```
sudo dnf install -y \
  git make gcc gcc-c++ \
  ncurses-devel \
  bison flex \
  openssl-devel \
  elfutils-libelf-devel \
  bc \
  wget cpio rsync \
  perl python3 \
  qemu-system-x86
```
### Установка buildroot
```
wget https://buildroot.org/downloads/buildroot-2022.11.tar.gz
tar xvf buildroot-2022.11.tar.gz
cd buildroot-2022.11
```
### Создать конфигурацию
```
make qemu_x86_64_defconfig
```
Проверка компонентов:
```
make list-defconfigs | grep qemu
```
### Запуск конфигуратора buildroot
```
make menuconfig
```
Из-за работы в qemu конфигурация была изменена:
```
Kernel → Linux Kernel → [*]
Kernel version → Custom version
Kernel version → 6.17
Toolchain → Headers → 6.0 or later

System hostname → qemu-buildroot
Enable root login with password → [*]
Root password → root

Kernel configuration → Using a defconfig
Defconfig name → x86_64
Kernel binary format → bzImage
```
### Конфигурация ядра
```
make linux-menuconfig
```
```
Processor type and features
 └── [*] Symmetric multi-processing support

Device Drivers
 └── Character devices
     └── [*] Enable TTY
         └── [*] Serial drivers
             └── [*] 8250/16550 UART
                 └── [*] Console on 8250/16550
Device Drivers
 └── Network device support
     └── [*] Virtio network driver
```
### После выхода получается файл
```
output/build/linux-6.17/.config
```
### Для более легкого запуска qmeu в конфигурации buildroot (menuconfig)
```
Filesystem images
 └── [*] cpio the root filesystem (for initramfs)
     Compression → gzip
```
### Сборка buildroot
```
make 2>&1 | tee build.log
```
### Результат сборки
```
ls output/images
```
Вывод
```
<img width="532" height="38" alt="image" src="https://github.com/user-attachments/assets/d1025d34-7a6f-41dc-b6a8-f4da0155fe05" />

```
# Запуск QMEU
```
qemu-system-x86_64 \
  -kernel output/images/bzImage \
  -initrd output/images/rootfs.cpio.gz \
  -append "console=ttyS0" \
  -nographic
```
### Результат

<img width="652" height="481" alt="image" src="https://github.com/user-attachments/assets/fd830c96-0375-4295-ae21-9ac91576a1f4" />

```
[    0.000000] Linux version 6.17.0 (vilena@rt) (x86_64-buildroot-linux-gnu-gcc6
[    0.000000] Command line: console=ttyS0
[    0.000000] BIOS-provided physical RAM map:
[    0.000000] BIOS-e820: [mem 0x0000000000000000-0x000000000009fbff] usable
[    0.000000] BIOS-e820: [mem 0x000000000009fc00-0x000000000009ffff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000000f0000-0x00000000000fffff] reserved
[    0.000000] BIOS-e820: [mem 0x0000000000100000-0x0000000007fdffff] usable
[    0.000000] BIOS-e820: [mem 0x0000000007fe0000-0x0000000007ffffff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000fffc0000-0x00000000ffffffff] reserved
[    0.000000] BIOS-e820: [mem 0x000000fd00000000-0x000000ffffffffff] reserved
[    0.000000] NX (Execute Disable) protection: active
[    0.000000] APIC: Static calls initialized
[    0.000000] SMBIOS 3.0.0 present.
[    0.000000] DMI: QEMU Standard PC (i440FX + PIIX, 1996), BIOS 1.16.2-2.fc39 4
[    0.000000] DMI: Memory slots populated: 1/1
[    0.000000] last_pfn = 0x7fe0 max_arch_pfn = 0x400000000
[    0.000000] MTRR map: 4 entries (3 fixed + 1 variable; max 19), built from 8s
[    0.000000] x86/PAT: Configuration [0-7]: WB  WC  UC- UC  WB  WP  UC- WT  
[    0.000000] found SMP MP-table at [mem 0x000f5be0-0x000f5bef]
[    0.000000] RAMDISK: [mem 0x07d99000-0x07fdffff]
[    0.000000] ACPI: Early table checksum verification disabled
[    0.000000] ACPI: RSDP 0x00000000000F5A10 000014 (v00 BOCHS )
[    0.000000] ACPI: RSDT 0x0000000007FE1C40 000034 (v01 BOCHS  BXPC     000000)
[    0.000000] ACPI: FACP 0x0000000007FE1AF4 000074 (v01 BOCHS  BXPC     000000)
[    0.000000] ACPI: DSDT 0x0000000007FE0040 001AB4 (v01 BOCHS  BXPC     000000)
[    0.000000] ACPI: FACS 0x0000000007FE0000 000040
[    0.000000] ACPI: APIC 0x0000000007FE1B68 000078 (v03 BOCHS  BXPC     000000)
[    0.000000] ACPI: HPET 0x0000000007FE1BE0 000038 (v01 BOCHS  BXPC     000000)
[    0.000000] ACPI: WAET 0x0000000007FE1C18 000028 (v01 BOCHS  BXPC     000000)
[    0.000000] ACPI: Reserving FACP table memory at [mem 0x7fe1af4-0x7fe1b67]
[    0.000000] ACPI: Reserving DSDT table memory at [mem 0x7fe0040-0x7fe1af3]
[    0.000000] ACPI: Reserving FACS table memory at [mem 0x7fe0000-0x7fe003f]
[    0.000000] ACPI: Reserving APIC table memory at [mem 0x7fe1b68-0x7fe1bdf]
[    0.000000] ACPI: Reserving HPET table memory at [mem 0x7fe1be0-0x7fe1c17]
[    0.000000] ACPI: Reserving WAET table memory at [mem 0x7fe1c18-0x7fe1c3f]
[    0.000000] No NUMA configuration found
[    0.000000] Faking a node at [mem 0x0000000000000000-0x0000000007fdffff]
[    0.000000] NODE_DATA(0) allocated [mem 0x07d95900-0x07d98fff]
[    0.000000] Zone ranges:
[    0.000000]   DMA      [mem 0x0000000000001000-0x0000000000ffffff]
[    0.000000]   DMA32    [mem 0x0000000001000000-0x0000000007fdffff]
[    0.000000]   Normal   empty
[    0.000000] Movable zone start for each node
[    0.000000] Early memory node ranges
[    0.000000]   node   0: [mem 0x0000000000001000-0x000000000009efff]
[    0.000000]   node   0: [mem 0x0000000000100000-0x0000000007fdffff]
[    0.000000] Initmem setup node 0 [mem 0x0000000000001000-0x0000000007fdffff]
[    0.000000] On node 0, zone DMA: 1 pages in unavailable ranges
[    0.000000] On node 0, zone DMA: 97 pages in unavailable ranges
[    0.000000] On node 0, zone DMA32: 32 pages in unavailable ranges
[    0.000000] ACPI: PM-Timer IO Port: 0x608
[    0.000000] ACPI: LAPIC_NMI (acpi_id[0xff] dfl dfl lint[0x1])
[    0.000000] IOAPIC[0]: apic_id 0, version 32, address 0xfec00000, GSI 0-23
[    0.000000] ACPI: INT_SRC_OVR (bus 0 bus_irq 0 global_irq 2 dfl dfl)
[    0.000000] ACPI: INT_SRC_OVR (bus 0 bus_irq 5 global_irq 5 high level)
[    0.000000] ACPI: INT_SRC_OVR (bus 0 bus_irq 9 global_irq 9 high level)
[    0.000000] ACPI: INT_SRC_OVR (bus 0 bus_irq 10 global_irq 10 high level)
[    0.000000] ACPI: INT_SRC_OVR (bus 0 bus_irq 11 global_irq 11 high level)
[    0.000000] ACPI: Using ACPI (MADT) for SMP configuration information
[    0.000000] ACPI: HPET id: 0x8086a201 base: 0xfed00000
[    0.000000] CPU topo: Max. logical packages:   1
[    0.000000] CPU topo: Max. logical dies:       1
[    0.000000] CPU topo: Max. dies per package:   1
[    0.000000] CPU topo: Max. threads per core:   1
[    0.000000] CPU topo: Num. cores per package:     1
[    0.000000] CPU topo: Num. threads per package:   1
[    0.000000] CPU topo: Allowing 1 present CPUs plus 0 hotplug CPUs
[    0.000000] PM: hibernation: Registered nosave memory: [mem 0x00000000-0x000]
[    0.000000] PM: hibernation: Registered nosave memory: [mem 0x0009f000-0x000]
[    0.000000] [mem 0x08000000-0xfffbffff] available for PCI devices
[    0.000000] Booting paravirtualized kernel on bare hardware
[    0.000000] clocksource: refined-jiffies: mask: 0xffffffff max_cycles: 0xfffs
[    0.000000] setup_percpu: NR_CPUS:64 nr_cpumask_bits:1 nr_cpu_ids:1 nr_node_1
[    0.000000] percpu: Embedded 52 pages/cpu s172312 r8192 d32488 u2097152
[    0.000000] Kernel command line: console=ttyS0
[    0.000000] printk: log buffer data + meta data: 262144 + 917504 = 1179648 bs
[    0.000000] Dentry cache hash table entries: 16384 (order: 5, 131072 bytes, )
[    0.000000] Inode-cache hash table entries: 8192 (order: 4, 65536 bytes, lin)
[    0.000000] Fallback order for Node 0: 0 
[    0.000000] Built 1 zonelists, mobility grouping on.  Total pages: 32638
[    0.000000] Policy zone: DMA32
[    0.000000] mem auto-init: stack:off, heap alloc:off, heap free:off
[    0.000000] SLUB: HWalign=64, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
[    0.000000] Dynamic Preempt: voluntary
[    0.000000] rcu: Preemptible hierarchical RCU implementation.
[    0.000000] rcu: 	RCU event tracing is enabled.
[    0.000000] rcu: 	RCU restricting CPUs from NR_CPUS=64 to nr_cpu_ids=1.
[    0.000000] 	Trampoline variant of Tasks RCU enabled.
[    0.000000] 	Tracing variant of Tasks RCU enabled.
[    0.000000] rcu: RCU calculated value of scheduler-enlistment delay is 100 j.
[    0.000000] rcu: Adjusting geometry for rcu_fanout_leaf=16, nr_cpu_ids=1
[    0.000000] RCU Tasks: Setting shift to 0 and lim to 1 rcu_task_cb_adjust=1 .
[    0.000000] RCU Tasks Trace: Setting shift to 0 and lim to 1 rcu_task_cb_adj.
[    0.000000] NR_IRQS: 4352, nr_irqs: 256, preallocated irqs: 16
[    0.000000] rcu: srcu_init: Setting srcu_struct sizes based on contention.
[    0.000000] Console: colour VGA+ 80x25
[    0.000000] printk: legacy console [ttyS0] enabled
[    0.000000] ACPI: Core revision 20250404
[    0.000000] clocksource: hpet: mask: 0xffffffff max_cycles: 0xffffffff, max_s
[    0.005000] APIC: Switch to symmetric I/O mode setup
[    0.011000] ..TIMER: vector=0x30 apic1=0 pin1=2 apic2=-1 pin2=-1
[    0.019000] tsc: Unable to calibrate against PIT
[    0.019000] tsc: using HPET reference calibration
[    0.021000] tsc: Detected 3193.925 MHz processor
[    0.002184] clocksource: tsc-early: mask: 0xffffffffffffffff max_cycles: 0x2s
[    0.004461] Calibrating delay loop (skipped), value calculated using timer f)
[    0.010136] Last level iTLB entries: 4KB 512, 2MB 255, 4MB 127
[    0.010136] Last level dTLB entries: 4KB 512, 2MB 255, 4MB 127, 1GB 0
[    0.011473] mitigations: Enabled attack vectors: user_kernel, user_user, SMTo
[    0.013245] Spectre V2 : Mitigation: Retpolines
[    0.014136] Spectre V1 : Mitigation: usercopy/swapgs barriers and __user poin
[    0.014136] Spectre V2 : Spectre v2 / SpectreRSB: Filling RSB on context swiT
[    0.014136] x86/fpu: x87 FPU will use FXSAVE
[    0.097136] Freeing SMP alternatives memory: 52K
[    0.098226] pid_max: default: 32768 minimum: 301
[    0.100136] LSM: initializing lsm=capability,selinux
[    0.103136] SELinux:  Initializing.
[    0.114136] Mount-cache hash table entries: 512 (order: 0, 4096 bytes, linea)
[    0.114136] Mountpoint-cache hash table entries: 512 (order: 0, 4096 bytes, )
[    0.173256] smpboot: CPU0: AMD QEMU Virtual CPU version 2.5+ (family: 0xf, m)
[    0.178296] Performance Events: PMU not available due to virtualization, usi.
[    0.179903] signal: max sigframe size: 1440
[    0.183236] rcu: Hierarchical SRCU implementation.
[    0.184136] rcu: 	Max phase no-delay instances is 400.
[    0.194136] smp: Bringing up secondary CPUs ...
[    0.195896] smp: Brought up 1 node, 1 CPU
[    0.197187] smpboot: Total of 1 processors activated (6387.85 BogoMIPS)
[    0.199136] Memory: 83776K/130552K available (18349K kernel code, 2888K rwda)
[    0.204136] devtmpfs: initialized
[    0.212136] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, ms
[    0.214136] posixtimers hash table entries: 512 (order: 1, 8192 bytes, linea)
[    0.214483] futex hash table entries: 256 (16384 bytes on 1 NUMA nodes, tota.
[    0.221136] PM: RTC time: 02:43:31, date: 2026-01-16
[    0.225136] NET: Registered PF_NETLINK/PF_ROUTE protocol family
[    0.229136] audit: initializing netlink subsys (disabled)
[    0.235163] audit: type=2000 audit(1768531410.252:1): state=initialized audi1
[    0.244136] thermal_sys: Registered thermal governor 'step_wise'
[    0.245136] cpuidle: using governor menu
[    0.251644] PCI: Using configuration type 1 for base access
[    0.255136] kprobes: kprobe jump-optimization is enabled. All kprobes are op.
[    0.262616] HugeTLB: allocation took 0ms with hugepage_allocation_threads=1
[    0.264602] HugeTLB: registered 2.00 MiB page size, pre-allocated 0 pages
[    0.265153] HugeTLB: 28 KiB vmemmap can be freed for a 2.00 MiB page
[    0.285391] ACPI: Added _OSI(Module Device)
[    0.286136] ACPI: Added _OSI(Processor Device)
[    0.286136] ACPI: Added _OSI(Processor Aggregator Device)
[    0.313136] ACPI: 1 ACPI AML tables successfully acquired and loaded
[    0.387847] ACPI: Interpreter enabled
[    0.398340] ACPI: PM: (supports S0 S3 S4 S5)
[    0.398622] ACPI: Using IOAPIC for interrupt routing
[    0.399527] PCI: Using host bridge windows from ACPI; if necessary, use "pcig
[    0.400136] PCI: Using E820 reservations for host bridge windows
[    0.404205] ACPI: Enabled 2 GPEs in block 00 to 0F
[    0.485910] ACPI: PCI Root Bridge [PCI0] (domain 0000 [bus 00-ff])
[    0.486875] acpi PNP0A03:00: _OSC: OS supports [ASPM ClockPM Segments MSI HP]
[    0.488306] acpi PNP0A03:00: _OSC: not requesting OS control; OS requires [E]
[    0.506136] acpi PNP0A03:00: fail to add MMCONFIG information, can't access e
[    0.516576] PCI host bridge to bus 0000:00
[    0.517136] pci_bus 0000:00: root bus resource [io  0x0000-0x0cf7 window]
[    0.517136] pci_bus 0000:00: root bus resource [io  0x0d00-0xffff window]
[    0.517136] pci_bus 0000:00: root bus resource [mem 0x000a0000-0x000bffff wi]
[    0.517136] pci_bus 0000:00: root bus resource [mem 0x08000000-0xfebfffff wi]
[    0.517136] pci_bus 0000:00: root bus resource [mem 0x100000000-0x17fffffff ]
[    0.527136] pci_bus 0000:00: root bus resource [bus 00-ff]
[    0.529667] pci 0000:00:00.0: [8086:1237] type 00 class 0x060000 conventionat
[    0.550136] pci 0000:00:01.0: [8086:7000] type 00 class 0x060100 conventionat
[    0.552321] pci 0000:00:01.1: [8086:7010] type 00 class 0x010180 conventionat
[    0.553136] pci 0000:00:01.1: BAR 4 [io  0xc040-0xc04f]
[    0.553218] pci 0000:00:01.1: BAR 0 [io  0x01f0-0x01f7]: legacy IDE quirk
[    0.553512] pci 0000:00:01.1: BAR 1 [io  0x03f6]: legacy IDE quirk
[    0.554152] pci 0000:00:01.1: BAR 2 [io  0x0170-0x0177]: legacy IDE quirk
[    0.555136] pci 0000:00:01.1: BAR 3 [io  0x0376]: legacy IDE quirk
[    0.557667] pci 0000:00:01.3: [8086:7113] type 00 class 0x068000 conventionat
[    0.558136] pci 0000:00:01.3: quirk: [io  0x0600-0x063f] claimed by PIIX4 ACI
[    0.559202] pci 0000:00:01.3: quirk: [io  0x0700-0x070f] claimed by PIIX4 SMB
[    0.561136] pci 0000:00:02.0: [1234:1111] type 00 class 0x030000 conventionat
[    0.561136] pci 0000:00:02.0: BAR 0 [mem 0xfd000000-0xfdffffff pref]
[    0.562136] pci 0000:00:02.0: BAR 2 [mem 0xfebf0000-0xfebf0fff]
[    0.562136] pci 0000:00:02.0: ROM [mem 0xfebe0000-0xfebeffff pref]
[    0.562861] pci 0000:00:02.0: Video device with shadowed ROM at [mem 0x000c0]
[    0.586782] pci 0000:00:03.0: [8086:100e] type 00 class 0x020000 conventionat
[    0.588136] pci 0000:00:03.0: BAR 0 [mem 0xfebc0000-0xfebdffff]
[    0.588136] pci 0000:00:03.0: BAR 1 [io  0xc000-0xc03f]
[    0.588221] pci 0000:00:03.0: ROM [mem 0xfeb80000-0xfebbffff pref]
[    0.628136] ACPI: PCI: Interrupt link LNKA configured for IRQ 10
[    0.629354] ACPI: PCI: Interrupt link LNKB configured for IRQ 10
[    0.631489] ACPI: PCI: Interrupt link LNKC configured for IRQ 11
[    0.633489] ACPI: PCI: Interrupt link LNKD configured for IRQ 11
[    0.636602] ACPI: PCI: Interrupt link LNKS configured for IRQ 9
[    0.664451] iommu: Default domain type: Translated
[    0.665136] iommu: DMA domain TLB invalidation policy: lazy mode
[    0.668136] SCSI subsystem initialized
[    0.690600] ACPI: bus type USB registered
[    0.693136] usbcore: registered new interface driver usbfs
[    0.694127] usbcore: registered new interface driver hub
[    0.694603] usbcore: registered new device driver usb
[    0.695136] pps_core: LinuxPPS API ver. 1 registered
[    0.695136] pps_core: Software ver. 5.3.6 - Copyright 2005-2007 Rodolfo Giom>
[    0.695753] PTP clock support registered
[    0.726136] Advanced Linux Sound Architecture Driver Initialized.
[    0.731136] NetLabel: Initializing
[    0.731136] NetLabel:  domain hash size = 128
[    0.731174] NetLabel:  protocols = UNLABELED CIPSOv4 CALIPSO
[    0.734455] NetLabel:  unlabeled traffic allowed by default
[    0.764136] PCI: Using ACPI for IRQ routing
[    0.765136] pci 0000:00:02.0: vgaarb: setting as boot VGA device
[    0.765136] pci 0000:00:02.0: vgaarb: bridge control possible
[    0.765136] pci 0000:00:02.0: vgaarb: VGA device added: decodes=io+mem,owns=e
[    0.766227] vgaarb: loaded
[    0.768136] hpet: 3 channels of 0 reserved for per-cpu timers
[    0.768571] hpet0: at MMIO 0xfed00000, IRQs 2, 8, 0
[    0.770211] hpet0: 3 comparators, 64-bit 100.000000 MHz counter
[    0.792136] clocksource: Switched to clocksource tsc-early
[    0.804375] VFS: Disk quotas dquot_6.6.0
[    0.806398] VFS: Dquot-cache hash table entries: 512 (order 0, 4096 bytes)
[    0.817811] pnp: PnP ACPI init
[    0.868157] pnp: PnP ACPI: found 6 devices
[    1.017241] clocksource: acpi_pm: mask: 0xffffff max_cycles: 0xffffff, max_is
[    1.278723] NET: Registered PF_INET protocol family
[    1.280900] IP idents hash table entries: 2048 (order: 2, 16384 bytes, linea)
[    1.284633] tcp_listen_portaddr_hash hash table entries: 256 (order: 0, 4096)
[    1.284633] Table-perturb hash table entries: 65536 (order: 6, 262144 bytes,)
[    1.295591] TCP established hash table entries: 1024 (order: 1, 8192 bytes, )
[    1.299126] TCP bind hash table entries: 1024 (order: 3, 32768 bytes, linear)
[    1.299637] TCP: Hash tables configured (established 1024 bind 1024)
[    1.299637] UDP hash table entries: 256 (order: 2, 16384 bytes, linear)
[    1.299637] UDP-Lite hash table entries: 256 (order: 2, 16384 bytes, linear)
[    1.315809] NET: Registered PF_UNIX/PF_LOCAL protocol family
[    1.329537] RPC: Registered named UNIX socket transport module.
[    1.329537] RPC: Registered udp transport module.
[    1.329537] RPC: Registered tcp transport module.
[    1.329537] RPC: Registered tcp-with-tls transport module.
[    1.329537] RPC: Registered tcp NFSv4.1 backchannel transport module.
[    1.354775] pci_bus 0000:00: resource 4 [io  0x0000-0x0cf7 window]
[    1.357900] pci_bus 0000:00: resource 5 [io  0x0d00-0xffff window]
[    1.358851] pci_bus 0000:00: resource 6 [mem 0x000a0000-0x000bffff window]
[    1.358851] pci_bus 0000:00: resource 7 [mem 0x08000000-0xfebfffff window]
[    1.358851] pci_bus 0000:00: resource 8 [mem 0x100000000-0x17fffffff window]
[    1.371221] pci 0000:00:01.0: PIIX3: Enabling Passive Release
[    1.372126] pci 0000:00:00.0: Limiting direct PCI/PCI transfers
[    1.381712] PCI: CLS 0 bytes, default 64
[    1.400642] Unpacking initramfs...
[    1.466936] Initialise system trusted keyrings
[    1.473162] Freeing initrd memory: 2332K
[    1.605639] workingset: timestamp_bits=56 max_order=15 bucket_order=0
[    1.617272] NFS: Registering the id_resolver key type
[    1.621581] Key type id_resolver registered
[    1.621897] Key type id_legacy registered
[    1.627586] 9p: Installing v9fs 9p2000 file system support
[    1.766902] Key type asymmetric registered
[    1.771595] Asymmetric key parser 'x509' registered
[    1.774841] Block layer SCSI generic (bsg) driver version 0.4 loaded (major )
[    1.779991] io scheduler mq-deadline registered
[    1.780930] io scheduler kyber registered
[    1.792191] input: Power Button as /devices/LNXSYSTM:00/LNXPWRBN:00/input/in0
[    1.824693] ACPI: button: Power Button [PWRF]
[    1.841318] Serial: 8250/16550 driver, 4 ports, IRQ sharing enabled
[    1.849584] 00:04: ttyS0 at I/O 0x3f8 (irq = 4, base_baud = 115200) is a 165A
[    1.882778] Non-volatile memory driver v1.3
[    1.883194] Linux agpgart interface v0.103
[    1.888513] ACPI: bus type drm_connector registered
[    1.942363] loop: module loaded
[    1.954236] scsi host0: ata_piix
[    1.967834] scsi host1: ata_piix
[    1.968505] ata1: PATA max MWDMA2 cmd 0x1f0 ctl 0x3f6 bmdma 0xc040 irq 14 lp0
[    1.971163] ata2: PATA max MWDMA2 cmd 0x170 ctl 0x376 bmdma 0xc048 irq 15 lp0
[    2.001566] e100: Intel(R) PRO/100 Network Driver
[    2.003766] e100: Copyright(c) 1999-2006 Intel Corporation
[    2.005412] e1000: Intel(R) PRO/1000 Network Driver
[    2.005811] e1000: Copyright (c) 1999-2006 Intel Corporation.
[    2.093730] ACPI: \_SB_.LNKC: Enabled at IRQ 11
[    2.461248] ata2: found unknown device (class 0)
[    2.487364] ata2.00: ATAPI: QEMU DVD-ROM, 2.5+, max UDMA/100
[    2.560957] scsi 1:0:0:0: CD-ROM            QEMU     QEMU DVD-ROM     2.5+ P5
[    2.648112] sr 1:0:0:0: [sr0] scsi3-mmc drive: 4x/4x cd/rw xa/form2 tray
[    2.649981] cdrom: Uniform CD-ROM driver Revision: 3.20
[    2.728543] sr 1:0:0:0: Attached scsi generic sg0 type 5
[    2.912494] e1000 0000:00:03.0 eth0: (PCI:33MHz:32-bit) 52:54:00:12:34:56
[    2.915641] e1000 0000:00:03.0 eth0: Intel(R) PRO/1000 Network Connection
[    2.917310] e1000e: Intel(R) PRO/1000 Network Driver
[    2.917612] e1000e: Copyright(c) 1999 - 2015 Intel Corporation.
[    2.920086] sky2: driver version 1.30
[    2.925005] usbcore: registered new interface driver usblp
[    2.926473] usbcore: registered new interface driver usb-storage
[    2.929002] i8042: PNP: PS/2 Controller [PNP0303:KBD,PNP0f13:MOU] at 0x60,0x2
[    2.934137] serio: i8042 KBD port at 0x60,0x64 irq 1
[    2.939776] serio: i8042 AUX port at 0x60,0x64 irq 12
[    2.962046] rtc_cmos 00:05: RTC can wake from S4
[    2.980252] rtc_cmos 00:05: registered as rtc0
[    2.999606] hpet: Lost 1 RTC interrupts
[    3.007961] rtc_cmos 00:05: alarms up to one day, y3k, 242 bytes nvram, hpets
[    3.017779] device-mapper: ioctl: 4.50.0-ioctl (2025-04-28) initialised: dm-v
[    3.021309] amd_pstate: the _CPC object is not present in SBIOS or ACPI disad
[    3.026063] hid: raw HID events driver (C) Jiri Kosina
[    3.038955] usbcore: registered new interface driver usbhid
[    3.040275] usbhid: USB HID core driver
[    3.072699] Initializing XFRM netlink socket
[    3.075693] NET: Registered PF_INET6 protocol family
[    3.112503] Segment Routing with IPv6
[    3.112896] In-situ OAM (IOAM) with IPv6
[    3.119579] sit: IPv6, IPv4 and MPLS over IPv4 tunneling driver
[    3.134974] NET: Registered PF_PACKET protocol family
[    3.142887] 9pnet: Installing 9P2000 support
[    3.143750] Key type dns_resolver registered
[    3.150141] IPI shorthand broadcast: enabled
[    3.239874] sched_clock: Marking stable (3258738279, -19863282)->(5252527845)
[    3.268875] registered taskstats version 1
[    3.273550] Loading compiled-in X.509 certificates
[    3.326506] Demotion targets for Node 0: null
[    3.332743] PM:   Magic number: 10:65:713
[    3.334198] netconsole: network logging started
[    3.336242] cfg80211: Loading compiled-in X.509 certificates for regulatory e
[    3.500963] modprobe (49) used greatest stack depth: 13320 bytes left
[    3.554182] Loaded X.509 cert 'sforshee: 00b28ddf47aef9cea7'
[    3.556150] Loaded X.509 cert 'wens: 61c038651aabdcf94bd0ac7ff06c7248db18c60'
[    3.560465] faux_driver regulatory: Direct firmware load for regulatory.db f2
[    3.565812] ALSA device list:
[    3.567624]   No soundcards found.
[    3.569627] cfg80211: failed to load regulatory.db
[    3.714099] Freeing unused kernel image (initmem) memory: 2864K
[    3.716508] Write protecting the kernel read-only data: 26624k
[    3.719592] Freeing unused kernel image (text/rodata gap) memory: 80K
[    3.721596] Freeing unused kernel image (rodata/data gap) memory: 876K
[    4.122564] x86/mm: Checked W+X mappings: passed, no W+X pages found.
[    4.125724] tsc: Refined TSC clocksource calibration: 3193.999 MHz
[    4.129909] clocksource: tsc: mask: 0xffffffffffffffff max_cycles: 0x2e0a24cs
[    4.132080] clocksource: Switched to clocksource tsc
[    4.147250] clocksource: Long readout interval, skipping watchdog check: cs_0
[    4.159126] input: ImExPS/2 Generic Explorer Mouse as /devices/platform/i8042
[    4.184968] input: AT Translated Set 2 keyboard as /devices/platform/i8042/s3
[    4.204620] Run /init as init process
Starting syslogd: OK
Starting klogd: OK
Running sysctl: OK
Saving random seed: [    7.150592] random: crng init done
OK
Starting network: [    7.503471] e1000: eth0 NIC Link is Up 1000 Mbps Full DuplX
udhcpc: started, v1.35.0
udhcpc: broadcasting discover
udhcpc: broadcasting select for 10.0.2.15, server 10.0.2.2
udhcpc: lease of 10.0.2.15 obtained from 10.0.2.2, lease time 86400
deleting routers
adding dns 10.0.2.3
OK

Welcome to Buildroot
qemu-buildroot login: root
Password: 
```

