#ifndef CUTEHMI_CUTEHMI__MODBUS__1__LIB_INCLUDE_MODBUS_INTERNAL_PLATFORM_HPP
#define CUTEHMI_CUTEHMI__MODBUS__1__LIB_INCLUDE_MODBUS_INTERNAL_PLATFORM_HPP

#include <QtGlobal>

#ifdef CUTEHMI_MODBUS_DYNAMIC
	#ifdef CUTEHMI_MODBUS_BUILD
		//export symbols to dll
		#define CUTEHMI_MODBUS_API Q_DECL_EXPORT
	#else
		//using functions from dll
		#define CUTEHMI_MODBUS_API Q_DECL_IMPORT
	#endif
#else
	#define CUTEHMI_MODBUS_API
#endif

#endif

//(c)MP: Copyright © 2017, Michal Policht. All rights reserved.
//(c)MP: This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
