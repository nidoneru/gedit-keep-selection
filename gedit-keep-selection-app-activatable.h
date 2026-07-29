/* SPDX-License-Identifier: MIT */

#pragma once

#include <libpeas/peas.h>

G_BEGIN_DECLS

#define GEDIT_TYPE_KEEP_SELECTION_APP_ACTIVATABLE \
	(gedit_keep_selection_app_activatable_get_type ())

#define GEDIT_KEEP_SELECTION_APP_ACTIVATABLE(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), \
	 GEDIT_TYPE_KEEP_SELECTION_APP_ACTIVATABLE, \
	 GeditKeepSelectionAppActivatable))

typedef struct _GeditKeepSelectionAppActivatable
	GeditKeepSelectionAppActivatable;

typedef struct _GeditKeepSelectionAppActivatableClass
	GeditKeepSelectionAppActivatableClass;

typedef struct _GeditKeepSelectionAppActivatablePrivate
	GeditKeepSelectionAppActivatablePrivate;

struct _GeditKeepSelectionAppActivatable
{
	GObject parent;

	GeditKeepSelectionAppActivatablePrivate *priv;
};

struct _GeditKeepSelectionAppActivatableClass
{
	GObjectClass parent_class;
};

GType
gedit_keep_selection_app_activatable_get_type (void);

void
gedit_keep_selection_app_activatable_register (
	PeasObjectModule *module
);

G_END_DECLS
