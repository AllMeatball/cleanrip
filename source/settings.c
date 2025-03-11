/**
 * CleanRip - settings.c
 * Copyright (C) 2025 AllMeatball
 *
 * Persistent settings reading/writing.
 *
 * CleanRip homepage: http://code.google.com/p/cleanrip/
 * email address: emukidid@gmail.com
 *
 *
 * This program is free software; you can redistribute it and/
 * or modify it under the terms of the GNU General Public Li-
 * cence as published by the Free Software Foundation; either
 * version 2 of the Licence, or any later version.
 *
 * This program is distributed in the hope that it will be use-
 * ful, but WITHOUT ANY WARRANTY; without even the implied war-
 * ranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public Licence for more details.
 *
 **/

#include "main.h"
#include "settings.h"

#include "IPLFontWrite.h"
#include "FrameBufferMagic.h"

#include <mxml.h>
#include <stdlib.h>

int getMaxPos(int option_pos) {
	switch (option_pos) {
		case WII_DUAL_LAYER:
			return DUAL_DELIM;
		case WII_CHUNK_SIZE:
			return CHUNK_DELIM;
		case NGC_ALIGN_BOUNDARY:
			return ALIGNB_DELIM;
		case NGC_ALIGN_FILES:
			return ALIGN_DELIM;
		case NGC_SHRINK_ISO:
			return SHRINK_DELIM;
		case WII_NEWFILE:
			return NEWFILE_DELIM;
	}
	return 0;
}

void toggleOption(int option_pos, int dir) {
	int max = getMaxPos(option_pos);
	if (options_map[option_pos] + dir >= max) {
		options_map[option_pos] = 0;
	} else if (options_map[option_pos] + dir < 0) {
		options_map[option_pos] = max - 1;
	} else {
		options_map[option_pos] += dir;
	}
}

char *getShrinkOption() {
	int opt = options_map[NGC_SHRINK_ISO];
	if (opt == SHRINK_ALL)
		return "Shrink All";
	else if (opt == SHRINK_PAD_GARBAGE)
		return "Wipe Garbage";
	else if (opt == SHRINK_NONE)
		return "No";
	return 0;
}

char *getAlignOption() {
	int opt = options_map[NGC_ALIGN_FILES];
	if (opt == ALIGN_ALL)
		return "Align All";
	else if (opt == ALIGN_AUDIO)
		return "Audio Only";
	return 0;
}

char *getAlignmentBoundaryOption() {
	int opt = options_map[NGC_ALIGN_BOUNDARY];
	if (opt == ALIGN_32)
		return "32Kb";
	else if (opt == ALIGN_2)
		return "2KB";
	else if (opt == ALIGN_512)
		return "512B";
	return 0;
}

char *getDualLayerOption() {
	int opt = options_map[WII_DUAL_LAYER];
	if (opt == AUTO_DETECT)
		return "Auto";
	else if (opt == SINGLE_MINI)
		return "1.4GB";
	else if (opt == SINGLE_LAYER)
		return "4.4GB";
	else if (opt == DUAL_LAYER)
		return "8GB";
	return 0;
}

char *getNewFileOption() {
	int opt = options_map[WII_NEWFILE];
	if (opt == ASK_USER)
		return "Yes";
	else if (opt == AUTO_CHUNK)
		return "No";
	return 0;
}

char *getChunkSizeOption() {
	int opt = options_map[WII_CHUNK_SIZE];
	if (opt == CHUNK_1GB)
		return "1GB";
	else if (opt == CHUNK_2GB)
		return "2GB";
	else if (opt == CHUNK_3GB)
		return "3GB";
	else if (opt == CHUNK_MAX)
		return "Max";
	return 0;
}

int Settings_setInteger(mxml_node_t *xml, mxml_node_t *element, const char *key, int *value) {
	mxml_node_t *node = mxmlFindElement(element, xml, key, NULL, NULL, MXML_DESCEND);

	print_gecko("Getting integer value from [%s]\r\n", key);
	if (!node) {
		print_gecko("Not Found\r\n");
		return 1;
	}

	*value = mxmlGetInteger(node);

	return 0;
}

int Settings_apply(mxml_node_t *pointer, char **error_str) {
	char *dummy_buffer = NULL;

	if (!error_str) {
		error_str = &dummy_buffer;
	}

	print_gecko("Applying settings\r\n");

#ifdef HW_RVL
	print_gecko("Wii settings:\r\n");
	mxml_node_t *wii = mxmlFindElement(pointer, pointer, "wii", NULL, NULL, MXML_DESCEND);
	if (!wii) {
		print_gecko("Not Found\r\n");
		*error_str = "Failed to find Wii settings";
		return 1;
	}

	Settings_setInteger(pointer, wii, "dual_layer", &options_map[DUAL_LAYER]);
	Settings_setInteger(pointer, wii, "chunk_size", &options_map[WII_CHUNK_SIZE]);
	Settings_setInteger(pointer, wii, "new_file",   &options_map[WII_NEWFILE]);
#endif

	// TODO: error message string
	return 0;
}

int Settings_read(char *mount_path, char **error_str) {
	int ret = 0;
	char *dummy_buffer = "";
	FILE *fp = NULL;
	mxml_node_t *xml = NULL;

	mxmlSetErrorCallback((mxml_error_cb_t)print_gecko);

	if (!error_str) {
		error_str = &dummy_buffer;
	}

	sprintf(txtbuffer, "%scleanrip-settings.xml", mount_path);
	fp = fopen(txtbuffer, "rb");
	if (fp) {
		xml = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
		fclose(fp);
	} else {
		*error_str = strerror(errno);
		mxmlDelete(xml);
		return 1;
	}

	if (xml) {
		if (DrawYesNoDialog(
			"Saved settings found",
			"Apply them right now?"
		)) {
			ret = Settings_apply(xml, error_str);
		}
	} else {
		*error_str = "Settings XML not found.";
		mxmlDelete(xml);
		return 1;
	}

	mxmlDelete(xml);

	return ret;
}

int Settings_write(char *mount_path, char **error_str) {
	char *dummy_buffer = "";

	if (!error_str) {
		error_str = &dummy_buffer;
	}

	mxml_node_t *xml = NULL;
	mxml_node_t *settings;
	mxml_node_t *wii;
	mxml_node_t *node;

	mxmlSetErrorCallback((mxml_error_cb_t)print_gecko);

	sprintf(txtbuffer, "%scleanrip-settings.xml", mount_path);
	FILE *fp = fopen(txtbuffer, "wb");
	if (!fp) {
		print_gecko("Failed to create settings file!\r\n");
		return 0;
	}

	xml = mxmlNewXML("1.0");
	settings = mxmlNewElement(xml, "settings");
	wii = mxmlNewElement(settings, "wii");

	node = mxmlNewElement(wii, "dual_layer");
	mxmlNewInteger(node, options_map[WII_DUAL_LAYER]);

	node = mxmlNewElement(wii, "chunk_size");
	mxmlNewInteger(node, options_map[WII_CHUNK_SIZE]);

	node = mxmlNewElement(wii, "new_file");
	mxmlNewInteger(node, options_map[WII_NEWFILE]);

	mxmlSaveFile(xml, fp, NULL);
	fclose(fp);

	mxmlDelete(xml);

	return 1;
}
