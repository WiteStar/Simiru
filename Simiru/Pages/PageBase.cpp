#include "stdafx.h"
#include "PageBase.h"

#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaScrollPageArea.h"

PageBase::PageBase(const QString& title, const QString& subtitle, QWidget* parent)
	: PageBase(parent)
{
	setWindowTitle(title);

	QWidget* titleWidget = (QWidget*)contentLayout->parent();
	titleWidget->setWindowTitle(title);
	addCentralWidget(titleWidget, true, true, 0);

	ElaText* subtitleText = new ElaText(this);
	subtitleText->setTextPixelSize(14);
	subtitleText->setText(subtitle);
	customLayout->addWidget(subtitleText);
}

PageBase::PageBase(QWidget* parent)
	: ElaScrollPage(parent)
{
	if (parent != nullptr)
		connect(eTheme, &ElaTheme::themeModeChanged, this, [&]() { update(); });
	setContentsMargins(20, 0, 0, 0);

	suggestBox = new ElaSuggestBox(this);
	suggestBox->setPlaceholderText("Search");
	suggestBox->setFixedHeight(32);
	suggestBox->setEnabled(false);
	suggestBox->setVisible(false);

	QWidget* contentWidget = new QWidget(this);
	contentLayout = new QVBoxLayout(contentWidget);
	contentLayout->setContentsMargins(0, 0, 0, 0);

	QWidget* customWidget = new QWidget(this);
	customLayout = new QVBoxLayout(customWidget);
	customLayout->setContentsMargins(2, 3, 2, 6);
	setCustomWidget(customWidget);

	connect(suggestBox, &ElaSuggestBox::suggestionClicked, this, &PageBase::slotNavigation);
}

PageBase::~PageBase()
{
}

void PageBase::slotNavigation(QString suggestion, QVariantMap data)
{
	if (data["Widget"].isNull()) return;
	QWidget* widget = (QWidget*)data["Widget"].value<uintptr_t>();
	QScrollArea* area = (QScrollArea*)contentLayout->parent(/*widget*/)->parent(/*viewport*/)->parent(/*area*/);
	area->ensureWidgetVisible(widget);
}

void PageBase::registerSuggestion(const QString& suggestion, void* widget)
{
	suggestBox->addSuggestion(suggestion, { { "Widget", (uintptr_t)widget} });
}

QHBoxLayout* PageBase::addGroup()
{
	ElaScrollPageArea* area = new ElaScrollPageArea(this);
	QHBoxLayout* layout = new QHBoxLayout(area);
	contentLayout->addWidget(area);
	return layout;
}

void PageBase::addWidget(const QString& name, QWidget* widget)
{
	QHBoxLayout* group = addGroup();
	registerSuggestion(name, group->parent());
	group->addWidget(widget);
}