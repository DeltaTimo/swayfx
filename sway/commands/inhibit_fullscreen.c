#include <string.h>
#include <strings.h>
#include "sway/commands.h"
#include "sway/input/seat.h"
#include "sway/ipc-server.h"
#include "sway/output.h"
#include "sway/tree/arrange.h"
#include "sway/tree/container.h"
#include "sway/tree/view.h"
#include "sway/tree/workspace.h"
#include "list.h"
#include "util.h"

struct cmd_results *cmd_inhibit_fullscreen(int argc, char **argv) {
	struct cmd_results *error = NULL;
	if ((error = checkarg(argc, "inhibit_fullscreen", EXPECTED_EQUAL_TO, 1))) {
		return error;
	}
	if (!root->outputs->length) {
		return cmd_results_new(CMD_INVALID,
				"Can't run this command while there's no outputs connected.");
	}
	struct sway_container *container = config->handler_context.container;
	if (!container) {
		return cmd_results_new(CMD_INVALID, "Missing container for inhibit_fullscreen.");
	}

	if (container_is_scratchpad_hidden(container)) {
		return cmd_results_new(CMD_INVALID,
				"Can't change inhibit_fullscreen on hidden scratchpad container");
	}

	bool wants_inhibit_fullscreen =
		parse_boolean(argv[0], container->inhibited_fullscreen);

	container->inhibited_fullscreen = wants_inhibit_fullscreen;

	return cmd_results_new(CMD_SUCCESS, NULL);
}
