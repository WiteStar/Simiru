#include "stdafx.h"
#include "Simiru.h"

#include "ElaToolButton.h"
#include "ElaSuggestBox.h"
#include "ElaNavigationRouter.h"

Simiru::Simiru(QWidget* parent)
	: ElaWindow(parent)
{
	initWindow();
	initEdgeLayout();
	initContent();
}

Simiru::~Simiru()
{
}

void Simiru::initWindow()
{
	setUserInfoCardTitle("Simiru");
	setUserInfoCardSubTitle("An exntendable toolbox");
	setUserInfoCardPixmap(QPixmap(":/Resource/Simiru.png"));

	//================================ Set Central Widget Head ================================//
	ElaToolButton* leftButton = new ElaToolButton(this);
	leftButton->setElaIcon(ElaIconType::AngleLeft);	leftButton->setEnabled(false);
	connect(leftButton, &ElaToolButton::clicked, []() { ElaNavigationRouter::getInstance()->navigationRouteBack(); });

	ElaToolButton* rightButton = new ElaToolButton(this);
	rightButton->setElaIcon(ElaIconType::AngleRight); rightButton->setEnabled(false);
	connect(rightButton, &ElaToolButton::clicked, []() { ElaNavigationRouter::getInstance()->navigationRouteForward(); });

	connect(ElaNavigationRouter::getInstance(), &ElaNavigationRouter::navigationRouterStateChanged,
		[leftButton, rightButton](ElaNavigationRouterType::RouteMode routeMode)
		{
			switch (routeMode)
			{
			case ElaNavigationRouterType::BackValid: leftButton->setEnabled(true); break;
			case ElaNavigationRouterType::BackInvalid: leftButton->setEnabled(false); break;
			case ElaNavigationRouterType::ForwardValid: rightButton->setEnabled(true); break;
			case ElaNavigationRouterType::ForwardInvalid: rightButton->setEnabled(false); break;
			}
		}
	);

	searchWidget = new ElaSuggestBox(this);
	searchWidget->setPlaceholderText("Search");
	searchWidget->setFixedHeight(32);
	searchWidget->setEnabled(false);
	searchWidget->setVisible(true);

	QWidget* headWidget = new QWidget(this);
	headLayout = new QHBoxLayout(headWidget);
	headLayout->setContentsMargins(13, 15, 13, 6);
	headLayout->addWidget(leftButton);
	headLayout->addWidget(rightButton);
	headLayout->addWidget(searchWidget);
	headLayout->addStretch();
	setCentralCustomWidget(headWidget);
	//================================ Set Central Widget Head ================================//

	connect(this, &Simiru::navigationNodeClicked, this, &Simiru::slotUpdateSuggestionList);
}

void Simiru::initEdgeLayout()
{
}

void Simiru::initContent()
{
	base = new PageBase("TEST", "test", this);
	addPageNode("HOME", base, ElaIconType::House);
	base = new PageBase("TEST1", "test1", this);
	addPageNode("HOME", base, ElaIconType::House);
}

void Simiru::slotUpdateSuggestionList(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey)
{
	if (keyToPage.contains(nodeKey) == false)
		return;

	PageBase* page = keyToPage[nodeKey];

	searchWidget->setEnabled(false);
	searchWidget->setVisible(false);
	headLayout->removeWidget(searchWidget);
	searchWidget = page->getSuggestBox();
	headLayout->insertWidget(2, searchWidget);
	searchWidget->setVisible(true);
	searchWidget->setEnabled(true);
}