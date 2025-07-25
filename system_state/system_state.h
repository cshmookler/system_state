/*****************************************************************************/
/*  Copyright (c) 2025 Caden Shmookler                                       */
/*                                                                           */
/*  This software is provided 'as-is', without any express or implied        */
/*  warranty. In no event will the authors be held liable for any damages    */
/*  arising from the use of this software.                                   */
/*                                                                           */
/*  Permission is granted to anyone to use this software for any purpose,    */
/*  including commercial applications, and to alter it and redistribute it   */
/*  freely, subject to the following restrictions:                           */
/*                                                                           */
/*  1. The origin of this software must not be misrepresented; you must not  */
/*     claim that you wrote the original software. If you use this software  */
/*     in a product, an acknowledgment in the product documentation would    */
/*     be appreciated but is not required.                                   */
/*  2. Altered source versions must be plainly marked as such, and must not  */
/*     be misrepresented as being the original software.                     */
/*  3. This notice may not be removed or altered from any source             */
/*     distribution.                                                         */
/*****************************************************************************/

/**
 * @file system_state.h
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Core utilities for fetching and modifying the system state.
 */

#ifndef SYST_SYSTEM_STATE_H
#define SYST_SYSTEM_STATE_H

#ifdef __cplusplus
#include "system_state.hpp"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void syst_string_free(char* error);

#ifdef __cplusplus
using syst_backlight_t = syst::backlight_t;
using syst_backlight_list_t = std::vector<syst_backlight_t>;
#else
typedef struct syst_backlight_t syst_backlight_t;
typedef struct syst_backlight_list_t syst_backlight_list_t;
#endif
char* syst_get_backlights(syst_backlight_list_t** backlight_list);
char* syst_backlight_list_get_size(
  syst_backlight_list_t* backlight_list, unsigned long* size);
char* syst_backlight_list_get(syst_backlight_list_t* backlight_list,
  unsigned long index,
  syst_backlight_t** backlight);
void syst_backlight_list_free(syst_backlight_list_t* backlight_list);
char* syst_backlight_get_sysfs_path(syst_backlight_t* backlight, char** path);
char* syst_backlight_get_name(syst_backlight_t* backlight, char** name);
char* syst_backlight_get_brightness(
  syst_backlight_t* backlight, double* brightness);
char* syst_backlight_set_brightness(
  syst_backlight_t* backlight, double brightness);
char* syst_backlight_set_brightness_relative(
  syst_backlight_t* backlight, double brightness);

#ifdef __cplusplus
using syst_sound_mixer_t = syst::sound_mixer_t;
#else
typedef struct syst_sound_mixer_t syst_sound_mixer_t;
#endif
char* syst_get_sound_mixer(syst_sound_mixer_t** sound_mixer);

#ifdef __cplusplus
using syst_sound_control_t = syst::sound_control_t;
using syst_sound_control_list_t = std::vector<syst_sound_control_t>;
#else
typedef struct syst_sound_control_t syst_sound_control_t;
typedef struct syst_sound_control_list_t syst_sound_control_list_t;
#endif
char* syst_sound_mixer_get_controls(syst_sound_mixer_t* sound_mixer,
  syst_sound_control_list_t** sound_control_list);
void syst_sound_mixer_free(syst_sound_mixer_t* sound_mixer);
char* syst_sound_control_list_get_size(
  syst_sound_control_list_t* sound_control_list, unsigned long* size);
char* syst_sound_control_list_get(syst_sound_control_list_t* sound_control_list,
  unsigned long index,
  syst_sound_control_t** sound_control);
void syst_sound_control_list_free(
  syst_sound_control_list_t* sound_control_list);
char* syst_sound_control_get_name(
  syst_sound_control_t* sound_control, char** name);
char* syst_sound_control_has_playback_status(
  syst_sound_control_t* sound_control, int* has_playback_status);
char* syst_sound_control_has_playback_volume(
  syst_sound_control_t* sound_control, int* has_playback_volume);
char* syst_sound_control_has_capture_status(
  syst_sound_control_t* sound_control, int* has_capture_status);
char* syst_sound_control_has_capture_volume(
  syst_sound_control_t* sound_control, int* has_capture_volume);
char* syst_sound_control_set_playback_status_all(
  syst_sound_control_t* sound_control, int status);
char* syst_sound_control_toggle_playback_status(
  syst_sound_control_t* sound_control);
char* syst_sound_control_set_playback_volume_all(
  syst_sound_control_t* sound_control, double volume);
char* syst_sound_control_set_playback_volume_all_relative(
  syst_sound_control_t* sound_control, double volume);
char* syst_sound_control_set_capture_status_all(
  syst_sound_control_t* sound_control, int status);
char* syst_sound_control_toggle_capture_status(
  syst_sound_control_t* sound_control);
char* syst_sound_control_set_capture_volume_all(
  syst_sound_control_t* sound_control, double volume);
char* syst_sound_control_set_capture_volume_all_relative(
  syst_sound_control_t* sound_control, double volume);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // SYST_SYSTEM_STATE_H
