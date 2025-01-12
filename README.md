# **system_state**

C++ library for fetching and modifying the system state on [MOOS](https://github.com/cshmookler/moos). Provides functions for volume control, networking, power management, backlight, thermal, resource usage, and more (see the README).

> NOTE: Features are not guaranteed to be thread safe!

## Build from Source

### 1.&nbsp; Install a C++ compiler, Meson, GoogleTest (optional), and ALSA libraries.

#### Linux (MOOS):

```bash
sudo pacman -S base-devel meson gtest alsa-lib
```

### 2.&nbsp; Clone this project.

```
git clone https://github.com/cshmookler/system_state.git
cd system_state
```

### 3.&nbsp; Build this project from source.

```
meson setup build
cd build
ninja
```

### 4.&nbsp; (Optional) Run all tests.

```
sudo ninja test
```

> NOTE: Some tests require root privileges to succeed.

### 4.&nbsp; (Optional) Install this project globally.

```
meson install
```

## **TODO**

- [X] error handling
- [X] make (partially) thread safe
- [X] add traces to error messages
- [X] examples
- [X] uptime
- [X] disks
- [X] disk partitions
- [X] disk name
- [X] disk space (and other related information)
- [ ] disk partition schemes
- [X] partition name
- [X] partition start position
- [ ] partition filesystems
- [ ] partition mount points
- [X] swap
- [X] memory
- [X] total cpu usage
- [X] individual cpu usage
- [ ] process statistics
- [X] thermal sensors
- [X] cooling devices
- [X] load averages (1 min, 5 min, 15 min)
- [X] battery name
- [X] battery status
- [X] battery current
- [X] battery power
- [X] battery percentage
- [X] battery time remaining
- [X] backlight name
- [X] backlight percentage
- [X] network interface name
- [X] network interface status
- [ ] network interface SSID
- [X] network interface statistics
- [ ] network interface signal strength percentage
- [X] playback mute
- [X] playback toggle
- [X] playback volume percentage
- [X] capture mute
- [X] capture toggle
- [X] capture volume percentage
- [ ] microphone status
- [ ] camera status
- [X] user name
- [X] running kernel version
- [X] installed kernel versions
