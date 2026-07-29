/* SPDX-License-Identifier: MIT */

#pragma once

#include <gedit/gedit-view.h>
#include <libpeas/peas.h>

G_BEGIN_DECLS

#define GEDIT_TYPE_KEEP_SELECTION_VIEW_ACTIVATABLE \
	(gedit_keep_selection_view_activatable_get_type ())

#define GEDIT_KEEP_SELECTION_VIEW_ACTIVATABLE(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), \
	 GEDIT_TYPE_KEEP_SELECTION_VIEW_ACTIVATABLE, \
	 GeditKeepSelectionViewActivatable))

typedef struct _GeditKeepSelectionViewActivatable
	GeditKeepSelectionViewActivatable;

typedef struct _GeditKeepSelectionViewActivatableClass
	GeditKeepSelectionViewActivatableClass;

typedef struct _GeditKeepSelectionViewActivatablePrivate
	GeditKeepSelectionViewActivatablePrivate;

struct _GeditKeepSelectionViewActivatable
{
	GObject parent;

	GeditKeepSelectionViewActivatablePrivate *priv;
};

struct _GeditKeepSelectionViewActivatableClass
{
	GObjectClass parent_class;
};

GType
gedit_keep_selection_view_activatable_get_type (void);

void
gedit_keep_selection_view_activatable_register (
	PeasObjectModule *module
);

void
gedit_keep_selection_keep_only (
	GeditView *view
);

G_END_DECLS
