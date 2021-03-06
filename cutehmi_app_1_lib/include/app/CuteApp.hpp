#ifndef CUTEHMI_CUTEHMI__APP__1__LIB_INCLUDE_APP_CUTEAPP_HPP
#define CUTEHMI_CUTEHMI__APP__1__LIB_INCLUDE_APP_CUTEAPP_HPP

#include "internal/common.hpp"

#include <QApplication>
#include <QTime>
#include <QTimer>

namespace cutehmi {
namespace app {

class CUTEHMI_APP_API CuteApp:
	//<workaround id="cutehmi_app_1_lib-1" target="Qt" cause="bug">
	public QApplication
	// Instead of:
	// public QGuiApplication
	//</workaround>
{
	typedef QApplication Parent;

	Q_OBJECT

	public:
		//<workaround ref="QML_CuteHMI_LockScreen-1" target="Qt" cause="design">
		static constexpr int IDLE_MEASURE_INTERVAL = 1000;	///< Idle measurement update interval [milliseconds].
		//</workaround>

		//<workaround ref="QML_CuteHMI_LockScreen-1" target="Qt" cause="design">
		Q_PROPERTY(double idle READ idle NOTIFY idleChanged)

		Q_PROPERTY(bool idleMeasureEnabled READ idleMeasureEnabled WRITE setIdleMeasureEnabled NOTIFY idleMeasureEnabledChanged)
		//</workaround>

		CuteApp(int & argc, char ** argv);

		//<workaround ref="QML_CuteHMI_LockScreen-1" target="Qt" cause="design">
		double idle() const;

		bool idleMeasureEnabled() const;

		void setIdleMeasureEnabled(bool idleMeasureEnabled);

		bool notify(QObject * receiver, QEvent * event) override;
		//</workaround>

	signals:
		//<workaround ref="QML_CuteHMI_LockScreen-1" target="Qt" cause="design">
		void idleChanged();

		void idleMeasureEnabledChanged();
		//</workaround>

	private slots:
		//<workaround ref="QML_CuteHMI_LockScreen-1" target="Qt" cause="design">
		void updateIdleTime();
		//</workaround>

	private:
		//<workaround ref="QML_CuteHMI_LockScreen-1" target="Qt" cause="design">
		void setIdle(double idle);
		//</workaround>

	private:
		struct Members
		{
			//<workaround ref="QML_CuteHMI_LockScreen-1" target="Qt" cause="design">
			int notifyEventCtr;
			double idle;	///< Idle time [seconds].
			bool idleMeasureEnabled;
			QTimer updateTimer;
			QTime idleTime;
			//</workaround>

			Members();
		};

		utils::MPtr<Members> m;
};

}
}

#endif
