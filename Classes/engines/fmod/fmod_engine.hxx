/** Copyright (c) 2016 Mozart Alexander Louis. All rights reserved. */

#ifndef __AUDIO_UTILS_HXX__
#define __AUDIO_UTILS_HXX__

#include "fmod_studio.hpp"
#include "globals.hxx"

/**
 * Map of audio instance pointers that use a a string to reference them.
 */
using AudioMap = unordered_map<string, FMOD::Studio::EventInstance *>;

/**
 * Manages all audio for this game. Uses that FMOD and FMOD Studio library to play audio.FMOD
 * provides a powerful low level interface combined with FMOD Studio, which provides advanced
 * audio manipulation.
 */
class AudioUtils {
 public:
  /**
   * Constructor.
   */
  AudioUtils();

  /**
   * Destructor.
   */
  virtual ~AudioUtils();

  /**
   * Caches audio instances from the FMOD bank. This enables faster loading
   * of audio.
   */
  void preloadAudio(const string &path);

  /**
   * Plays audio by name.
   */
  void playAudio(const char *name, bool persist = true);

  /**
   * Plays audio and set a specified parameter to `value`.
   */
  void playAudioWithParam(const char *name, const char *param, float value, bool persist = true);

  /**
   * Get the name of the audio that is currently playing.
   */
  string getCurrentlyPlaying();

  /**
   * Pause audio.
   */
  void pauseAudio(const char *name);

  /**
   * Resume audio.
   */
  void resumeAudio(const char *name);

  /**
   * Stops audio.
   */
  void stopAudio(const char *name, bool clean = false);

  /**
   * Sets a audio parameter tat is created in FMOD Studio.
   */
  void setAudioParam(const char *name, const char *param, float value);

  /**
   * Stops all audio instances.
   */
  void stopAll(bool release = false);

  /**
   * Pauses the main output thread
   */
  void pauseMixer() const;

  /**
   * Resumes the main output thread
   */
  void resumeMixer() const;

  /**
   * Setter for `audio_enabled_`
   */
  void setEnabled(bool enable);

  /**
   * Gets `audio_enabled_`
   */
  bool getEnabled() const;

  /**
   * Updates the main mixer thread to sync with game play.
   */
  void update() const;

  /**
   * Gets singleton instance of `AudioUtils`.
   */
  static AudioUtils *getInstance();

 private:
  /**
   * Finds `name`s audio-id inside the `audio_instance_map_`.
   */
  void findAudioInstance(string name);

  /**
   * Initializes FMOD and load the master bank.
   */
  void initAudioEngine();

  /**
   * Gets and audio instance using the `event_path`.
   */
  FMOD::Studio::EventInstance *getEvent(const char *event_path, bool preload = false) const;

  // Whether audio should play or not.
  bool audio_enabled_;

  // FMOD low level system.
  FMOD::System *low_level_system_;

  // FMOD Studio instance.
  FMOD::Studio::System *fmod_system_;

  // A key/pair string to audio instance map. Holds all of the audio instance pointers.
  AudioMap audio_instance_map_;

  // `AudioMap` iterator used to find audio instances.
  AudioMap::iterator audio_instance_map_iter_;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  string MASTER_FILE = "file:///android_asset/audio/Evermaze.bank";
  string MASTER_STRINGS_FILE = "file:///android_asset/audio/Evermaze.strings.bank";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  string MASTER_FILE = FileUtils::getInstance()->fullPathForFilename("audio/Evermaze.bank");
  string MASTER_STRINGS_FILE = FileUtils::getInstance()->fullPathForFilename("audio/Evermaze.strings.bank");
#else
  string MASTER_FILE = "audio/Evermaze.bank";
  string MASTER_STRINGS_FILE = "audio/Evermaze.strings.bank";
#endif

  // Singleton instance of `AudioUtils`.
  static AudioUtils *m_instance_;
};

#endif  // __AUDIO_UTILS_HXX__
