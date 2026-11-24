/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL

The markdown export is adapted from Forkgram (https://github.com/forkgram/tdesktop).
*/
#pragma once

#include "base/flat_map.h"
#include "base/flat_set.h"
#include "base/object_ptr.h"
#include "ui/text/text_selection.h"

class HistoryItem;

namespace Ui {
class BoxContent;
class PopupMenu;
} // namespace Ui

namespace HistoryView {
struct SelectedItem;
} // namespace HistoryView

namespace Window {
class SessionController;
} // namespace Window

namespace Menu {

[[nodiscard]] object_ptr<Ui::BoxContent> MakeMarkdownClipboardTextBox();

void AddSaveToMarkdownFileAction(
	not_null<Ui::PopupMenu*> menu,
	not_null<Window::SessionController*> controller,
	const base::flat_map<HistoryItem*, TextSelection, std::less<>> &items);

void AddSaveToMarkdownFileAction(
	not_null<Ui::PopupMenu*> menu,
	not_null<Window::SessionController*> controller,
	const std::vector<HistoryView::SelectedItem> &selectedItems);

void AddSaveToMarkdownFileAction(
	not_null<Ui::PopupMenu*> menu,
	not_null<Window::SessionController*> controller,
	const base::flat_set<not_null<HistoryItem*>, std::less<>> &items);

} // namespace Menu
