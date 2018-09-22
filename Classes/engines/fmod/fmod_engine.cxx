// Copyright (c) 2016 Mozart Alexander Louis. All rights reserved.

#include "fmod_engine.hxx"

AudioUtils* AudioUtils::m_instance_ = nullptr;

inline void check_result(const FMOD_RESULT result) { CCASSERT(result == FMOD_OK, "FMOD Asset Failed..."); }

AudioUtils::AudioUtils() : audio_enabled_(true), low_level_system_(nullptr), fmod_system_(nullptr) {}

AudioUtils::~AudioUtils() {
  check_result(low_level_system_->release());
  check_result(fmod_system_->release());
  delete m_instance_;
}

AudioUtils* AudioUtils::getInstance() {
  if (!m_instance_) {
    m_instance_ = new (nothrow) AudioUtils();
    m_instance_->initAudioEngine();
  }
  return m_instance_;
}

void AudioUtils::preloadAudio(const string& path) {
  if (audio_instance_map_iter_ == audio_instance_map_.end()) {
    const auto& instance = getEvent(path.c_str(), true);
    audio_instance_map_.insert(audio_instance_map_iter_, make_pair(path.c_str(), instance));
  }
}

void AudioUtils::playAudio(const char* name, bool persist) {
  if (audio_enabled_) {
    findAudioInstance(name);
    if (audio_instance_map_iter_ == audio_instance_map_.end()) {
      // Create a new audio instance and insert it into our audio instance map.
      const auto& instance = getEvent(name);
      check_result(instance->start());

      // Store the created instance in the audio map if persistence is needed
      // if not, fire and forget.
      if (persist)
        audio_instance_map_.insert(audio_instance_map_iter_, make_pair(name, instance));
      else
        check_result(instance->release());
    } else {
      if (getCurrentlyPlaying() != audio_instance_map_iter_->first)
        check_result(audio_instance_map_iter_->second->start());
    }
  }
}

void AudioUtils::playAudioWithParam(const char* name, const char* param, const float value,
                                    const bool persist) {
  if (audio_enabled_) {
    FMOD::Studio::ParameterInstance* paramInst;

    findAudioInstance(name);
    if (audio_instance_map_iter_ == audio_instance_map_.end()) {
      const auto& instance = getEvent(name);

      check_result(instance->getParameter(param, &paramInst));
      check_result(paramInst->setValue(value));
      check_result(instance->start());

      // Store the created instance in the audio map if persistence is needed
      // if not, fire and forget.
      if (persist)
        audio_instance_map_.insert(audio_instance_map_iter_, make_pair(name, instance));
      else
        check_result(instance->release());
    } else {
      check_result(audio_instance_map_iter_->second->getParameter(param, &paramInst));
      check_result(paramInst->setValue(value));
    }
  }
}

string AudioUtils::getCurrentlyPlaying() {
  for (const auto& itr : audio_instance_map_) {
    FMOD_STUDIO_PLAYBACK_STATE state;
    check_result(itr.second->getPlaybackState(&state));
    if (state == FMOD_STUDIO_PLAYBACK_PLAYING) return itr.first;
  }
  return "";
}

void AudioUtils::pauseAudio(const char* name) {
  findAudioInstance(name);
  if (audio_instance_map_iter_ != audio_instance_map_.end())
    check_result(audio_instance_map_iter_->second->setPaused(true));
}

void AudioUtils::resumeAudio(const char* name) {
  if (audio_enabled_) {
    findAudioInstance(name);
    if (audio_instance_map_iter_ != audio_instance_map_.end())
      check_result(audio_instance_map_iter_->second->setPaused(false));
  }
}

void AudioUtils::stopAudio(const char* name, bool release) {
  findAudioInstance(name);
  if (audio_instance_map_iter_ != audio_instance_map_.end()) {
    check_result(audio_instance_map_iter_->second->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
    if (release) {
      CCLOG("Releasing Audio");
      FMOD::Studio::EventDescription* desc;
      check_result(audio_instance_map_iter_->second->getDescription(&desc));
      check_result(audio_instance_map_iter_->second->release());
      audio_instance_map_.erase(audio_instance_map_iter_);
    }
  }
}

void AudioUtils::setAudioParam(const char* name, const char* param, float value) {
  findAudioInstance(name);
  if (audio_instance_map_iter_ != audio_instance_map_.end()) {
    FMOD::Studio::ParameterInstance* paramInst;
    check_result(audio_instance_map_iter_->second->getParameter(param, &paramInst));
    check_result(paramInst->setValue(value));
  }
}

void AudioUtils::stopAll(bool release) {
  for (const auto& pair : audio_instance_map_) {
    check_result(pair.second->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
    if (release) {
      CCLOG("Releasing Audio");
      FMOD::Studio::EventDescription* desc;
      check_result(pair.second->getDescription(&desc));
      check_result(pair.second->release());
    }
  }

  if (release) audio_instance_map_.clear();
}

void AudioUtils::pauseMixer() const { check_result(low_level_system_->mixerSuspend()); }

void AudioUtils::resumeMixer() const { check_result(low_level_system_->mixerResume()); }

void AudioUtils::setEnabled(bool enable) { audio_enabled_ = enable; }

bool AudioUtils::getEnabled() const { return audio_enabled_; }

void AudioUtils::update() const { check_result(fmod_system_->update()); }

void AudioUtils::findAudioInstance(string name) {
  audio_instance_map_iter_ =
      find_if(audio_instance_map_.begin(), audio_instance_map_.end(),
              [name](pair<string, FMOD::Studio::EventInstance*> audio_id) { return audio_id.first == name; });
}

void AudioUtils::initAudioEngine() {
  check_result(FMOD::Studio::System::create(&fmod_system_));
  check_result(fmod_system_->getLowLevelSystem(&low_level_system_));

  check_result(low_level_system_->setDSPBufferSize(1024, 4));
  check_result(fmod_system_->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));

  FMOD::Studio::Bank* masterBank;
  check_result(fmod_system_->loadBankFile(MASTER_FILE.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));

  FMOD::Studio::Bank* stringsBank;
  check_result(
      fmod_system_->loadBankFile(MASTER_STRINGS_FILE.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
}

FMOD::Studio::EventInstance* AudioUtils::getEvent(const char* event_path, bool preload) const {
  FMOD::Studio::EventDescription* desc;
  check_result(fmod_system_->getEvent(event_path, &desc));

  if (preload) desc->loadSampleData();

  FMOD::Studio::EventInstance* inst = nullptr;
  check_result(desc->createInstance(&inst));

  return inst;
}
