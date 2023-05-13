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
  int ver = ioctl(dev, JP_IOCTL_VERSION);
  cout << "version: " << hex << setw(3) << setfill('0') << ver << endl;
  return 0;
}
