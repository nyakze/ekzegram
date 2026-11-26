/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "ui/effects/outline_segments.h"
#include "ui/userpic_view.h"


#include <QtCore/QtMath> // M_PI
#include <algorithm>
#include <cmath>

namespace Ui {
namespace {

[[nodiscard]] QPointF SquareArcPoint(const QRectF &oval, float64 angle) {
	const auto rad = M_PI / 2. - (angle * M_PI / 180.);
	const auto r = oval.width() / 2.;
	const auto c = std::cos(rad);
	const auto s = std::sin(rad);
	const auto scale = r / std::max(std::abs(c), std::abs(s));
	return QPointF(oval.x() + r + scale * c, oval.y() + r + scale * s);
}

// FORK (ekzegram): square avatars setting, adapted from Forkgram.
void DrawSquareArc(
		QPainter &p,
		const QRectF &oval,
		int startAngle,
		int spanAngle) {
	const auto from = float64((startAngle / 16) + 90);
	const auto till = from + (spanAngle / 16);
	auto path = QPainterPath();
	path.moveTo(SquareArcPoint(oval, from));
	for (auto i = 0; i != 4; ++i) {
		const auto corner = 225. + (90. * i);
		if (corner > from && corner < till) {
			path.lineTo(SquareArcPoint(oval, corner));
		}
	}
	path.lineTo(SquareArcPoint(oval, till));
	p.drawPath(path);
}

} // namespace

void PaintOutlineSegments(
		QPainter &p,
		QRectF ellipse,
		const std::vector<OutlineSegment> &segments,
		float64 fromFullProgress) {
	Expects(!segments.empty());

	p.setBrush(Qt::NoBrush);
	const auto count = std::min(int(segments.size()), kOutlineSegmentsMax);
	if (count == 1) {
		p.setPen(QPen(segments.front().brush, segments.front().width));
		if (::Ui::SquareUserpics()) {
			p.drawRect(ellipse);
		} else {
			p.drawEllipse(ellipse);
		}
		return;
	}
	const auto small = 160;
	const auto full = arc::kFullLength;
	const auto separator = (full > 1.1 * small * count)
		? small
		: (full / (count * 1.1));
	const auto left = full - (separator * count);
	const auto length = left / float64(count);
	const auto spin = separator * (1. - fromFullProgress);

	auto start = 0. + (arc::kQuarterLength + (separator / 2)) + (3. * spin);
	auto pen = QPen(
		segments.back().brush,
		segments.back().width,
		Qt::SolidLine,
		Qt::RoundCap);
	p.setPen(pen);
	for (auto i = 0; i != count;) {
		const auto &segment = segments[count - (++i)];
		if (!segment.width) {
			start += length + separator;
			continue;
		} else if (pen.brush() != segment.brush
			|| pen.widthF() != segment.width) {
			pen = QPen(
				segment.brush,
				segment.width,
				Qt::SolidLine,
				Qt::RoundCap);
			p.setPen(pen);
		}
		const auto from = int(base::SafeRound(start));
		const auto till = start + length;
		auto added = spin;
		for (; i != count;) {
			start += length + separator;
			const auto &next = segments[count - (++i)];
			if (next.width) {
				--i;
				break;
			}
			added += (separator + length) * (1. - fromFullProgress);
		}
		const auto span = int(base::SafeRound(till + added)) - from;
		if (::Ui::SquareUserpics()) {
			DrawSquareArc(p, ellipse, from, span);
		} else {
			p.drawArc(ellipse, from, span);
		}
	}
}

void PaintOutlineSegments(
		QPainter &p,
		QRectF rect,
		float64 radius,
		const std::vector<OutlineSegment> &segments) {
	Expects(!segments.empty());

	p.setBrush(Qt::NoBrush);
	const auto count = std::min(int(segments.size()), kOutlineSegmentsMax);
	if (count == 1 || true) {
		p.setPen(QPen(segments.back().brush, segments.back().width));
		p.drawRoundedRect(rect, radius, radius);
		return;
	}
}

QLinearGradient UnreadStoryOutlineGradient(
		QRectF rect,
		const QColor &c1,
		const QColor &c2) {
	auto result = QLinearGradient(rect.topRight(), rect.bottomLeft());
	result.setStops({ { 0., c1 }, { 1., c2 } });
	return result;
}

QLinearGradient UnreadStoryOutlineGradient(QRectF rect) {
	return UnreadStoryOutlineGradient(
		std::move(rect),
		st::groupCallLive1->c,
		st::groupCallMuted1->c);
}

} // namespace Ui
