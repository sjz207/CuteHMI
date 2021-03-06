#ifndef CUTEHMI_CUTEHMI__CHARTS__1__LIB_INCLUDE_CHARTS_POINTSERIES_HPP
#define CUTEHMI_CUTEHMI__CHARTS__1__LIB_INCLUDE_CHARTS_POINTSERIES_HPP

#include "internal/common.hpp"

#include <QPoint>
#include <QVector>
#include <QObject>

namespace cutehmi {
namespace charts {

class CUTEHMI_CHARTS_API PointSeries:
	public QObject
{
	Q_OBJECT

	public:
		typedef QVector<QPointF> DataContainer;

		PointSeries(QObject * parent = nullptr);

		~PointSeries() override;

		int count() const;

		void append(const QPointF & point);

		const DataContainer & data() const;

		void setData(const DataContainer & data);

		void clear();

		//shift(const QPoint & point) // remove first and append point

		const QPointF * rawData() const;

	signals:
		void dataChanged();

	private:
		struct Members
		{
			DataContainer data;
		};

		utils::MPtr<Members> m;
};

}
}

#endif

//(c)MP: Copyright © 2017, Michal Policht. All rights reserved.
//(c)MP: This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
