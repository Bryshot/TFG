#include "headers/advanced-scene-switcher.hpp"


static inline QString MakeTimeSwitchName(const QString &scene,
					 const QString &transition,
					 const timeTrigger &trigger,
					 const QTime &time)
{
	QString switchName;

	switch (trigger) {
	case ANY_DAY:
		switchName = QStringLiteral("On any weekday");
		break;
	case MONDAY:
		switchName = QStringLiteral("Mondays");
		break;
	case TUSEDAY:
		switchName = QStringLiteral("Tusedays");
		break;
	case WEDNESDAY:
		switchName = QStringLiteral("Wednesdays");
		break;
	case THURSDAY:
		switchName = QStringLiteral("Thursdays");
		break;
	case FRIDAY:
		switchName = QStringLiteral("Fridays");
		break;
	case SATURDAY:
		switchName = QStringLiteral("Saturdays");
		break;
	case SUNDAY:
		switchName = QStringLiteral("Sundays");
		break;
	case LIVE:
		switchName = QStringLiteral(
			"Relative to starting streaming / recording");
		break;
	}
	switchName += QStringLiteral(" at ") + time.toString() +
		      QStringLiteral(" switch to ") + scene +
		      QStringLiteral(" using ") + transition;
	return switchName;
}
