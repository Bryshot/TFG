#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>
#include <curl/curl.h>

#include "headers/advanced-scene-switcher.hpp"
#include "headers/curl-helper.hpp"

#define LOCAL_FILE_IDX 0
#define REMOTE_FILE_IDX 1


static inline QString MakeFileSwitchName(const QString &scene,
					 const QString &transition,
					 const QString &fileName,
					 const QString &text, bool useRegex,
					 bool useTime)
{
	QString switchName = QStringLiteral("Switch to ") + scene +
			     QStringLiteral(" using ") + transition +
			     QStringLiteral(" if ") + fileName;
	if (useTime)
		switchName += QStringLiteral(" was modified and");
	switchName += QStringLiteral(" contains");
	if (useRegex)
		switchName += QStringLiteral(" (RegEx): \n\"");
	else
		switchName += QStringLiteral(": \n\"");
	if (text.length() > 30)
		switchName += text.left(27) + QStringLiteral("...\"");
	else
		switchName += text + QStringLiteral("\"");

	return switchName;
}
