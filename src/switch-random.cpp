#include <random>

#include "headers/advanced-scene-switcher.hpp"

static inline QString MakeRandomSwitchName(const QString &scene,
					   const QString &transition,
					   double &delay)
{
	return QStringLiteral("[") + scene + QStringLiteral(", ") + transition +
	       QStringLiteral("]: ") + QString::number(delay) +
	       QStringLiteral(" seconds");
}
