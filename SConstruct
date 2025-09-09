#!/usr/bin/env python
import os

def add_sources(sources, dir, extension):
    for f in os.listdir(dir):
        if f.endswith(extension):
            sources.append(dir + "/" + f)

# Scons environment
env = SConscript("godot-cpp/SConstruct")

# Includes
env.Append(CPPPATH=["src/", "."])

# Sources to compile
sources = []
add_sources(sources, "src", ".cpp")
add_sources(sources, "src/node", ".cpp")
add_sources(sources, "src/particletype", ".cpp")
add_sources(sources, "src/property", ".cpp")
add_sources(sources, "src/rendering", ".cpp")
add_sources(sources, "src/util", ".cpp")
add_sources(sources, "pixelpart-runtime/asset", ".cpp")
add_sources(sources, "pixelpart-runtime/common", ".cpp")
add_sources(sources, "pixelpart-runtime/computegraph", ".cpp")
add_sources(sources, "pixelpart-runtime/effect", ".cpp")
add_sources(sources, "pixelpart-runtime/engine", ".cpp")
add_sources(sources, "pixelpart-runtime/shadergraph", ".cpp")
add_sources(sources, "pixelpart-runtime/zlib", ".c")

# Target file extension
target_ext = env["SHLIBSUFFIX"]
if env["platform"] == "ios":
    target_ext = ".a"

# Additional identifier
additional_identifier = ""
if env["platform"] == "web":
    additional_identifier = ".threads" if env["threads"] else ".nothreads"
elif env["platform"] == "ios":
    additional_identifier = ".simulator" if env["ios_simulator"] else ""

# Target file path
target_path = "pixelpart-plugin/addons/pixelpart/bin/libpixelpart.{}-{}{}{}".format(
    env["platform"], env["arch"], additional_identifier, target_ext)

# Disable multi-threading in web build
if env["platform"] != "web":
    env.Append(CPPFLAGS=["-DPIXELPART_RUNTIME_MULTITHREADING"])

# Fix for zlib
if env["platform"] != "windows":
    env.Append(CFLAGS=["-DHAVE_UNISTD_H"])

if env["platform"] == "ios":
    library = env.StaticLibrary(target_path, source=sources)
else:
    library = env.SharedLibrary(target_path, source=sources)

Default(library)
