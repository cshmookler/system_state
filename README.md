 **system_state**

C++ library for modifying the system state on [MOOS](https://github.com/cshmookler/moos). Provides functions for volume control, networking, power management, backlight, thermal, resource usage, and more (see the README).

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

## **TODO**

- [X] date and time
- [X] uptime
- [ ] disk space
- [X] swap
- [X] memory
- [ ] cpu usage
- [ ] cpu temperature
- [X] load averages (1 min, 5 min, 15 min)
- [ ] battery status
- [ ] battery name
- [ ] battery percentage
- [ ] battery time remaining
- [ ] backlight percentage
- [ ] network status
- [ ] network name
- [ ] network SSID
- [ ] network signal strength percentage
- [ ] volume mute
- [ ] volume percentage
- [ ] capture mute
- [ ] capture percentage
- [ ] microphone status
- [ ] camera status
- [ ] user name
- [ ] outdated kernel indicator
