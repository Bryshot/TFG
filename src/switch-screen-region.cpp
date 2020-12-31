#include <QTimer>

#include "headers/advanced-scene-switcher.hpp"



static inline QString MakeScreenRegionSwitchName(const QString &scene,
						 const QString &transition,
						 int minX, int minY, int maxX,
						 int maxY)
{
	return QStringLiteral("[") + scene + QStringLiteral(", ") + transition +
	       QStringLiteral("]: ") + QString::number(minX) +
	       QStringLiteral(", ") + QString::number(minY) +
	       QStringLiteral(" x ") + QString::number(maxX) +
	       QStringLiteral(", ") + QString::number(maxY);
}
