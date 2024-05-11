#include <stdlib.h>
#include <string.h>
#include "sway/config.h"
#include "sway/commands.h"
#include "sway/input/input-manager.h"

struct cmd_results *input_cmd_faulty_mouse_workaround(int argc, char **argv) {
	struct cmd_results *error = NULL;
	if ((error = checkarg(argc, "faulty_mouse_workaround", EXPECTED_EQUAL_TO, 2))) {
		return error;
	}
	struct input_config *ic = config->handler_context.input_config;
	if (!ic) {
		return cmd_results_new(CMD_FAILURE, "No input device defined.");
	}

	int delay = atoi(argv[1]);
	if (delay < 0) {
		return cmd_results_new(CMD_INVALID, "Faulty mouse workaround release delay cannot be negative");
	}
	if (strcmp(argv[0], "left")) {
		ic->faulty_mouse_workaround_delay_left = delay;
	} else if (strcmp(argv[0], "right")) {
		ic->faulty_mouse_workaround_delay_right = delay;
	} else {
		return cmd_results_new(CMD_INVALID, "Faulty mouse workaround button must be either left or right.");
	}

	return cmd_results_new(CMD_SUCCESS, NULL);
}
