import sys
import subprocess

if "platform=windows" in sys.argv:
    subprocess.run("scons platform=windows bits=32 target=release generate_bindings=true", shell = True)
    subprocess.run("scons platform=windows bits=64 target=release generate_bindings=true", shell = True)

elif "platform=linux" in sys.argv:
    subprocess.run("scons platform=linux bits=32 target=release generate_bindings=true", shell = True)
    subprocess.run("scons platform=linux bits=64 target=release generate_bindings=true", shell = True)

elif "platform=osx" in sys.argv:
    subprocess.run("scons platform=osx bits=64 target=release generate_bindings=true", shell = True)

elif "platform=android" in sys.argv:
    subprocess.run("scons platform=android android_arch=armv7 target=release generate_bindings=true", shell = True)
    subprocess.run("scons platform=android android_arch=arm64v8 target=release generate_bindings=true", shell = True)
    subprocess.run("scons platform=android android_arch=x86 target=release generate_bindings=true", shell = True)
    subprocess.run("scons platform=android android_arch=x86_64 target=release generate_bindings=true", shell = True)

elif "platform=ios" in sys.argv:
    subprocess.run("scons platform=ios ios_arch=arm64 target=release generate_bindings=true", shell = True)

elif "platform=web" in sys.argv:
    subprocess.run("scons platform=web bits=32 target=release generate_bindings=true", shell = True)