#include <strings.h>
#include "log.h"
#include "sway/commands.h"
#include "sway/config.h"
#include "sway/tree/arrange.h"
#include "sway/tree/container.h"
#include "sway/tree/view.h"
#include "sway/tree/workspace.h"
#include "util.h"

// fullscreen [enable|disable|toggle] [global|maximize] ...
struct cmd_results *cmd_fullscreen(int argc, char **argv) {
	struct cmd_results *error = NULL;
	if ((error = checkarg(argc, "fullscreen", EXPECTED_AT_MOST, 3))) {
		return error;
	}
	if (!root->outputs->length) {
		return cmd_results_new(CMD_FAILURE,
				"Can't run this command while there's no outputs connected.");
	}
	struct sway_container *container = config->handler_context.container;

	if (!container) {
		// If the focus is not a container, do nothing successfully
		return cmd_results_new(CMD_SUCCESS, NULL);
	} else if (!container->pending.workspace) {
		// If in the scratchpad, operate on the highest container
		while (container->pending.parent) {
			container = container->pending.parent;
		}
	}

	bool is_fullscreen = container->pending.fullscreen_mode != FULLSCREEN_NONE;
	bool global = false;
	bool maximize = false;
	bool enable = !is_fullscreen;

	if (argc >= 1) {
		if (strcasecmp(argv[0], "global") == 0) {
			global = true;
		} else if (strcasecmp(argv[0], "maximize") == 0) {
			maximize = true;
		} else {
			enable = parse_boolean(argv[0], is_fullscreen);
		}
	}

	for (int i = 1; i < argc; ++i) {
		if (strcasecmp(argv[i], "global") == 0) {
			global = true;
		} else if (strcasecmp(argv[i], "maximize") == 0) {
			maximize = true;
		}
	}

	enum sway_fullscreen_mode mode = FULLSCREEN_NONE;
	if (enable) {
		mode = global ? FULLSCREEN_GLOBAL : FULLSCREEN_WORKSPACE;
		mode |= maximize ? FULLSCREEN_MAXIMIZED : FULLSCREEN_NONE;
	}

	container_set_fullscreen(container, mode);
	arrange_root();

	return cmd_results_new(CMD_SUCCESS, NULL);
}
