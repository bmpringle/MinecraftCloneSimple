#! /usr/bin/env python3
import os
import subprocess

def execCmd(cmd, cwdOverride = './'):
    process = subprocess.Popen(shell='true', cwd=cwdOverride, args=cmd, stdout=subprocess.PIPE)
    return process.communicate()

def execCmdProcessReturn(cmd, cwdOverride = './'):
    process = subprocess.Popen(shell='true', cwd=cwdOverride, args=cmd, stdout=subprocess.PIPE)
    return process

output, error = execCmd('mkdir -p lib')
output, error = execCmd('make library', cwdOverride = './EventQueue/')
output, error = execCmd('mv ./eventLib.a ../../lib/eventLib.a', cwdOverride = './EventQueue/lib/')
output, error = execCmd('make library', cwdOverride = './TimerMap/')
output, error = execCmd('mv ./timerMapLib.a ../../lib/timerMapLib.a', cwdOverride = './TimerMap/lib/')
output, error = execCmd('make library', cwdOverride = './ObjLoader/')
output, error = execCmd('mv ./objLoaderLib.a ../../lib/objLoaderLib.a', cwdOverride = './ObjLoader/lib/')

output, error = execCmd('mkdir -p build', cwdOverride = './libnoise/')
output, error = execCmd('g++ -c ../src/*.cpp ../src/model/* ../src/module/* -I../src/noise/ -DNOISE_STATIC', cwdOverride = './libnoise/build/')
output, error = execCmd('ar rvs libnoise.a *.o', cwdOverride = './libnoise/build/')
output, error = execCmd('mv libnoise.a ../../lib/', cwdOverride = './libnoise/build/')

process = execCmdProcessReturn('python compileDeps.py', cwdOverride = './VulkanEngine')

process.wait()

output, error = execCmd('scons -j8', cwdOverride = './VulkanEngine')

output, error = execCmd('scons -j8 DBG=1', cwdOverride = './VulkanEngine')

output, error = execCmd('mv ./VulkanEngine/lib/rel/libVulkanEngineLib.dylib ./lib/rel/')

output, error = execCmd('mv ./VulkanEngine/lib/dbg/libVulkanEngineLib.dylib ./lib/dbg/')

output, error = execCmd('cp ./VulkanEngine/assets/unifont-13.0.06.ttf ./assets/')

output, error = execCmd('mkdir -p shaders/output', cwdOverride = './VulkanEngine')

output, error = execCmd('sh compileShaders.sh', cwdOverride = './VulkanEngine')

output, error = execCmd('mkdir -p shaders/output')

output, error = execCmd('cp -r VulkanEngine/shaders/output/ shaders/output/')

output, error = execCmd('cp VulkanEngine/assets/missing_texture.png assets/')

output, error = execCmd('cp VulkanEngine/assets/blank_texture.png assets/')