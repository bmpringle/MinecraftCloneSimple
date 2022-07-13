#! /usr/bin/env python3
import os
import subprocess

def execCmd(cmd, cwdOverride = './'):
    process = subprocess.Popen(shell='true', cwd=cwdOverride, args=cmd, stdout=subprocess.PIPE)
    return process.communicate()

def execCmdProcessReturn(cmd, cwdOverride = './'):
    process = subprocess.Popen(shell='true', cwd=cwdOverride, args=cmd, stdout=subprocess.PIPE)
    return process
    
output, error = execCmd('git submodule update --init --recursive')