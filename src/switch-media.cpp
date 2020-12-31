#include "headers/advanced-scene-switcher.hpp"

static inline QString
MakeMediaSwitchName(const QString &source, const QString &scene,
		    const QString &transition, obs_media_state state,
		    time_restriction restriction, uint64_t time)
{
	QString switchName = QStringLiteral("Switch to ") + scene +
			     QStringLiteral(" using ") + transition +
			     QStringLiteral(" if ") + source +
			     QStringLiteral(" state is ");
	if (state == OBS_MEDIA_STATE_NONE) {
		switchName += QStringLiteral("none");
	} else if (state == OBS_MEDIA_STATE_PLAYING) {
		switchName += QStringLiteral("playing");
	} else if (state == OBS_MEDIA_STATE_OPENING) {
		switchName += QStringLiteral("opening");
	} else if (state == OBS_MEDIA_STATE_BUFFERING) {
		switchName += QStringLiteral("buffering");
	} else if (state == OBS_MEDIA_STATE_PAUSED) {
		switchName += QStringLiteral("paused");
	} else if (state == OBS_MEDIA_STATE_STOPPED) {
		switchName += QStringLiteral("stopped");
	} else if (state == OBS_MEDIA_STATE_ENDED) {
		switchName += QStringLiteral("ended");
	} else if (state == OBS_MEDIA_STATE_ERROR) {
		switchName += QStringLiteral("error");
	}
	if (restriction == TIME_RESTRICTION_SHORTER) {
		switchName += QStringLiteral(" and time shorter than ");
	} else if (restriction == TIME_RESTRICTION_LONGER) {
		switchName += QStringLiteral(" and time longer than ");
	} else if (restriction == TIME_RESTRICTION_REMAINING_SHORTER) {
		switchName +=
			QStringLiteral(" and time remaining shorter than ");
	} else if (restriction == TIME_RESTRICTION_REMAINING_LONGER) {
		switchName +=
			QStringLiteral(" and time remaining longer than ");
	}
	if (restriction != TIME_RESTRICTION_NONE) {
		switchName += std::to_string(time).c_str();
		switchName += QStringLiteral(" ms");
	}
	return switchName;
}
