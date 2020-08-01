#!/usr/bin/env python3.8

# requires python3.8

import argparse
import json
import shutil
import os
import subprocess
from braceexpand import braceexpand as expand

# ***********************************************************************************
#
# Main function
#
# ***********************************************************************************
def main():

    # load configuration
    with open(args.config, 'r') as fin:
        config = json.loads(fin.read())

    # handle copy steps
    for step in config['copy']:
        src_path = os.path.join(args.sdk, step['from'])
        dest_path = os.path.join(args.mbed, step['to'])
        ignore_patterns = step['ignore']
        print("copy")
        print("from '" + src_path + "'")
        print("to '" + dest_path + "'")
        print("\texcluding:")
        print(ignore_patterns)
        print()
        shutil.rmtree(dest_path, ignore_errors=True)
        shutil.copytree(src_path, dest_path, ignore=shutil.ignore_patterns(*ignore_patterns), dirs_exist_ok=True)

    # handle pincfg generation
    print('generate pincfgs:')
    def absolutify(path):
        new_path = path
        new_path = new_path.replace('{_mbed}', args.mbed)
        new_path = new_path.replace('{_sdk}', args.sdk)        
        return new_path

    selectors = ['c', 'h']
    for name, job in config['generate']['pincfgs'].items():
        print('\t', name)
        for selector in selectors:
            results = subprocess.run(   [
                                            'python', 
                                            absolutify('{_sdk}/boards_sfe/common/bsp_pinconfig/pinconfig.py'), 
                                            absolutify(job['src']), selector,
                                            '-g', job['guard'],
                                            '-p', job['prefix'], 
                                            '-b', job['bgaguard']
                                        ],
                                        capture_output=True)
            
            with open(absolutify(job['dest']) + '.' + selector, 'wb') as fout:
                fout.write(results.stdout)
            errors = str(results.stderr)
            if errors != "b''":
                print('error: ', str(results.stderr))

    exit()



# ******************************************************************************
#
# Main program flow
#
# ******************************************************************************
if __name__ == '__main__':

    parser = argparse.ArgumentParser(
        description='Utility to generate GCC flag and symbol files from mbed build profiles')

    parser.add_argument('-m', '--mbed', dest='mbed', required=True, help='path to root of mbed')
    parser.add_argument('-c', '--config', dest='config', required=False, default='./boards_sfe/common/tools_sfe/scripts/configuration/mbed-config.json', help='path to the json configuration file')
    parser.add_argument('-s', '--sdk', dest='sdk', required=False, default='.', help='path to root of AmbiqSuiteSDK')
    parser.add_argument('-v', '--verbose', default=0, help='enable verbose output', action='store_true')

    args = parser.parse_args()

    # Create print function for verbose output if caller deems it: https://stackoverflow.com/questions/5980042/how-to-implement-the-verbose-or-v-option-into-a-script
    if args.verbose:
        def verboseprint(*args):
            # Print each argument separately so caller doesn't need to
            # stuff everything to be printed into a single string
            for arg in args:
                print(arg, end='', flush=True)
            print()
    else:
        verboseprint = lambda *a: None      # do-nothing function

    def twopartprint(verbosestr, printstr):
        if args.verbose:
            print(verbosestr, end = '')

        print(printstr)

    main()