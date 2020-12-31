#include "headers/advanced-scene-switcher.hpp"
#include "headers/volume-control.hpp"

static inline QString MakeAudioSwitchName(const QString &scene,
					  const QString &transition,
					  const QString &audioSrouce,
					  const int &volume)
{
	QString switchName = QStringLiteral("When volume of ") + audioSrouce +
			     QStringLiteral(" is above ") +
			     QString::number(volume) +
			     QStringLiteral("% switch to ") + scene +
			     QStringLiteral(" using ") + transition;
	return switchName;
}
