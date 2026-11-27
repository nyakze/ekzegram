/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "ui/controls/window_update_bar.h"

#include "lang/lang_keys.h"
#include "ui/painter.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "styles/style_window.h"

namespace Ui {
namespace {

class UpdateBar final : public RpWidget {
public:
	UpdateBar(
		not_null<QWidget*> parent,
		const QString &tag,
		Fn<void()> open,
		Fn<void()> close);

	int resizeGetHeight(int newWidth) override;

protected:
	void paintEvent(QPaintEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;

private:
	const Fn<void()> _open;
	object_ptr<FlatLabel> _title;
	object_ptr<FlatLabel> _details;
	object_ptr<IconButton> _close;

};

UpdateBar::UpdateBar(
	not_null<QWidget*> parent,
	const QString &tag,
	Fn<void()> open,
	Fn<void()> close)
: RpWidget(parent)
, _open(std::move(open))
, _title(
	this,
	rpl::single(tr::lng_update_available(tr::now, lt_tag, tag)),
	st::windowOutdatedTitle)
, _details(this, tr::lng_update_open_release(), st::windowOutdatedDetails)
, _close(this, st::windowOutdatedClose) {
	_title->setTryMakeSimilarLines(true);
	_details->setTryMakeSimilarLines(true);
	_title->setAttribute(Qt::WA_TransparentForMouseEvents);
	_details->setAttribute(Qt::WA_TransparentForMouseEvents);
	_close->setClickedCallback(std::move(close));
	setCursor(Qt::PointingHandCursor);
}

int UpdateBar::resizeGetHeight(int newWidth) {
	const auto padding = st::windowOutdatedPadding;
	const auto skip = _close->width();
	const auto available = newWidth - 2 * skip;

	_title->resizeToWidth(available);
	_title->moveToLeft(skip, padding.top(), newWidth);

	_details->resizeToWidth(available);
	_details->moveToLeft(
		skip,
		_title->y() + _title->height() + st::windowOutdatedSkip,
		newWidth);

	_close->moveToRight(0, 0, newWidth);

	return _details->y() + _details->height() + padding.bottom();
}

void UpdateBar::paintEvent(QPaintEvent *e) {
	QPainter(this).fillRect(e->rect(), st::activeButtonBg);
}

void UpdateBar::mouseReleaseEvent(QMouseEvent *e) {
	if (!QRect(_close->pos(), _close->size()).contains(e->pos())) {
		_open();
	}
}

} // namespace

object_ptr<RpWidget> CreateUpdateBar(
		not_null<QWidget*> parent,
		const QString &tag,
		Fn<void()> open,
		Fn<void()> close) {
	return object_ptr<UpdateBar>(
		parent,
		tag,
		std::move(open),
		std::move(close));
}

} // namespace Ui
