#include <bits/stdc++.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "../include/ioctl.h"

using namespace std;

constexpr const char *device_name = "/dev/JudgePenguin";

int open_dev() {
  int fd = open(device_name, O_RDWR);
  if (fd < 0) {
    cerr << "error opening device " << device_name << endl;
    exit(1);
  }
  return fd;
}

int main() {
  int dev = open_dev();
  printf("device %s opened\n", device_name);
  int ver = ioctl(dev, 0, JP_IOCTL_VERSION);
  int ver_major, ver_minor, ver_patch;
  ver_major = ver >> 8, ver_minor = (ver >> 4) & 0xf, ver_patch = ver & 0xf;
  printf("version: %d.%d.%d\n", ver_major, ver_minor, ver_patch);

  int magic = ioctl(dev, 0, JP_IOCTL_MAGIC);
  char magic_str[5];
  ((int *)magic_str)[0] = magic;
  magic_str[4] = '\0';
  printf("magic: %s\n", magic_str);

  int ret = ioctl(dev, 0, JP_IOCTL_TEST);
  printf("test: %d\n", ret);

  return 0;
}
