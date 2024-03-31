import sys
import subprocess

if "platform=linux" in sys.argv:
    subprocess.run("scons platform=linux arch=x86_32 target=template_release generate_bindings=true", shell = True)
    subprocess.run("scons platform=linux arch=x86_64 target=template_release generate_bindings=true", shell = True)

elif "platform=macos" in sys.argv:
    subprocess.run("scons platform=macos arch=x86_64 target=template_release generate_bindings=true", shell = True)

elif "platform=windows" in sys.argv:
    subprocess.run("scons platform=windows arch=x86_32 target=template_release generate_bindings=true", shell = True)
    subprocess.run("scons platform=windows arch=x86_64 target=template_release generate_bindings=true", shell = True)

elif "platform=android" in sys.argv:
    subprocess.run("scons platform=android arch=arm32 target=template_release generate_bindings=true", shell = True)
    subprocess.run("scons platform=android arch=arm64 target=template_release generate_bindings=true", shell = True)
    subprocess.run("scons platform=android arch=x86_32 target=template_release generate_bindings=true", shell = True)
    subprocess.run("scons platform=android arch=x86_64 target=template_release generate_bindings=true", shell = True)

elif "platform=ios" in sys.argv:
    subprocess.run("scons platform=ios arch=universal target=template_release generate_bindings=true", shell = True)

elif "platform=web" in sys.argv:
    subprocess.run("scons platform=javascript arch=wasm32 target=template_release generate_bindings=true", shell = True)