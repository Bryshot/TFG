#include "headers/advanced-scene-switcher.hpp"

static inline QString MakeSwitchNameExecutable(const QString &scene,
					       const QString &value,
					       const QString &transition,
					       bool inFocus)
{
	if (!inFocus)
		return QStringLiteral("[") + scene + QStringLiteral(", ") +
		       transition + QStringLiteral("]: ") + value;
	return QStringLiteral("[") + scene + QStringLiteral(", ") + transition +
	       QStringLiteral("]: ") + value +
	       QStringLiteral(" (only if focused)");
}
