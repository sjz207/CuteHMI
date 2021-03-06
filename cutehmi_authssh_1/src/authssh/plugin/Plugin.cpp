#include "macros.hpp"
#include "Plugin.hpp"
#include "PluginNodeData.hpp"
#include "AuthSSHNodeData.hpp"

#include <authssh/Auth.hpp>

#include <base/XMLBackendPlugin.hpp>
#include <base/Exception.hpp>

#include <QtDebug>

namespace cutehmi {
namespace authssh {
namespace plugin {

void Plugin::init(base::ProjectNode & node)
{
    std::unique_ptr<PluginNodeData> pluginNodeData(new PluginNodeData(this));
    node.addExtension(pluginNodeData->xmlBackendPlugin());
    node.data().append(std::move(pluginNodeData));
}

void Plugin::readXML(QXmlStreamReader & xmlReader, base::ProjectNode & node)
{
	CUTEHMI_AUTHSSH_1_QDEBUG("Plugin 'cutehmi_aauthssh_1' starts parsing its own portion of document...");

    QStringList supportedVersions;
    supportedVersions << "http://michpolicht.github.io/CuteHMI/cutehmi_authssh_1/xsd/1.0/";

    base::xml::ParseHelper helper(& xmlReader, supportedVersions);
	helper << base::xml::ParseElement("cutehmi_authssh_1", 1, 1);

    while (helper.readNextRecognizedElement()) {
        if (xmlReader.name() == "cutehmi_authssh_1") {
            base::xml::ParseHelper nodeHelper(& helper);
			nodeHelper << base::xml::ParseElement("auth", { base::xml::ParseAttribute("id"), base::xml::ParseAttribute("name") }, 1, 1);
            while (nodeHelper.readNextRecognizedElement()) {
				if (xmlReader.name() == "auth")
					parseAuth(nodeHelper, node, xmlReader.attributes().value("id").toString(), xmlReader.attributes().value("name").toString());
            }
        }
    }
}

void Plugin::writeXML(QXmlStreamWriter & xmlWriter, base::ProjectNode & node) const
{
    Q_UNUSED(xmlWriter);
    Q_UNUSED(node);
    throw base::Exception("cutehmi::authssh::plugin::Plugin::writeXML() not implemented yet.");
}

void Plugin::parseAuth(const base::xml::ParseHelper & parentHelper, base::ProjectNode & node, const QString & id, const QString & name)
{
	std::unique_ptr<Auth> auth;
	QString host;
	int port;

	base::xml::ParseHelper helper(& parentHelper);
	helper << base::xml::ParseElement("server", 1, 1);

	QXmlStreamReader & xmlReader = *helper.xmlReader();
	while (helper.readNextRecognizedElement()) {
		if (xmlReader.name() == "server")
			parseServer(helper, node, host, port);
	}

	auth.reset(new Auth);
	auth->client()->setHost(host);
	auth->client()->setPort(port);
	base::ProjectNode * authNode = node.addChild(id, base::ProjectNodeData(name));
	authNode->addExtension(auth.get());

	authNode->data().append(std::unique_ptr<AuthSSHNodeData>(new AuthSSHNodeData(std::move(auth))));
}

void Plugin::parseServer(const base::xml::ParseHelper & parentHelper, base::ProjectNode & node, QString & host, int & port)
{
	Q_UNUSED(node);

    base::xml::ParseHelper helper(& parentHelper);
    helper << base::xml::ParseElement("host", 1, 1)
		   << base::xml::ParseElement("port", 1, 1);
//           << base::xml::ParseElement("guest_username", 0, 1)
//           << base::xml::ParseElement("guest_password", 0, 1);

    QXmlStreamReader & xmlReader = *helper.xmlReader();
    while (helper.readNextRecognizedElement()) {
        if (xmlReader.name() == "host")
			host = xmlReader.readElementText();
        else if (xmlReader.name() == "port") {
            bool ok;
			port = xmlReader.readElementText().toInt(& ok);
            if (!ok)
                xmlReader.raiseError(QObject::tr("Could not conver 'port' element contents to integer."));
		} /*else if (xmlReader.name() == "guest_username")
            guest_username = xmlReader.readElementText();
        else if (xmlReader.name() == "guest_password")
			guest_password = xmlReader.readElementText();*/
    }

//	node.addExtension(mainScreen.get());

//	screensNodeData.reset(new ScreensNodeData(std::move(mainScreen)));
//	node.data().append(std::move(screensNodeData));
}

}
}
}

//(c)MP: Copyright © 2017, Michal Policht. All rights reserved.
//(c)MP: This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
