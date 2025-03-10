/**
 * CleanRip - settings.c
 * Copyright (C) 2025 AllMeatball
 *
 * Persisent settings reading/writing.
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

int Settings_write(const char *path) {
    mxml_node_t *xml;
    mxml_node_t *settings;
    mxml_node_t *wii;
    mxml_node_t *node;

    mxmlSetErrorCallback((mxml_error_cb_t)print_gecko);

    FILE *fp = fopen("GAMEID-state.xml", "w");
    if (!fp) {
        perror("Failed to open disc state for writing");
        return 1;
    }

    char str_buffer[128];
    xml = mxmlNewXML("1.0");
    settings = mxmlNewElement(xml, "settings");
        wii = mxmlNewElement(settings, "wii");

        snprintf(str_buffer, 128, "%d", options_map[WII_DUAL_LAYER]);
        node = mxmlNewElement(wii, "dual_layer");

        snprintf(str_buffer, 128, "%d", options_map[WII_CHUNK_SIZE]);
        node = mxmlNewElement(wii, "chunk_size");

        snprintf(str_buffer, 128, "%d", options_map[WII_NEWFILE]);
        node = mxmlNewElement(wii, "new_file");

    mxmlSaveFile(xml, fp, NULL);
    fclose(fp);
    mxmlDelete(xml);

    return 0;
}
