#pragma once
#include "BNBase.h"

class BNText : public BNBase
{
public:
	virtual bool LoadJson(const QJsonObject& obj) override;
	virtual bool SetupPage(PageBase* page) override final;
	virtual QString GetArg() override final;
};