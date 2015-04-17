#ifndef CONSTANTS
#define CONSTANTS

#include "generic.h"
#include "util.h"

static const string PATH_SEPERATOR = get_path_seperator();
static const string PATH = "dash_files";

static const int NUM_THREADS = 1;
static const int NUM_CHROMOSOMES = 2;
static const int INDEX_WIDTH = 3;   //chromosome_005, width of the index

static const string DB_PATH = PATH + PATH_SEPERATOR + "db";
static const string DB_NAME = "dash_db";
static const string DB_LOG = PATH + PATH_SEPERATOR + "db_log";
static const string TABLE_PREFIX = "index_chromosome_";
static const string TABLE_BLACKLIST_PREFIX = "index_chromosome_blacklist_";
static const string DB_CONFIG = PATH + PATH_SEPERATOR + "db_config";

static const string GENOME_PATH = PATH + PATH_SEPERATOR + "genome" + PATH_SEPERATOR;
static const string CHROMOSOME_PREFIX = "chromosome_";
static const string INDEX_PROP_FILE = PATH + PATH_SEPERATOR + "index.prop";
static const string SEED_LEN_PROP = "SEED_LEN";
static const int SEED_LEN = 20;

static const int JOB_LEN = 10000000;
static const int INDEX_THRESHOLD_LIMIT = 200;

static const int CONFIDENCE_THRESHOLD = 10;     // min. diff in edit distance scores between best and second best
static const int MAX_HITS = 10;     //max index locations in db to check for a seed
static const int MAX_EDIT_DIST = 20;    //max tolerable edit dist. between read and reference sequence

#endif