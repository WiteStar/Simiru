#pragma once

#include "ElaScrollPage.h"
#include "ElaSuggestBox.h"

class PageBase : public ElaScrollPage
{
	Q_OBJECT

public:
	PageBase(const QString& title, const QString& subtitle, QWidget* parent = nullptr);
	PageBase(QWidget* parent = nullptr);
	~PageBase();

	void registerSuggestion(const QString& suggestion, void* widget);

	QHBoxLayout* addGroup();
	void addWidget(const QString& name, QWidget* widget);
	inline ElaSuggestBox* getSuggestBox() { return suggestBox; }

public slots:
	void slotNavigation(QString suggestion, QVariantMap data);

private:
	QVBoxLayout* contentLayout;
	QVBoxLayout* customLayout;
	ElaSuggestBox* suggestBox;
};