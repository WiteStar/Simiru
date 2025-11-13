#pragma once

#include <QtWidgets/QMainWindow>

#include "ElaWindow.h"
#include "ElaSuggestBox.h"
#include "ElaContentDialog.h"

#include "Pages/PageBase.h"

class Simiru : public ElaWindow
{
	Q_OBJECT

public:
	Simiru(QWidget* parent = nullptr);
	~Simiru();

private:
	void initWindow();
	void initEdgeLayout();
	void initContent();

public:
	template<typename... Args>
	inline void addPageNode(const QString& title, PageBase* page, Args... args);

	template<typename... Args>
	inline void addFooterNode(const QString& title, PageBase* page, Args... args);

private slots:
	void slotUpdateSuggestionList(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);

private:
	PageBase* base;
	ElaSuggestBox* searchWidget;
	QHBoxLayout* headLayout;
	QHash<QString, PageBase*> keyToPage;
};

#include "Simiru.hpp"