/* SPDX-License-Identifier: MIT */

#include "gedit-keep-selection-view-activatable.h"

#include <gtk/gtk.h>

#include <gedit/gedit-view.h>
#include <gedit/gedit-view-activatable.h>

struct _GeditKeepSelectionViewActivatablePrivate
{
	GeditView *view;
};

enum
{
	PROP_0,
	PROP_VIEW
};

static void
gedit_view_activatable_iface_init (
	GeditViewActivatableInterface *iface
);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (
	GeditKeepSelectionViewActivatable,
	gedit_keep_selection_view_activatable,
	G_TYPE_OBJECT,
	0,
	G_ADD_PRIVATE_DYNAMIC (
		GeditKeepSelectionViewActivatable
	)
	G_IMPLEMENT_INTERFACE_DYNAMIC (
		GEDIT_TYPE_VIEW_ACTIVATABLE,
		gedit_view_activatable_iface_init
	)
)

void
gedit_keep_selection_keep_only (
	GeditView *view
)
{
	GtkTextBuffer *buffer;
	GtkTextIter selection_start;
	GtkTextIter selection_end;
	GtkTextIter buffer_start;
	GtkTextIter buffer_end;
	GtkTextMark *start_mark;
	GtkTextMark *end_mark;

	g_return_if_fail (GEDIT_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (
		GTK_TEXT_VIEW (view)
	);

	if (!gtk_text_buffer_get_selection_bounds (
		    buffer,
		    &selection_start,
		    &selection_end
	    ))
	{
		return;
	}

	/*
	 * 選択範囲の位置を保持します。
	 *
	 * テキストを削除するとGtkTextIterは無効になるため、
	 * GtkTextMarkを使って選択範囲の境界を記録します。
	 */
	start_mark = gtk_text_buffer_create_mark (
		buffer,
		NULL,
		&selection_start,
		TRUE
	);

	end_mark = gtk_text_buffer_create_mark (
		buffer,
		NULL,
		&selection_end,
		FALSE
	);

	gtk_text_buffer_begin_user_action (buffer);

	/*
	 * 先に選択範囲より後ろを削除します。
	 */
	gtk_text_buffer_get_iter_at_mark (
		buffer,
		&selection_end,
		end_mark
	);

	gtk_text_buffer_get_end_iter (
		buffer,
		&buffer_end
	);

	gtk_text_buffer_delete (
		buffer,
		&selection_end,
		&buffer_end
	);

	/*
	 * 次に選択範囲より前を削除します。
	 */
	gtk_text_buffer_get_start_iter (
		buffer,
		&buffer_start
	);

	gtk_text_buffer_get_iter_at_mark (
		buffer,
		&selection_start,
		start_mark
	);

	gtk_text_buffer_delete (
		buffer,
		&buffer_start,
		&selection_start
	);

	/*
	 * 残ったテキスト全体を選択状態にします。
	 */
	gtk_text_buffer_get_start_iter (
		buffer,
		&selection_start
	);

	gtk_text_buffer_get_end_iter (
		buffer,
		&selection_end
	);

	gtk_text_buffer_select_range (
		buffer,
		&selection_start,
		&selection_end
	);

	gtk_text_buffer_end_user_action (buffer);

	gtk_text_buffer_delete_mark (
		buffer,
		start_mark
	);

	gtk_text_buffer_delete_mark (
		buffer,
		end_mark
	);
}

static void
keep_selection_only_cb (
	GtkMenuItem                       *menu_item,
	GeditKeepSelectionViewActivatable *self
)
{
	(void) menu_item;

	gedit_keep_selection_keep_only (
		self->priv->view
	);
}

static gboolean
is_japanese_locale (void)
{
	const gchar * const *languages;

	languages = g_get_language_names ();

	for (gint i = 0; languages[i] != NULL; i++)
	{
		if (g_str_has_prefix (languages[i], "ja"))
		{
			return TRUE;
		}
	}

	return FALSE;
}

static const gchar *
get_keep_selection_label (void)
{
	if (is_japanese_locale ())
	{
		return "選択範囲だけ残す";
	}

	return "Keep Selection";
}

static void
populate_popup_cb (
	GtkTextView                       *text_view,
	GtkWidget                         *popup,
	GeditKeepSelectionViewActivatable *self
)
{
	GtkTextBuffer *buffer;
	GtkTextIter selection_start;
	GtkTextIter selection_end;
	GtkWidget *separator;
	GtkWidget *item;
	gboolean has_selection;

	if (!GTK_IS_MENU_SHELL (popup))
	{
		return;
	}

	buffer = gtk_text_view_get_buffer (text_view);

	has_selection = gtk_text_buffer_get_selection_bounds (
		buffer,
		&selection_start,
		&selection_end
	);

	separator = gtk_separator_menu_item_new ();
	gtk_widget_show (separator);

	gtk_menu_shell_append (
		GTK_MENU_SHELL (popup),
		separator
	);

	item = gtk_menu_item_new_with_label (
		get_keep_selection_label ()
	);

	gtk_widget_set_sensitive (
		item,
		has_selection
	);

	g_signal_connect_object (
		item,
		"activate",
		G_CALLBACK (keep_selection_only_cb),
		self,
		G_CONNECT_DEFAULT
	);

	gtk_widget_show (item);

	gtk_menu_shell_append (
		GTK_MENU_SHELL (popup),
		item
	);
}

static void
gedit_keep_selection_view_activatable_get_property (
	GObject    *object,
	guint       prop_id,
	GValue     *value,
	GParamSpec *pspec
)
{
	GeditKeepSelectionViewActivatable *self;

	self = GEDIT_KEEP_SELECTION_VIEW_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_VIEW:
			g_value_set_object (
				value,
				self->priv->view
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
gedit_keep_selection_view_activatable_set_property (
	GObject      *object,
	guint         prop_id,
	const GValue *value,
	GParamSpec   *pspec
)
{
	GeditKeepSelectionViewActivatable *self;

	self = GEDIT_KEEP_SELECTION_VIEW_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_VIEW:
			g_assert (
				self->priv->view == NULL
			);

			self->priv->view = GEDIT_VIEW (
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
gedit_keep_selection_view_activatable_dispose (
	GObject *object
)
{
	GeditKeepSelectionViewActivatable *self;

	self = GEDIT_KEEP_SELECTION_VIEW_ACTIVATABLE (object);

	g_clear_object (
		&self->priv->view
	);

	G_OBJECT_CLASS (
		gedit_keep_selection_view_activatable_parent_class
	)->dispose (object);
}

static void
gedit_keep_selection_view_activatable_class_init (
	GeditKeepSelectionViewActivatableClass *klass
)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);

	object_class->get_property =
		gedit_keep_selection_view_activatable_get_property;

	object_class->set_property =
		gedit_keep_selection_view_activatable_set_property;

	object_class->dispose =
		gedit_keep_selection_view_activatable_dispose;

	g_object_class_override_property (
		object_class,
		PROP_VIEW,
		"view"
	);
}

static void
gedit_keep_selection_view_activatable_class_finalize (
	GeditKeepSelectionViewActivatableClass *klass
)
{
	(void) klass;
}

static void
gedit_keep_selection_view_activatable_init (
	GeditKeepSelectionViewActivatable *self
)
{
	self->priv =
		gedit_keep_selection_view_activatable_get_instance_private (
			self
		);
}

static void
gedit_keep_selection_view_activatable_activate (
	GeditViewActivatable *activatable
)
{
	GeditKeepSelectionViewActivatable *self;

	self = GEDIT_KEEP_SELECTION_VIEW_ACTIVATABLE (
		activatable
	);

	g_signal_connect_object (
		self->priv->view,
		"populate-popup",
		G_CALLBACK (populate_popup_cb),
		self,
		G_CONNECT_DEFAULT
	);
}

static void
gedit_keep_selection_view_activatable_deactivate (
	GeditViewActivatable *activatable
)
{
	GeditKeepSelectionViewActivatable *self;

	self = GEDIT_KEEP_SELECTION_VIEW_ACTIVATABLE (
		activatable
	);

	g_signal_handlers_disconnect_by_func (
		self->priv->view,
		populate_popup_cb,
		self
	);
}

static void
gedit_view_activatable_iface_init (
	GeditViewActivatableInterface *iface
)
{
	iface->activate =
		gedit_keep_selection_view_activatable_activate;

	iface->deactivate =
		gedit_keep_selection_view_activatable_deactivate;
}

void
gedit_keep_selection_view_activatable_register (
	PeasObjectModule *module
)
{
	gedit_keep_selection_view_activatable_register_type (
		G_TYPE_MODULE (module)
	);

	peas_object_module_register_extension_type (
		module,
		GEDIT_TYPE_VIEW_ACTIVATABLE,
		GEDIT_TYPE_KEEP_SELECTION_VIEW_ACTIVATABLE
	);
}
