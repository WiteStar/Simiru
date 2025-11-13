#pragma once
#include "Simiru.h"

template<typename ...Args>
inline void Simiru::addPageNode(const QString& title, PageBase* page, Args ...args)
{
	if (ElaWindow::addPageNode(title, page, args...) == ElaNavigationType::NodeOperateReturnType::Success)
		keyToPage[page->property("ElaPageKey").value<QString>()] = page;
}

template<typename ...Args>
inline void Simiru::addFooterNode(const QString& title, PageBase* page, Args ...args)
{
	if (ElaWindow::addFooterNode(title, page, args...) == ElaNavigationType::NodeOperateReturnType::Success)
		keyToPage[page->property("ElaPageKey").value<QString>()] = page;
}
