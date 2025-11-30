#pragma once
#include "BNBase.h"

#include "ElaSpinBox.h"

// TODO: double and date spin
class BNSpin : public BNBase
{
public:
	virtual bool LoadJson(const QJsonObject& obj) override;
	virtual bool SetupPage(PageBase* page) override final;
	virtual QString GetArg() override final;

private:
	ElaSpinBox* spin;
	int min, max;
	union
	{
		int default_value;
		int value;
	};
};