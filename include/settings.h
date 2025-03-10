/**
 * CleanRip - settings.h
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

enum options
{
    NGC_SHRINK_ISO=0,
    NGC_ALIGN_FILES,
    NGC_ALIGN_BOUNDARY,
    WII_DUAL_LAYER,
    WII_CHUNK_SIZE,
    WII_NEWFILE
};

enum shrinkOptions
{
    SHRINK_NONE=0,
    SHRINK_PAD_GARBAGE,
    SHRINK_ALL,
    SHRINK_DELIM
};

enum alignOptions
{
    ALIGN_ALL=0,
    ALIGN_AUDIO,
    ALIGN_DELIM
};

enum alignBoundaryOptions
{
    ALIGN_32=0,
    ALIGN_2,
    ALIGN_512,
    ALIGNB_DELIM
};

enum dualOptions
{
    AUTO_DETECT=0,
    SINGLE_MINI,
    SINGLE_LAYER,
    DUAL_LAYER,
    DUAL_DELIM
};

enum chunkOptions
{
    CHUNK_1GB=0,
    CHUNK_2GB,
    CHUNK_3GB,
    CHUNK_MAX,
    CHUNK_DELIM
};

enum newFileOptions
{
    ASK_USER=0,
    AUTO_CHUNK,
    NEWFILE_DELIM
};

static int options_map[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int getMaxPos(int option_pos);
void toggleOption(int option_pos, int dir);
char *getShrinkOption();
char *getAlignOption();
char *getAlignmentBoundaryOption();
char *getDualLayerOption();
char *getNewFileOption();
char *getChunkSizeOption();
