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

	ElaText* subtitleText = new ElaText(this);
	subtitleText->setTextPixelSize(12);
	subtitleText->setText(subtitle);

	QWidget* subtitleWidget = new QWidget(this);
	QVBoxLayout* subtitleLayout = new QVBoxLayout(subtitleWidget); 
	subtitleLayout->setContentsMargins(2, 2, 0, 0);
	subtitleLayout->addWidget(subtitleText);
	setCustomWidget(subtitleWidget);
}

PageBase::PageBase(QWidget* parent)
	: ElaScrollPage(parent)
{
	if (parent != nullptr)
		connect(eTheme, &ElaTheme::themeModeChanged, this, [&]() { update(); });
	setContentsMargins(20, 0, 0, 0);
}

PageBase::~PageBase()
{
}