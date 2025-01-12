// External includes
#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

// Local includes
#include "../system_state/core.hpp"
#include "util.hpp"

namespace syst {

struct sound_mixer_t::impl_t {
    snd_mixer_t* mixer = nullptr;
};

sound_mixer_t::sound_mixer_t(const impl_t& impl)
: impl_(std::make_unique<impl_t>(impl)) {
}

sound_mixer_t::sound_mixer_t(sound_mixer_t&& sound_mixer) noexcept {
    if (sound_mixer.impl_ == nullptr || sound_mixer.impl_->mixer == nullptr) {
        return;
    }

    this->impl_ = std::make_unique<impl_t>(impl_t{ sound_mixer.impl_->mixer });
    sound_mixer.impl_->mixer = nullptr;
}

sound_mixer_t::~sound_mixer_t() {
    if (this->impl_ == nullptr || this->impl_->mixer == nullptr) {
        return;
    }

    // Assume that this function always succeeds. Destructors have no way of
    // communicating failure.
    snd_mixer_close(this->impl_->mixer);
}

syst::optional_t<sound_mixer_t> sound_mixer_t::get() {
    // This solution was derived from the first half of the answer to this Stack
    // Overflow post:
    // https://stackoverflow.com/questions/6787318/set-alsa-master-volume-from-c-code

    int snd_errno = 0;

    impl_t impl;

    // NOTE: Mixer mode is an unused attribute and can be any value.
    // https://stackoverflow.com/questions/45716092/alsa-snd-mixer-open-open-mode
    // https://git.alsa-project.org/?p=alsa-lib.git;a=blob;f=src/mixer/mixer.c;h=3a79c8e91efb0f6c67c0ba365b54eb6f72679d69;hb=HEAD#l68
    const int mixer_mode = 0;

    snd_errno = snd_mixer_open(&impl.mixer, mixer_mode);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(std::string{ "ALSA error: snd_mixer_open: " }
          + snd_strerror(snd_errno));
    }

    // NOTE: I was unable to find documentation for the "default" HCTL
    // interface. This code assumes that it is always available.
    const std::string default_hctl_name = "default";
    // HCTL = High level ConTroL interface
    // https://www.alsa-project.org/alsa-doc/alsa-lib/group___h_control.html

    snd_errno = snd_mixer_attach(impl.mixer, default_hctl_name.data());
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(std::string{ "ALSA error: snd_mixer_attach: " }
          + snd_strerror(snd_errno));
    }

    snd_errno = snd_mixer_selem_register(impl.mixer, nullptr, nullptr);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_register: " }
          + snd_strerror(snd_errno));
    }

    snd_errno = snd_mixer_load(impl.mixer);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(std::string{ "ALSA error: snd_mixer_load: " }
          + snd_strerror(snd_errno));
    }

    return sound_mixer_t{ impl };
}

struct sound_control_t::impl_t {
    snd_mixer_elem_t* elem = nullptr;
};

std::list<sound_control_t> sound_mixer_t::all_controls() const {
    snd_mixer_elem_t* first_elem = snd_mixer_first_elem(this->impl_->mixer);
    snd_mixer_elem_t* last_elem = snd_mixer_last_elem(this->impl_->mixer);

    std::list<sound_control_t> controls;

    if (first_elem == nullptr || snd_mixer_selem_is_active(first_elem) == 0) {
        return controls;
    }

    controls.push_back(
      sound_control_t{ sound_control_t::impl_t{ first_elem } });
    while (controls.back().impl_->elem != last_elem) {
        snd_mixer_elem_t* next_elem =
          snd_mixer_elem_next(controls.back().impl_->elem);

        if (snd_mixer_selem_is_active(first_elem) == 0) {
            continue;
        }

        controls.emplace_back(
          sound_control_t{ sound_control_t::impl_t{ next_elem } });
    }

    return controls;
}

sound_control_t::sound_control_t(const impl_t& impl)
: impl_(std::make_unique<impl_t>(impl)) {
}

sound_control_t::~sound_control_t() {
}

std::string sound_control_t::name() const {
    return snd_mixer_selem_get_name(this->impl_->elem);
}

bool sound_control_t::has_playback_status() const {
    return snd_mixer_selem_has_playback_switch(this->impl_->elem) != 0;
}

bool sound_control_t::has_playback_volume() const {
    return snd_mixer_selem_has_playback_volume(this->impl_->elem) != 0;
}

bool sound_control_t::has_capture_status() const {
    return snd_mixer_selem_has_capture_switch(this->impl_->elem) != 0;
}

bool sound_control_t::has_capture_volume() const {
    return snd_mixer_selem_has_capture_volume(this->impl_->elem) != 0;
}

result_t get_channel_playback_status(snd_mixer_elem_t* control,
  snd_mixer_selem_channel_id_t channel,
  std::optional<bool>& status) {
    if (snd_mixer_selem_has_playback_channel(control, channel) == 0) {
        status = std::nullopt;
        return syst::success;
    }

    int value = 0;
    int snd_errno =
      snd_mixer_selem_get_playback_switch(control, channel, &value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_get_playback_switch: " }
          + snd_strerror(snd_errno));
    }

    status = (value != 0);
    return syst::success;
}

syst::optional_t<sound_control_t::status_t>
sound_control_t::get_playback_status() const {
    status_t status{};

    result_t result = get_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_LEFT, status.front_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_RIGHT, status.front_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_LEFT, status.rear_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_RIGHT, status.rear_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_CENTER, status.front_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_WOOFER, status.woofer);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_SIDE_LEFT, status.side_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_SIDE_RIGHT, status.side_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_CENTER, status.rear_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return status;
}

result_t get_channel_playback_volume(snd_mixer_elem_t* control,
  snd_mixer_selem_channel_id_t channel,
  long min_value,
  long max_value,
  std::optional<double>& status) {
    if (snd_mixer_selem_has_playback_channel(control, channel) == 0) {
        status = std::nullopt;
        return syst::success;
    }

    long value = 0;
    int snd_errno =
      snd_mixer_selem_get_playback_volume(control, channel, &value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_get_playback_switch: " }
          + snd_strerror(snd_errno));
    }

    status = syst::value_to_percent(min_value, max_value, value);
    return syst::success;
}

syst::optional_t<sound_control_t::volume_t>
sound_control_t::get_playback_volume() const {
    long max_value = 0;
    long min_value = 0;
    int snd_errno = snd_mixer_selem_get_playback_volume_range(
      this->impl_->elem, &min_value, &max_value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{
            "ALSA error: snd_mixer_selem_get_playback_volume_range: " }
          + snd_strerror(snd_errno));
    }

    volume_t volume{};

    result_t result = get_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_LEFT,
      min_value,
      max_value,
      volume.front_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_RIGHT,
      min_value,
      max_value,
      volume.front_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_LEFT,
      min_value,
      max_value,
      volume.rear_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_RIGHT,
      min_value,
      max_value,
      volume.rear_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_CENTER,
      min_value,
      max_value,
      volume.front_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_WOOFER,
      min_value,
      max_value,
      volume.woofer);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_SIDE_LEFT,
      min_value,
      max_value,
      volume.side_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_SIDE_RIGHT,
      min_value,
      max_value,
      volume.side_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_CENTER,
      min_value,
      max_value,
      volume.rear_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return volume;
}

result_t get_channel_capture_status(snd_mixer_elem_t* control,
  snd_mixer_selem_channel_id_t channel,
  std::optional<bool>& status) {
    if (snd_mixer_selem_has_capture_channel(control, channel) == 0) {
        status = std::nullopt;
        return syst::success;
    }

    int value = 0;
    int snd_errno =
      snd_mixer_selem_get_capture_switch(control, channel, &value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_get_capture_switch: " }
          + snd_strerror(snd_errno));
    }

    status = (value != 0);
    return syst::success;
}

syst::optional_t<sound_control_t::status_t>
sound_control_t::get_capture_status() const {
    status_t status{};

    result_t result = get_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_LEFT, status.front_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_RIGHT, status.front_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_LEFT, status.rear_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_RIGHT, status.rear_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_CENTER, status.front_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_WOOFER, status.woofer);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_SIDE_LEFT, status.side_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_SIDE_RIGHT, status.side_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_CENTER, status.rear_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return status;
}

result_t get_channel_capture_volume(snd_mixer_elem_t* control,
  snd_mixer_selem_channel_id_t channel,
  long min_value,
  long max_value,
  std::optional<double>& status) {
    if (snd_mixer_selem_has_capture_channel(control, channel) == 0) {
        status = std::nullopt;
        return syst::success;
    }

    long value = 0;
    int snd_errno =
      snd_mixer_selem_get_capture_volume(control, channel, &value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_get_capture_volume: " }
          + snd_strerror(snd_errno));
    }

    status = value_to_percent(min_value, max_value, value);
    return syst::success;
}

syst::optional_t<sound_control_t::volume_t>
sound_control_t::get_capture_volume() const {
    long max_value = 0;
    long min_value = 0;
    int snd_errno = snd_mixer_selem_get_capture_volume_range(
      this->impl_->elem, &min_value, &max_value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{
            "ALSA error: snd_mixer_selem_get_capture_volume_range: " }
          + snd_strerror(snd_errno));
    }

    volume_t volume{};

    result_t result = get_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_LEFT,
      min_value,
      max_value,
      volume.front_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_RIGHT,
      min_value,
      max_value,
      volume.front_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_LEFT,
      min_value,
      max_value,
      volume.rear_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_RIGHT,
      min_value,
      max_value,
      volume.rear_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_CENTER,
      min_value,
      max_value,
      volume.front_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_WOOFER,
      min_value,
      max_value,
      volume.woofer);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_SIDE_LEFT,
      min_value,
      max_value,
      volume.side_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_SIDE_RIGHT,
      min_value,
      max_value,
      volume.side_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = get_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_CENTER,
      min_value,
      max_value,
      volume.rear_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return volume;
}

result_t set_channel_playback_status(snd_mixer_elem_t* control,
  snd_mixer_selem_channel_id_t channel,
  const std::optional<bool>& status) {
    if (! status.has_value()) {
        return syst::success;
    }

    if (snd_mixer_selem_has_playback_channel(control, channel) == 0) {
        return syst::success;
    }

    int value = 0;
    if (status.value()) {
        value = 1;
    }

    int snd_errno =
      snd_mixer_selem_set_playback_switch(control, channel, value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_set_playback_switch: " }
          + snd_strerror(snd_errno));
    }

    return syst::success;
}

result_t sound_control_t::set_playback_status(const status_t& status) {
    result_t result = set_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_LEFT, status.front_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_RIGHT, status.front_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_LEFT, status.rear_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_RIGHT, status.rear_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_CENTER, status.front_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_WOOFER, status.woofer);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_SIDE_LEFT, status.side_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_SIDE_RIGHT, status.side_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_CENTER, status.rear_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return syst::success;
}

result_t sound_control_t::set_playback_status_all(bool status) {
    int value = 0;
    if (status) {
        value = 1;
    }

    int snd_errno =
      snd_mixer_selem_set_playback_switch_all(this->impl_->elem, value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_set_playback_switch_all: " }
          + snd_strerror(snd_errno));
    }

    return syst::success;
}

[[nodiscard]] sound_control_t::status_t toggle_status(
  const sound_control_t::status_t& status) {
    sound_control_t::status_t new_status;

    if (status.front_left.has_value()) {
        new_status.front_left = ! status.front_left.value();
    }

    if (status.front_right.has_value()) {
        new_status.front_right = ! status.front_right.value();
    }

    if (status.rear_left.has_value()) {
        new_status.rear_left = ! status.rear_left.value();
    }

    if (status.rear_right.has_value()) {
        new_status.rear_right = ! status.rear_right.value();
    }

    if (status.front_center.has_value()) {
        new_status.front_center = ! status.front_center.value();
    }

    if (status.woofer.has_value()) {
        new_status.woofer = ! status.woofer.value();
    }

    if (status.side_left.has_value()) {
        new_status.side_left = ! status.side_left.value();
    }

    if (status.side_right.has_value()) {
        new_status.side_right = ! status.side_right.value();
    }

    if (status.rear_center.has_value()) {
        new_status.rear_center = ! status.rear_center.value();
    }

    return new_status;
}

result_t sound_control_t::toggle_playback_status() {
    auto status = this->get_playback_status();
    if (status.has_error()) {
        return SYST_TRACE(status.error());
    }

    auto new_status = syst::toggle_status(status.value());

    auto result = this->set_playback_status(new_status);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return syst::success;
}

result_t set_channel_playback_volume(snd_mixer_elem_t* control,
  snd_mixer_selem_channel_id_t channel,
  long min_value,
  long max_value,
  const std::optional<double>& status) {
    if (! status.has_value()) {
        return syst::success;
    }

    if (snd_mixer_selem_has_playback_channel(control, channel) == 0) {
        return syst::success;
    }

    if (status.value() < static_cast<double>(0)
      || status.value() > static_cast<double>(100)) {
        return SYST_NEW_ERROR(
          "The new status given for the playback volume is out of "
          "bounds.\n\tstatus: '"
          + std::to_string(status.value()) + "'");
    }

    long value = percent_to_value(min_value, max_value, status.value());
    int snd_errno =
      snd_mixer_selem_set_playback_volume(control, channel, value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_set_playback_volume: " }
          + snd_strerror(snd_errno));
    }

    return syst::success;
}

result_t sound_control_t::set_playback_volume(const volume_t& volume) {
    long max_value = 0;
    long min_value = 0;
    int snd_errno = snd_mixer_selem_get_playback_volume_range(
      this->impl_->elem, &min_value, &max_value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{
            "ALSA error: snd_mixer_selem_get_playback_volume_range: " }
          + snd_strerror(snd_errno));
    }

    result_t result = set_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_LEFT,
      min_value,
      max_value,
      volume.front_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_RIGHT,
      min_value,
      max_value,
      volume.front_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_LEFT,
      min_value,
      max_value,
      volume.rear_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_RIGHT,
      min_value,
      max_value,
      volume.rear_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_CENTER,
      min_value,
      max_value,
      volume.front_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_WOOFER,
      min_value,
      max_value,
      volume.woofer);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_SIDE_LEFT,
      min_value,
      max_value,
      volume.side_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_SIDE_RIGHT,
      min_value,
      max_value,
      volume.side_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_playback_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_CENTER,
      min_value,
      max_value,
      volume.rear_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return syst::success;
}

result_t sound_control_t::set_playback_volume_all(double volume) {
    long max_value = 0;
    long min_value = 0;
    int snd_errno = snd_mixer_selem_get_playback_volume_range(
      this->impl_->elem, &min_value, &max_value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{
            "ALSA error: snd_mixer_selem_get_playback_volume_range: " }
          + snd_strerror(snd_errno));
    }

    if (volume < static_cast<double>(0) || volume > static_cast<double>(100)) {
        return SYST_NEW_ERROR(
          "The new status given for the playback volume is out of "
          "bounds.\n\tstatus: '"
          + std::to_string(volume) + "'");
    }

    long value = percent_to_value(min_value, max_value, volume);
    snd_errno =
      snd_mixer_selem_set_playback_volume_all(this->impl_->elem, value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_set_playback_volume_all: " }
          + snd_strerror(snd_errno));
    }

    return syst::success;
}

result_t set_channel_capture_status(snd_mixer_elem_t* control,
  snd_mixer_selem_channel_id_t channel,
  const std::optional<bool>& status) {
    if (! status.has_value()) {
        return syst::success;
    }

    if (snd_mixer_selem_has_capture_channel(control, channel) == 0) {
        return syst::success;
    }

    int value = 0;
    if (status.value()) {
        value = 1;
    }

    int snd_errno = snd_mixer_selem_set_capture_switch(control, channel, value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_set_capture_switch: " }
          + snd_strerror(snd_errno));
    }

    return syst::success;
}

result_t sound_control_t::set_capture_status(const status_t& status) {
    result_t result = set_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_LEFT, status.front_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_RIGHT, status.front_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_LEFT, status.rear_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_RIGHT, status.rear_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_FRONT_CENTER, status.front_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_WOOFER, status.woofer);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_SIDE_LEFT, status.side_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_SIDE_RIGHT, status.side_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_status(
      this->impl_->elem, SND_MIXER_SCHN_REAR_CENTER, status.rear_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return syst::success;
}

result_t sound_control_t::set_capture_status_all(bool status) {
    int value = 0;
    if (status) {
        value = 1;
    }

    int snd_errno =
      snd_mixer_selem_set_capture_switch_all(this->impl_->elem, value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_set_capture_switch_all: " }
          + snd_strerror(snd_errno));
    }

    return syst::success;
}

result_t sound_control_t::toggle_capture_status() {
    auto status = this->get_capture_status();
    if (status.has_error()) {
        return SYST_TRACE(status.error());
    }

    auto new_status = syst::toggle_status(status.value());

    auto result = this->set_capture_status(new_status);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return syst::success;
}

result_t set_channel_capture_volume(snd_mixer_elem_t* control,
  snd_mixer_selem_channel_id_t channel,
  long min_value,
  long max_value,
  const std::optional<double>& status) {
    if (! status.has_value()) {
        return syst::success;
    }

    if (snd_mixer_selem_has_capture_channel(control, channel) == 0) {
        return syst::success;
    }

    if (status.value() < static_cast<double>(0)
      || status.value() > static_cast<double>(100)) {
        return SYST_NEW_ERROR(
          "The new status given for the capture volume is out of "
          "bounds.\n\tstatus: '"
          + std::to_string(status.value()) + "'");
    }

    long value = percent_to_value(min_value, max_value, status.value());
    int snd_errno = snd_mixer_selem_set_capture_volume(control, channel, value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_set_capture_volume: " }
          + snd_strerror(snd_errno));
    }

    return syst::success;
}

result_t sound_control_t::set_capture_volume(const volume_t& volume) {
    long max_value = 0;
    long min_value = 0;
    int snd_errno = snd_mixer_selem_get_capture_volume_range(
      this->impl_->elem, &min_value, &max_value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{
            "ALSA error: snd_mixer_selem_get_capture_volume_range: " }
          + snd_strerror(snd_errno));
    }

    result_t result = set_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_LEFT,
      min_value,
      max_value,
      volume.front_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_RIGHT,
      min_value,
      max_value,
      volume.front_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_LEFT,
      min_value,
      max_value,
      volume.rear_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_RIGHT,
      min_value,
      max_value,
      volume.rear_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_FRONT_CENTER,
      min_value,
      max_value,
      volume.front_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_WOOFER,
      min_value,
      max_value,
      volume.woofer);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_SIDE_LEFT,
      min_value,
      max_value,
      volume.side_left);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_SIDE_RIGHT,
      min_value,
      max_value,
      volume.side_right);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    result = set_channel_capture_volume(this->impl_->elem,
      SND_MIXER_SCHN_REAR_CENTER,
      min_value,
      max_value,
      volume.rear_center);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return syst::success;
}

result_t sound_control_t::set_capture_volume_all(double volume) {
    long max_value = 0;
    long min_value = 0;
    int snd_errno = snd_mixer_selem_get_capture_volume_range(
      this->impl_->elem, &min_value, &max_value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{
            "ALSA error: snd_mixer_selem_get_capture_volume_range: " }
          + snd_strerror(snd_errno));
    }

    if (volume < static_cast<double>(0) || volume > static_cast<double>(100)) {
        return SYST_NEW_ERROR(
          "The new status given for the capture volume is out of "
          "bounds.\n\tstatus: '"
          + std::to_string(volume) + "'");
    }

    long value = percent_to_value(min_value, max_value, volume);
    snd_errno =
      snd_mixer_selem_set_capture_volume_all(this->impl_->elem, value);
    if (snd_errno != 0) {
        return SYST_NEW_ERROR(
          std::string{ "ALSA error: snd_mixer_selem_set_capture_volume_all: " }
          + snd_strerror(snd_errno));
    }

    return syst::success;
}

} // namespace syst
