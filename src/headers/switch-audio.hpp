#pragma once
#include <string>
#include "utility.hpp"
#include "switch-generic.hpp"

constexpr auto audio_func = 8;
constexpr auto default_priority_8 = audio_func;


static inline QString MakeAudioSwitchName(const QString &scene,
					  const QString &transition,
					  const QString &audioSrouce,
					  const int &volume);
