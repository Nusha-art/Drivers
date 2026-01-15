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
