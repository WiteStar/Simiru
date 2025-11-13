#include "stdafx.h"
#include "PageBase.h"

#include "ElaText.h"
#include "ElaTheme.h"

PageBase::PageBase(const QString& title, const QString& subtitle, QWidget* parent)
	: PageBase(parent)
{
	setWindowTitle(title);

	QWidget* titleWidget = new QWidget(this);
	titleWidget->setWindowTitle(title);
	QVBoxLayout* titleLayout = new QVBoxLayout(titleWidget);
	titleLayout->setContentsMargins(0, 0, 0, 0);
	titleLayout->addStretch();
	addCentralWidget(titleWidget, true, true, 0);

	ElaText* subtitleWidget = new ElaText(this);
	subtitleWidget->setTextPixelSize(12);
	subtitleWidget->setText(subtitle);
	layout->addWidget(subtitleWidget);
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
	layout = new QVBoxLayout(contentWidget);
	layout->setContentsMargins(2, 2, 0, 0);
	setCustomWidget(contentWidget);
}

PageBase::~PageBase()
{
}

void PageBase::addWidget(const QString& name, QWidget* widget)
{
	suggestBox->addSuggestion(name, { { "Widget", (uintptr_t)widget} });
	layout->addWidget(widget);
}