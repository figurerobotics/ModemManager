/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details:
 *
 * Copyright (C) 2024 Stig Magnus Baugstø <stig@peerconsulting.no>
 */

#define _LIBMM_INSIDE_MM
#include "mm-plugin-common.h"
#include "mm-log-object.h"

#if defined WITH_QMI
# include "mm-broadband-modem-qmi.h"
#else
# include "mm-broadband-modem.h"
#endif

#define MM_TYPE_PLUGIN_CELLIENT mm_plugin_cellient_get_type ()
MM_DEFINE_PLUGIN (CELLIENT, cellient, Cellient)

/*****************************************************************************/

static MMBaseModem *
create_modem (MMPlugin     *self,
              const gchar  *uid,
              const gchar  *physdev,
              const gchar **drivers,
              guint16       vendor,
              guint16       product,
              guint16       subsystem_vendor,
              guint16       subsystem_device,
              GList        *probes,
              GError      **error)
{
#if defined WITH_QMI
    if (mm_port_probe_list_has_qmi_port (probes)) {
        mm_obj_dbg (self, "QMI-powered Cellient modem found...");
        return MM_BASE_MODEM (mm_broadband_modem_qmi_new (uid,
                                                          physdev,
                                                          drivers,
                                                          mm_plugin_get_name (self),
                                                          vendor,
                                                          product));
    }
#endif

    mm_obj_dbg (self, "Generic Cellient modem found...");
    return MM_BASE_MODEM (mm_broadband_modem_new (uid,
                                                  physdev,
                                                  drivers,
                                                  mm_plugin_get_name (self),
                                                  vendor,
                                                  product));
}

/*****************************************************************************/

MM_PLUGIN_NAMED_CREATOR_SCOPE MMPlugin *
mm_plugin_create_cellient (void)
{
    static const gchar   *subsystems[] = { "tty", "net", "usbmisc", "wwan", NULL };
    static const guint16  vendor_ids[] = { 0x2692, 0 };

    return MM_PLUGIN (
        g_object_new (MM_TYPE_PLUGIN_CELLIENT,
                      MM_PLUGIN_NAME,               MM_MODULE_NAME,
                      MM_PLUGIN_ALLOWED_SUBSYSTEMS, subsystems,
		      MM_PLUGIN_ALLOWED_VENDOR_IDS, vendor_ids,
                      MM_PLUGIN_ALLOWED_AT,         TRUE,
                      MM_PLUGIN_ALLOWED_QMI,        TRUE,
                      NULL));
}

static void
mm_plugin_cellient_init (MMPluginCellient *self)
{
}

static void
mm_plugin_cellient_class_init (MMPluginCellientClass *klass)
{
    MMPluginClass *plugin_class = MM_PLUGIN_CLASS (klass);

    plugin_class->create_modem = create_modem;
}
