#pragma once
#include "BNBase.h"
#include "ElaLineEdit.h"

class BNEdit : public BNBase
{
public:
	virtual bool LoadJson(const QJsonObject& obj) override;
	virtual bool SetupPage(PageBase* page) override final;
	virtual QString GetArg() override final;

private:
	ElaLineEdit* line;
};