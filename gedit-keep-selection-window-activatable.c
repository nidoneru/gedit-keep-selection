/* SPDX-License-Identifier: MIT */

#include "gedit-keep-selection-window-activatable.h"
#include "gedit-keep-selection-view-activatable.h"

#include <gio/gio.h>

#include <gedit/gedit-view.h>
#include <gedit/gedit-window.h>
#include <gedit/gedit-window-activatable.h>

struct _GeditKeepSelectionWindowActivatablePrivate
{
	GeditWindow *window;
};

enum
{
	PROP_0,
	PROP_WINDOW
};

static void
gedit_window_activatable_iface_init (
	GeditWindowActivatableInterface *iface
);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (
	GeditKeepSelectionWindowActivatable,
	gedit_keep_selection_window_activatable,
	G_TYPE_OBJECT,
	0,
	G_ADD_PRIVATE_DYNAMIC (
		GeditKeepSelectionWindowActivatable
	)
	G_IMPLEMENT_INTERFACE_DYNAMIC (
		GEDIT_TYPE_WINDOW_ACTIVATABLE,
		gedit_window_activatable_iface_init
	)
)

static void
keep_selection_activate_cb (
	GSimpleAction *action,
	GVariant      *parameter,
	gpointer       user_data
)
{
	GeditKeepSelectionWindowActivatable *self;
	GeditView *view;

	(void) action;
	(void) parameter;

	self = GEDIT_KEEP_SELECTION_WINDOW_ACTIVATABLE (
		user_data
	);

	view = gedit_window_get_active_view (
		self->priv->window
	);

	if (view == NULL)
	{
		return;
	}

	gedit_keep_selection_keep_only (view);
}

static const GActionEntry action_entries[] = {
	{
		"keep-selection-plugin-keep",
		keep_selection_activate_cb
	}
};

static void
add_actions (
	GeditKeepSelectionWindowActivatable *self
)
{
	g_action_map_add_action_entries (
		G_ACTION_MAP (self->priv->window),
		action_entries,
		G_N_ELEMENTS (action_entries),
		self
	);
}

static void
remove_actions (
	GeditKeepSelectionWindowActivatable *self
)
{
	g_action_map_remove_action_entries (
		G_ACTION_MAP (self->priv->window),
		action_entries,
		G_N_ELEMENTS (action_entries)
	);
}

static void
gedit_keep_selection_window_activatable_get_property (
	GObject    *object,
	guint       prop_id,
	GValue     *value,
	GParamSpec *pspec
)
{
	GeditKeepSelectionWindowActivatable *self;

	self = GEDIT_KEEP_SELECTION_WINDOW_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_WINDOW:
			g_value_set_object (
				value,
				self->priv->window
			);

			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (
				object,
				prop_id,
				pspec
			);

			break;
	}
}

static void
gedit_keep_selection_window_activatable_set_property (
	GObject      *object,
	guint         prop_id,
	const GValue *value,
	GParamSpec   *pspec
)
{
	GeditKeepSelectionWindowActivatable *self;

	self = GEDIT_KEEP_SELECTION_WINDOW_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_WINDOW:
			g_assert (
				self->priv->window == NULL
			);

			self->priv->window = GEDIT_WINDOW (
				g_value_dup_object (value)
			);

			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (
				object,
				prop_id,
				pspec
			);

			break;
	}
}

static void
gedit_keep_selection_window_activatable_dispose (
	GObject *object
)
{
	GeditKeepSelectionWindowActivatable *self;

	self = GEDIT_KEEP_SELECTION_WINDOW_ACTIVATABLE (object);

	g_clear_object (
		&self->priv->window
	);

	G_OBJECT_CLASS (
		gedit_keep_selection_window_activatable_parent_class
	)->dispose (object);
}

static void
gedit_keep_selection_window_activatable_class_init (
	GeditKeepSelectionWindowActivatableClass *klass
)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);

	object_class->get_property =
		gedit_keep_selection_window_activatable_get_property;

	object_class->set_property =
		gedit_keep_selection_window_activatable_set_property;

	object_class->dispose =
		gedit_keep_selection_window_activatable_dispose;

	g_object_class_override_property (
		object_class,
		PROP_WINDOW,
		"window"
	);
}

static void
gedit_keep_selection_window_activatable_class_finalize (
	GeditKeepSelectionWindowActivatableClass *klass
)
{
	(void) klass;
}

static void
gedit_keep_selection_window_activatable_init (
	GeditKeepSelectionWindowActivatable *self
)
{
	self->priv =
		gedit_keep_selection_window_activatable_get_instance_private (
			self
		);
}

static void
gedit_keep_selection_window_activatable_activate (
	GeditWindowActivatable *activatable
)
{
	GeditKeepSelectionWindowActivatable *self;

	self = GEDIT_KEEP_SELECTION_WINDOW_ACTIVATABLE (
		activatable
	);

	add_actions (self);
}

static void
gedit_keep_selection_window_activatable_deactivate (
	GeditWindowActivatable *activatable
)
{
	GeditKeepSelectionWindowActivatable *self;

	self = GEDIT_KEEP_SELECTION_WINDOW_ACTIVATABLE (
		activatable
	);

	remove_actions (self);
}

static void
gedit_window_activatable_iface_init (
	GeditWindowActivatableInterface *iface
)
{
	iface->activate =
		gedit_keep_selection_window_activatable_activate;

	iface->deactivate =
		gedit_keep_selection_window_activatable_deactivate;
}

void
gedit_keep_selection_window_activatable_register (
	PeasObjectModule *module
)
{
	gedit_keep_selection_window_activatable_register_type (
		G_TYPE_MODULE (module)
	);

	peas_object_module_register_extension_type (
		module,
		GEDIT_TYPE_WINDOW_ACTIVATABLE,
		GEDIT_TYPE_KEEP_SELECTION_WINDOW_ACTIVATABLE
	);
}
