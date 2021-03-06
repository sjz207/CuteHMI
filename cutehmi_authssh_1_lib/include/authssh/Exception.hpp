#ifndef CUTEHMI_CUTEHMI__AUTHSSH__1__LIB_INCLUDE_AUTHSSH_EXCEPTION_HPP
#define CUTEHMI_CUTEHMI__AUTHSSH__1__LIB_INCLUDE_AUTHSSH_EXCEPTION_HPP

#include "internal/common.hpp"

#include <base/ExceptionMixin.hpp>

namespace cutehmi {
namespace authssh {

class CUTEHMI_AUTHSSH_API Exception:
	public base::ExceptionMixin<Exception>
{
	typedef base::ExceptionMixin<Exception> Parent;

	public:
		using Parent::Parent;

		~Exception() override = default;
};

}
}

#endif

//(c)MP: Copyright © 2017, Michal Policht. All rights reserved.
//(c)MP: This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
