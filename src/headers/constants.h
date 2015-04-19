#ifndef CONSTANTS
#define CONSTANTS

#include "generic.h"
#include "util.h"

static const string PATH_SEPERATOR = get_path_seperator();
static const string PATH = "dash_files";

static const int NUM_THREADS = 4;
static const int PAD_WIDTH = 3;

static const string DB_PATH = PATH + PATH_SEPERATOR + "db";
static const string DB_NAME = "dash_db";
static const string DB_LOG = PATH + PATH_SEPERATOR + "db_log";
static const string TABLE_PREFIX = "index_chromosome_";
static const string TABLE_BLACKLIST = "index_chromosome_blacklist";
static const string DB_CONFIG = PATH + PATH_SEPERATOR + "db_config";

static const string GENOME_PATH = PATH + PATH_SEPERATOR + "genome" + PATH_SEPERATOR;
static const string CHROMOSOME_PREFIX = "chromosome_";
static const string INDEX_PROP_FILE = PATH + PATH_SEPERATOR + "index.prop";
static const string SEED_LEN_PROP = "SEED_LEN";
static const string NUM_CHROMO_PROP = "NUM_CHROMOSOMES";
static const int SEED_LEN = 20;

static const int JOB_LEN = 10000000;
static const int INDEX_THRESHOLD_LIMIT = 200;   //TODO: include in options?

//TODO : think about reasonable value for conf threshold
static const int CONFIDENCE_THRESHOLD = 10;     // min. diff in edit distance scores between best and second best
static const int MAX_EDIT_DIST = 8;            //max tolerable edit dist. between read and reference sequence

static const string CHROMOSOME_OFFSET = "chromosome_offset_";
static const string PROP_SEPARATOR = ":";

#endif