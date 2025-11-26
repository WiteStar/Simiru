#pragma once
#include "BNBase.h"
#include "ElaLineEdit.h"

struct BNEdit : public BNBase
{
	ElaLineEdit* line;

	virtual bool LoadJson(const QJsonObject& obj) override;
	virtual bool SetupPage(PageBase* page);
	virtual QString GetArg();
};