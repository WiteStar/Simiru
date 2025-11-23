#pragma once
#include "BNBase.h"

struct BNText : public BNBase
{
	virtual bool LoadJson(const QJsonObject& obj) override;
	virtual bool SetupPage(PageBase* page);
	virtual QString GetArg();
};