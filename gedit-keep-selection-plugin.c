/* SPDX-License-Identifier: MIT */

#include "gedit-keep-selection-app-activatable.h"
#include "gedit-keep-selection-window-activatable.h"
#include "gedit-keep-selection-view-activatable.h"

#include <libpeas/peas.h>
G_MODULE_EXPORT void peas_register_types (PeasObjectModule *module);


G_MODULE_EXPORT void
peas_register_types (
	PeasObjectModule *module
)
{
	gedit_keep_selection_app_activatable_register (
		module
	);

	gedit_keep_selection_window_activatable_register (
		module
	);

	gedit_keep_selection_view_activatable_register (
		module
	);
}
