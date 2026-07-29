/* SPDX-License-Identifier: MIT */

#include "gedit-keep-selection-app-activatable.h"

#include <gtk/gtk.h>

#include <gedit/gedit-app.h>
#include <gedit/gedit-app-activatable.h>

struct _GeditKeepSelectionAppActivatablePrivate
{
	GeditApp *app;
};

enum
{
	PROP_0,
	PROP_APP
};

static void
gedit_app_activatable_iface_init (
	GeditAppActivatableInterface *iface
);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (
	GeditKeepSelectionAppActivatable,
	gedit_keep_selection_app_activatable,
	G_TYPE_OBJECT,
	0,
	G_ADD_PRIVATE_DYNAMIC (
		GeditKeepSelectionAppActivatable
	)
	G_IMPLEMENT_INTERFACE_DYNAMIC (
		GEDIT_TYPE_APP_ACTIVATABLE,
		gedit_app_activatable_iface_init
	)
)

static void
gedit_keep_selection_app_activatable_get_property (
	GObject    *object,
	guint       prop_id,
	GValue     *value,
	GParamSpec *pspec
)
{
	GeditKeepSelectionAppActivatable *self;

	self = GEDIT_KEEP_SELECTION_APP_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_APP:
			g_value_set_object (
				value,
				self->priv->app
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
gedit_keep_selection_app_activatable_set_property (
	GObject      *object,
	guint         prop_id,
	const GValue *value,
	GParamSpec   *pspec
)
{
	GeditKeepSelectionAppActivatable *self;

	self = GEDIT_KEEP_SELECTION_APP_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_APP:
			g_assert (self->priv->app == NULL);

			self->priv->app = GEDIT_APP (
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
gedit_keep_selection_app_activatable_dispose (
	GObject *object
)
{
	GeditKeepSelectionAppActivatable *self;

	self = GEDIT_KEEP_SELECTION_APP_ACTIVATABLE (object);

	g_clear_object (&self->priv->app);

	G_OBJECT_CLASS (
		gedit_keep_selection_app_activatable_parent_class
	)->dispose (object);
}

static void
gedit_keep_selection_app_activatable_class_init (
	GeditKeepSelectionAppActivatableClass *klass
)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);

	object_class->get_property =
		gedit_keep_selection_app_activatable_get_property;

	object_class->set_property =
		gedit_keep_selection_app_activatable_set_property;

	object_class->dispose =
		gedit_keep_selection_app_activatable_dispose;

	g_object_class_override_property (
		object_class,
		PROP_APP,
		"app"
	);
}

static void
gedit_keep_selection_app_activatable_class_finalize (
	GeditKeepSelectionAppActivatableClass *klass
)
{
}

static void
gedit_keep_selection_app_activatable_init (
	GeditKeepSelectionAppActivatable *self
)
{
	self->priv =
		gedit_keep_selection_app_activatable_get_instance_private (
			self
		);
}

static void
add_accels (
	GtkApplication *app
)
{
	const gchar *accels[] = {
		"<Control>K",
		NULL
	};

	gtk_application_set_accels_for_action (
		app,
		"win.keep-selection-plugin-keep",
		accels
	);
}

static void
remove_accels (
	GtkApplication *app
)
{
	const gchar *null_accels[] = {
		NULL
	};

	gtk_application_set_accels_for_action (
		app,
		"win.keep-selection-plugin-keep",
		null_accels
	);
}

static void
gedit_keep_selection_app_activatable_activate (
	GeditAppActivatable *activatable
)
{
	GeditKeepSelectionAppActivatable *self;

	self = GEDIT_KEEP_SELECTION_APP_ACTIVATABLE (
		activatable
	);

	add_accels (
		GTK_APPLICATION (self->priv->app)
	);
}

static void
gedit_keep_selection_app_activatable_deactivate (
	GeditAppActivatable *activatable
)
{
	GeditKeepSelectionAppActivatable *self;

	self = GEDIT_KEEP_SELECTION_APP_ACTIVATABLE (
		activatable
	);

	remove_accels (
		GTK_APPLICATION (self->priv->app)
	);
}

static void
gedit_app_activatable_iface_init (
	GeditAppActivatableInterface *iface
)
{
	iface->activate =
		gedit_keep_selection_app_activatable_activate;

	iface->deactivate =
		gedit_keep_selection_app_activatable_deactivate;
}

void
gedit_keep_selection_app_activatable_register (
	PeasObjectModule *module
)
{
	gedit_keep_selection_app_activatable_register_type (
		G_TYPE_MODULE (module)
	);

	peas_object_module_register_extension_type (
		module,
		GEDIT_TYPE_APP_ACTIVATABLE,
		GEDIT_TYPE_KEEP_SELECTION_APP_ACTIVATABLE
	);
}
