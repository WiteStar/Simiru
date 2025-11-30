#include "stdafx.h"
#include "BNSpin.h"

#include "ElaText.h"
#include "ElaToggleSwitch.h"
#include "JsonUtils.h"

bool BNSpin::LoadJson(const QJsonObject& obj)
{
	if (BNBase::LoadJson(obj) == false)
		return false;

	PARSE_NUMBER(min);
	PARSE_NUMBER(max);
	PARSE_NUMBER(default_value);

	return true;
}

bool BNSpin::SetupPage(PageBase* page)
{
	QHBoxLayout* layout = StartPage(page);
	QWidget* widget = (QWidget*)layout->parent();

	spin = new ElaSpinBox(widget);
	spin->setEnabled(enabled_by_default);
	spin->setValue(default_value);
	spin->setRange(min, max);

	QObject::connect(enabler, &ElaToggleSwitch::toggled, spin, &ElaSpinBox::setEnabled);
	QObject::connect(spin, &ElaSpinBox::valueChanged, [&](int v) { value = v; });
	layout->addWidget(spin);
	layout->addStretch();
	return true;
}

QString BNSpin::GetArg()
{
	return isEnabled ? arg + ' ' + QString::number(value) : QString();
}