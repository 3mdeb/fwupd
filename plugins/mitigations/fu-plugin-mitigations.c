/*
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include "config.h"

#include <fwupdplugin.h>

void
fu_plugin_init(FuPlugin *plugin)
{
	fu_plugin_set_build_hash(plugin, FU_BUILD_HASH);
}

static void
fu_plugin_add_security_attr_meltdown(FuPlugin *plugin, FuSecurityAttrs *attrs)
{
	g_autoptr(FwupdSecurityAttr) attr = NULL;
	g_autoptr(GError) error_local = NULL;
	gsize bufsz = 0;
	g_autofree gchar *buf = NULL;
	g_autofree gchar *fn = NULL;

	fn = g_build_filename("/sys",
			      "devices",
			      "system",
			      "cpu",
			      "vulnerabilities",
			      "meltdown",
			      NULL);

	/* create attr*/
	attr = fwupd_security_attr_new(FWUPD_SECURITY_ATTR_ID_MELTDOWN);
	fwupd_security_attr_set_plugin(attr, fu_plugin_get_name(plugin));
	fwupd_security_attr_set_level(attr, FWUPD_SECURITY_ATTR_LEVEL_CRITICAL);
	fu_security_attrs_append(attrs, attr);

	/* load file */
	if (!g_file_get_contents(fn, &buf, &bufsz, &error_local)) {
		g_warning("could not open %s: %s", fn, error_local->message);
		fwupd_security_attr_set_result(attr, FWUPD_SECURITY_ATTR_RESULT_NOT_VALID);
		return;
	}

	if (g_strstr_len(buf, bufsz, "Vulnerable") != NULL) {
		fwupd_security_attr_set_result(attr, FWUPD_SECURITY_ATTR_RESULT_NOT_ENABLED);
		return;
	}

	/* success */
	fwupd_security_attr_add_flag(attr, FWUPD_SECURITY_ATTR_FLAG_SUCCESS);
	fwupd_security_attr_set_result(attr, FWUPD_SECURITY_ATTR_RESULT_ENABLED);
}

void
fu_plugin_add_security_attrs(FuPlugin *plugin, FuSecurityAttrs *attrs)
{
	/* look for the the file in sysfs */
	fu_plugin_add_security_attr_meltdown(plugin, attrs);
}
