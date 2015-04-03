#ifndef CONSTANTS
#define CONSTANTS

#include "generic.h"
#include "util.h"

static const string PATH_SEPERATOR = get_path_seperator();
static const string PATH = "dash_files";
static const int NUM_THREADS = 4;

static const string DB_PATH = PATH + PATH_SEPERATOR + "db";
static const string DB_NAME = "dash_db";
static const string DB_LOG = PATH + PATH_SEPERATOR + "db_log";
static const string TABLE_PREFIX = "index_chromosome_";
static const string DB_CONFIG = PATH + PATH_SEPERATOR + "db_config";

static const string GENOME_PATH = PATH + PATH_SEPERATOR + "genome";

#endif