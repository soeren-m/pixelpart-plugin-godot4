#!/usr/bin/env python
import os
import sys

def add_sources(sources, dir, extension):
    for f in os.listdir(dir):
        if f.endswith("." + extension):
            sources.append(dir + "/" + f)

# Scons environment
env = SConscript("godot-cpp/SConstruct")

# Includes
env.Append(CPPPATH=["src/", "pixelpart-runtime/"])

# Sources to compile
sources = []
add_sources(sources, "src", ".cpp")
add_sources(sources, "pixelpart-runtime", ".cpp")
add_sources(sources, "pixelpart-runtime/zlib", ".c")

# Target file extension
target_ext = env["SHLIBSUFFIX"]
if env["platform"] == "ios":
    target_ext = ".a"

# Target file path
target_path = "pixelpart-plugin/addons/pixelpart/bin/libpixelpart.{}-{}{}".format(
    env["platform"], env["arch"], target_ext)

if env["platform"] == "ios":
    library = env.StaticLibrary(target_path, source=sources)
else:
    library = env.SharedLibrary(target_path, source=sources)

Default(library)