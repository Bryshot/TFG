#include "headers/advanced-scene-switcher.hpp"



static inline QString MakeWindowSwitchName(const QString &scene,
					   const QString &value,
					   const QString &transition,
					   bool fullscreen, bool maximized,
					   bool focus)
{
	QString name = QStringLiteral("[") + scene + QStringLiteral(", ") +
		       transition + QStringLiteral("]: ") + value;

	if (fullscreen || maximized || focus) {
		name += QStringLiteral(" (only if");

		if (fullscreen)
			name += QStringLiteral(" fullscreen ");
		if (maximized)
			name += QStringLiteral(" maximized ");
		if (focus)
			name += QStringLiteral(" focused");

		name += QStringLiteral(")");
	}

	return name;
}
