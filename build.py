import sys
import subprocess

if "platform=windows" in sys.argv:
    subprocess.run("scons platform=windows arch=x86_32 target=template_release generate_bindings=true disable_exceptions=false", shell = True)
    subprocess.run("scons platform=windows arch=x86_64 target=template_release generate_bindings=true disable_exceptions=false", shell = True)

elif "platform=linux" in sys.argv:
    subprocess.run("scons platform=linux arch=x86_32 target=template_release generate_bindings=true disable_exceptions=false", shell = True)
    subprocess.run("scons platform=linux arch=x86_64 target=template_release generate_bindings=true disable_exceptions=false", shell = True)

elif "platform=macos" in sys.argv:
    subprocess.run("scons platform=macos arch=universal target=template_release generate_bindings=true disable_exceptions=false", shell = True)

elif "platform=ios" in sys.argv:
    subprocess.run("scons platform=ios arch=universal target=template_release ios_simulator=yes generate_bindings=true disable_exceptions=false", shell = True)
    subprocess.run("scons platform=ios arch=arm64 target=template_release ios_simulator=no generate_bindings=true disable_exceptions=false", shell = True)

    subprocess.run("xcodebuild -create-xcframework " +
        "-library pixelpart-plugin/addons/pixelpart/bin/libpixelpart.ios-arm64.a " +
        "-library pixelpart-plugin/addons/pixelpart/bin/libpixelpart.ios-universal.simulator.a " +
        "-output pixelpart-plugin/addons/pixelpart/bin/libpixelpart.ios.xcframework", shell = True)
    subprocess.run("xcodebuild -create-xcframework " +
        "-library godot-cpp/bin/libgodot-cpp.ios.template_release.arm64.a " +
        "-library godot-cpp/bin/libgodot-cpp.ios.template_release.universal.simulator.a " +
        "-output pixelpart-plugin/addons/pixelpart/bin/libgodot-cpp.ios.xcframework", shell = True)

elif "platform=android" in sys.argv:
    subprocess.run("scons platform=android arch=arm32 target=template_release generate_bindings=true disable_exceptions=false", shell = True)
    subprocess.run("scons platform=android arch=arm64 target=template_release generate_bindings=true disable_exceptions=false", shell = True)
    subprocess.run("scons platform=android arch=x86_32 target=template_release generate_bindings=true disable_exceptions=false", shell = True)
    subprocess.run("scons platform=android arch=x86_64 target=template_release generate_bindings=true disable_exceptions=false", shell = True)

elif "platform=web" in sys.argv:
    subprocess.run("scons platform=web arch=wasm32 threads=yes target=template_release generate_bindings=true disable_exceptions=false", shell = True)
    subprocess.run("scons platform=web arch=wasm32 threads=no target=template_release generate_bindings=true disable_exceptions=false", shell = True)