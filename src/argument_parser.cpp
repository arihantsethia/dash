#include "headers/argument_parser.h"

ArgumentParser::ArgumentParser() {
    index = align = distributed = false;
}

int ArgumentParser::parse_args(int argc, char* argv[]) {
    while (1) {
        struct option lopts [] = {
            {"index", 0, NULL, 0},
            {"align", 0, NULL, 0},
            {"g", 1, NULL, 0},
            {"l", 1, NULL, 0},
            {"t", 1, NULL, 0},
            {"d", 0, NULL, 0},
            {"i", 1, NULL, 0},
            {"o", 1, NULL, 0},
            {"u", 1, NULL, 0},
            {"e", 1, NULL, 0},
            {"c", 1, NULL, 0},
            {"help", 0, NULL, 0},
            NULL
        };

        int idx;
        int c = getopt_long (argc, argv, "", lopts, &idx);

        if (c == -1) break;

        if (c != 0) {
            print_usage ();
            exit (1); /* parsing failed? */
        }

        if (strcmp (lopts[idx].name, "index") == 0)
            index = true;
        else if (strcmp (lopts[idx].name, "align") == 0)
            align = true;
        else if (strcmp (lopts[idx].name, "d") == 0)
            distributed = false;
        else if (strcmp (lopts[idx].name, "g") == 0)
            genome = strdup (optarg);
        else if (strcmp (lopts[idx].name, "i") == 0)
            in = strdup (optarg);
        else if (strcmp (lopts[idx].name, "o") == 0)
            out = strdup (optarg);
        else if (strcmp (lopts[idx].name, "u") == 0)
            usage = strdup (optarg);
        else if (strcmp (lopts[idx].name, "l") == 0)
            seed_len = atoi (optarg);
        else if (strcmp (lopts[idx].name, "t") == 0)
            threads = atoi (optarg);
        else if (strcmp (lopts[idx].name, "e") == 0)
            edit_dist = atoi (optarg);
        else if (strcmp (lopts[idx].name, "c") == 0)
            conf = atoi (optarg);
        else if (strcmp (lopts[idx].name, "help") == 0) {
            print_usage ();
            exit (0);
        }
        else assert (false);
    }

    // /* return the execute command */
    // if (optind == argc) {
    //     //fprintf (stderr, "No program name given.\n");
    //     cout<<"DF"<<endl;
    //     print_usage ();
    //     exit (0);
    // }

    return optind;
}


void ArgumentParser::print_usage() {
    printf (
        "For running genome indexer: ./dash --index [options_index]\n"
        "\n"
        "options_index:\n"
        "--g=<file>        genome file path     \n"
        "--l=<integer>        seed length          \n"
        "--t=<integer>    number of threads    \n"
        "--help                 display this help page           \n"
        "\n"
        "*******                           \n"
        "For running reads aligner: ./dash --align [options_align] \n"
        "\n"
        "options_align:"
        "--i=<file>        reads file path \n"
        "--o=<file>       output file path \n"
        "--t=<integer>     number of threads  \n"
        "--d                  if this option is specified, MapReduce version will be used \n"
        "--u=<mode>          <mode> = master or <mode> = client. Valid only with --dist option \n"
        "--e=<integer>    max tolerable edit distance between read and reference sequence \n"
        "--c=<integer>        min. diff in edit distance scores between best and second best\n"
        "--help                display this help page           \n"
    );
}