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
 * Copyright (C) 2014 Aleksander Morgado <aleksander@aleksander.es>
 */

#include <string.h>
#include <gmodule.h>

#define _LIBMM_INSIDE_MM
#include <libmm-glib.h>

#include "mm-plugin-common.h"
#include "mm-broadband-modem.h"

#define MM_TYPE_PLUGIN_HAIER mm_plugin_haier_get_type ()
MM_DEFINE_PLUGIN (HAIER, haier, Haier)

/*****************************************************************************/

static MMBaseModem *
create_modem (MMPlugin *self,
              const gchar *uid,
              const gchar *physdev,
              const gchar **drivers,
              guint16 vendor,
              guint16 product,
              guint16 subsystem_vendor,
              guint16 subsystem_device,
              GList *probes,
              GError **error)
{
    return MM_BASE_MODEM (mm_broadband_modem_new (uid,
                                                  physdev,
                                                  drivers,
                                                  mm_plugin_get_name (self),
                                                  vendor,
                                                  product));
}

/*****************************************************************************/

MM_PLUGIN_NAMED_CREATOR_SCOPE MMPlugin *
mm_plugin_create_haier (void)
{
    static const gchar *subsystems[] = { "tty", NULL };
    static const guint16 vendor_ids[] = { 0x201e, 0 };

    return MM_PLUGIN (g_object_new (MM_TYPE_PLUGIN_HAIER,
                                    MM_PLUGIN_NAME,               MM_MODULE_NAME,
                                    MM_PLUGIN_ALLOWED_SUBSYSTEMS, subsystems,
                                    MM_PLUGIN_ALLOWED_VENDOR_IDS, vendor_ids,
                                    MM_PLUGIN_ALLOWED_AT,         TRUE,
                                    NULL));
}

static void
mm_plugin_haier_init (MMPluginHaier *self)
{
}

static void
mm_plugin_haier_class_init (MMPluginHaierClass *klass)
{
    MMPluginClass *plugin_class = MM_PLUGIN_CLASS (klass);

    plugin_class->create_modem = create_modem;
}
