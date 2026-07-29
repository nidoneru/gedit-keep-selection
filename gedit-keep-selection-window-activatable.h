/* SPDX-License-Identifier: MIT */

#pragma once

#include <libpeas/peas.h>

G_BEGIN_DECLS

#define GEDIT_TYPE_KEEP_SELECTION_WINDOW_ACTIVATABLE \
	(gedit_keep_selection_window_activatable_get_type ())

#define GEDIT_KEEP_SELECTION_WINDOW_ACTIVATABLE(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), \
	 GEDIT_TYPE_KEEP_SELECTION_WINDOW_ACTIVATABLE, \
	 GeditKeepSelectionWindowActivatable))

typedef struct _GeditKeepSelectionWindowActivatable
	GeditKeepSelectionWindowActivatable;

typedef struct _GeditKeepSelectionWindowActivatableClass
	GeditKeepSelectionWindowActivatableClass;

typedef struct _GeditKeepSelectionWindowActivatablePrivate
	GeditKeepSelectionWindowActivatablePrivate;

struct _GeditKeepSelectionWindowActivatable
{
	GObject parent;

	GeditKeepSelectionWindowActivatablePrivate *priv;
};

struct _GeditKeepSelectionWindowActivatableClass
{
	GObjectClass parent_class;
};

GType
gedit_keep_selection_window_activatable_get_type (void);

void
gedit_keep_selection_window_activatable_register (PeasObjectModule *module);

G_END_DECLS
