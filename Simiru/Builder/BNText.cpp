#include "stdafx.h"
#include "BNText.h"

#include "ElaText.h"
#include "ElaToggleSwitch.h"

bool BNText::LoadJson(const QJsonObject& obj)
{
	if (BNBase::LoadJson(obj) == false)
		return false;

	return true;
}

bool BNText::SetupPage(PageBase* page)
{
	QHBoxLayout* layout = StartPage(page);
	FinishPage(layout);
	return true;
}

QString BNText::GetArg()
{
	return isEnabled ? arg : QString();
}