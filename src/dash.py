#!/usr/bin/env python
from lib import mincemeat
import argparse, subprocess, itertools
from operator import is_not
from functools import partial

class Dash(object):
    def __init__(self):
        self.lines = 100000

    def create_index(self, genome_file, seed_length, num_threads):
        args = ['./dashc++', '--index','--g='+genome_file, '--l='+str(seed_length),'--t='+str(num_threads)]
        subprocess.call(args)

    def run_master(self, input_file, output_file, edit_distance, confidence, port, num_threads):
        meta = ' '.join(map(str, [edit_distance, confidence, num_threads]))
        datasource = self._get_dict(input_file, meta)
        def mapfn(k, v):
            import subprocess
            edit_distance, confidence, n_threads = k.split()[1:]
            ifile_name = "dash_"+str(k)+".fq"
            ofile_name = "dash_"+str(k)+".sam"
            ifile = open(ifile_name, "w")
            ifile.write(str(v));
            args = ['./dashc++', '--align','--i='+ifile_name, '--o='+ofile_name,'--e='+edit_distance,'--c='+confidence, '--t='+n_threads ]
            subprocess.call(args)
            with open(ofile_name, 'r') as content_file:
                content = content_file.read()
            yield k,content

        def reducefn(k, vs):
            return '\n'.join(vs)

        s = mincemeat.Server()
        s.datasource = datasource
        s.mapfn = mapfn
        s.reducefn = reducefn

        results = s.run_server(port=port)
        o_file = open(output_file,"w")
        o_file.write('\n'.join(results.values())+'\n')

    def run_slave(self, ip, port, num_threads):
        client = mincemeat.Client()
        client.password = ""
        client.conn(ip, port)

    def _get_dict(self, input_file, meta):
        datasource = {}
        counter = 0
        with open(input_file) as f:
            for next_n_lines in itertools.izip_longest(*[f] * self.lines):
                datasource[str(counter)+" "+meta] = ''.join(filter(partial(is_not, None), next_n_lines))
                counter += 1
        return datasource

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='mode', help='command help')

    index_parser = subparsers.add_parser('index', help='index help')

    index_parser.add_argument("-g", "--genome_file", help="genome file for creating index", required=True)
    index_parser.add_argument("-l", "--seed_length", help="seed length", type=int, default=20)
    index_parser.add_argument("-t", "--threads", help="number of threads", type=int, default=4)

    align_parser = subparsers.add_parser('align', help='align help')

    align_subparsers = align_parser.add_subparsers(dest='map_reduce_mode', help='command help')

    map_parser = align_subparsers.add_parser('master', help='master help')

    map_parser.add_argument("-p", "--port", help="port", type=int, default=5128)
    map_parser.add_argument("-i", "--input_file", help="input file containing reads", required=True)
    map_parser.add_argument("-o", "--output_file", help="output file from the aligner", default="dash_align.sam")
    map_parser.add_argument("-e", "--edit_distance", help="number of threads", type=int, default=8)
    map_parser.add_argument("-c", "--confidence", help="number of threads", type=int, default=10)
    map_parser.add_argument("-t", "--threads", help="number of threads", type=int, default=4)

    reduce_parser = align_subparsers.add_parser('slave', help='slave help')

    reduce_parser.add_argument("-ip", "--ip", help="ip of master", required=True)
    reduce_parser.add_argument("-p", "--port", help="port", type=int, default=5128)
    reduce_parser.add_argument("-t", "--threads", help="number of threads", type=int, default=4)
    
    args = parser.parse_args()

    dash = Dash()
    if(args.mode=='index'):
        dash.create_index(args.genome_file, args.seed_length, args.threads)
        print "Genome Index Created"
    elif(args.mode=='align'):
        if(args.map_reduce_mode=='master'):
            print "Map-reduce master created"
            dash.run_master(args.input_file, args.output_file, args.edit_distance, args.confidence, args.port, args.threads)
            print "Output Generated"
        elif(args.map_reduce_mode=='slave'):
            print "Map-reduce slave created"
            dash.run_slave(args.ip, args.port, args.threads)
