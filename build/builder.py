#!/usr/bin/python
# -*- coding: utf-8 -*-
import os
import sys
import shutil
import platform
import subprocess
from operator import eq

frame = "======================================================="

def cmdstr(cmd):
    ret = str(subprocess.check_output(cmd, shell=True))[2:-3]
    if platform.system() == "Windows":
        ret = "c:/" + ret[3:]

    print("cmdstr=" + ret)
    return ret

python3 = ""
def branch(command):
    if command == "help":
        return help()
    elif command == "history":
        return history()
    elif command == "clean":
        return clean()
    elif command == "commit":
        return commit()
    elif command == "build":
        return build()
    elif command == "run":
        arg = None if len(sys.argv) < 3 else sys.argv[2]
        return run(arg)
    elif command == "doc":
        return doc()

    print(command + " is unknown.")
    return -1

def _cleanIntermediates():
    print("removing intermediate outputs...", end=" ")
    os.system("rm -rf " + cwd + "/xml")
    os.system("rm -rf " + cwd + "/*.tmp")
    os.system("git config --unset user.name") # remove local config only
    os.system("git config --unset user.email")
    print("done.")

def doc():
    # Idea from Travis Gockel.
    global cwd, python3

    _cleanIntermediates()
    os.system("rm -rf " + cwd + "/html")

    # standby gh-pages repo:
    print("cloning gh-pages branch...", end=" ")
    res = os.system("git clone -b gh-pages --depth 5 git@github.com:kniz/worldlang.git --single-branch " + cwd + "/html")
    if res != 0:
        print("fail to clone gh-pages repo.")
        _cleanIntermediates()
        return -1
    print("done.")
    os.system("git rm -rf " + cwd + "/html")

    # build doxygen + m.css:
    print("generating docs using doxygen...", end=" ")
    res = os.system(python3 + " " + cwd + "/m.css/doxygen/dox2html5.py " + cwd + "/Doxyfile")
    if res != 0:
        print("fail to run m.css doxy parser.")
        _cleanIntermediates()
        return -1
    print("done.")

    # pushing on gh-pages:
    origin = cmdstr("git rev-parse --verify HEAD")
    print("origin=" + str(origin))
    os.chdir(cwd + "/html")
    os.system("git add .")
    os.system("git config user.name \"autodocbot\"") # put on local config.
    os.system("git config user.email \"knizofficial@gmail.com\"")
    res = os.system("git commit -m \"The our poor little Autobot \(❍ᴥ❍ʋ)/ generated docs for " + origin + ", clitter-clatter.\"")
    if res != 0:
        print("fail to commit on gh-pages.")
        print("it seems that nothing changed.")
        _cleanIntermediates()
        return -1
    res = os.system("git push origin gh-pages")
    if res != 0:
        print("fail to push on gh-pages")
        _cleanIntermediates()
        return -1
    os.chdir(cwd)

    _cleanIntermediates()
    return 0

def run(arg):
    if arg is None:
        print("build world and run one of next followings...")
        print("\t * unittests")
        return -1

    if arg == "unittests":
        result = build()
        if result:
            return result
        return _ut()

# currently this application only supports window and linux.
def isWindow():
    return platform.system().find("Windows") >= 0

# def _beautify():
#     print("beautify source codes...")
#     for path, dirs, files in os.walk("..\\"):
#         for file in files:
#             file_path = os.path.join(path, file)
#             ext = os.path.splitext(file_path)[1]
#             if  ext == ".cpp" or ext == ".c" or ext == ".hpp" or ext == ".h" or \
#                 ext == ".inl":
#                 # change encoding:
#                 # beautify
#                 print("\t" + file)
#                 os.system("astyle --style=world " + file_path)

def _createMakefiles():
    print("")
    generator = "MinGW Makefiles" if isWindow() else "Unix Makefiles"
    print("generating makefiles as " + generator + "...")

    res = os.system("cmake . -G \"" + generator + "\"")
    if res != 0:
        return -1

# World uses BuildInfo at RELEASE.md
# and builder.py drives to all world libraries and make it sync all build-info including 
# doc releasing.
ver_major = 0
ver_minor = 0
ver_fix = 0
ver_name = ""
ver_buildcnt = 0
def _extractBuildInfo(): # from RELEASE.md at root directory.
    global cwd, ver_major, ver_minor, ver_fix, ver_name, ver_buildcnt
    path = cwd + "/../RELEASE.md"

    fp = open(path, "r")
    while True:
        line = fp.readline()
        if not line: break
        if line[:4] in "## v":
            minor_head_n = line.find('.', 5) + 1
            ver_major = int(line[4:minor_head_n-1])
            ver_minor = int(line[minor_head_n: minor_head_n+1])
            ver_name_n = line.find(' ', minor_head_n+1)+1
            ver_fix_str = line[minor_head_n+1: ver_name_n]
            if ver_fix_str in "" or ver_fix_str in " ":
                ver_fix = 0
            else:
                ver_fix = int(ver_fix_str)
            ver_name = line[ver_name_n: len(line)-6-1-1]
            break

    fp.close()
updated = False
def _updateLine(lines, n, trg, basestr):
    global updated
    idx = len(basestr)-1
    value = int(lines[n][idx:len(lines[n])-2])
    if value != trg:
        lines[n] = basestr + str(trg) + ")\n"
        print(lines[n])
        updated = True

def _updateLineString(lines, n, trg, basestr):
    global updated
    idx = len(basestr)-1
    value = lines[n][idx:len(lines[n])-2]
    if eq(value, trg):
        lines[n] = basestr + str(trg) + ")\n"
        print(lines[n])
        updated = True

def _injectBuildInfo():
    global cwd, ver_major, ver_minor, ver_fix, ver_name, ver_buildcnt
    path = cwd + "/CMakeLists.txt"

    print("updating buildinfo on CMakeLists.txt...", end=" ")
    global updated
    updated = False;
    fp = open(path, "r")
    lines = fp.readlines()
    for n in range(0, len(lines)):
        line = lines[n]
        if line[:17] in "set(VERSION_MAJOR":
            _updateLine(lines, n, ver_major, "set(VERSION_MAJOR ")
        elif line[:17] in "set(VERSION_MINOR":
            _updateLine(lines, n, ver_minor, "set(VERSION_MINOR ")
        elif line[:15] in "set(VERSION_FIX":
            _updateLine(lines, n, ver_fix, "set(VERSION_FIX ")
        elif line[:16] in "set(VERSION_NAME":
            _updateLineString(lines, n, ver_name, "set(VERSION_NAME ");
    fp.close()

    if updated == False:
        print("skip")
        return

    fp = open(path, "w")
    fp.write("".join(lines)) 
    fp.close()

def _incBuildCnt():
    global cwd
    path = cwd + "/CMakeLists.txt"

    print("Increase Build count...", end=" ")
    fp = open(path, "r")
    lines = fp.readlines()
    buildcnt = 0
    for n in range(0, len(lines)):
        line = lines[n]
        if line[:15] in "set(BUILD_COUNT":
            buildcnt = int(line[16:len(line)-2])
            lines[n] = "set(BUILD_COUNT " + str(buildcnt+1) + ")\n"
            break
    fp.close()

    fp = open(path, "w")
    fp.write("".join(lines))
    fp.close()
    print("done")

def _make():
    print("")
    make_option = "-j4 -s"  # j4 -> 4 multithread.
                            # s ->  don't print command.
    print("making " + make_option + "...", end=" ")
    if isWindow():
        os.system("mingw32-make -v")
        result = os.system("mingw32-make " + make_option)
        if result != 0:
            print("failed")
            return -1
    else:
        os.system("make -v")
        result = os.system("make " + make_option)
        if result != 0:
            print("failed")
            return -1
    print("done")

def build():
    #_beautify()
    _injectBuildInfo()
    if _createMakefiles(): 
        return -1
    if _make():
        return -1
    if _ut():
        return -1
    _incBuildCnt()
    return 0

def _ut():
    print("")
    print("let's initiate unit tests...", end=" ")
    global cwd
    res = os.system(cwd + "/unittests")
    if res == 0:
        print("done")
    return res

def commit():
    return 0

def _extractPythonVersion(verstr):
    return float(verstr[7:10])

def checkDependencies():
    global python3
    print("")
    print ("checking dependencies...", end=" ")
    simple_depencies = ["git", "cmake", "java"]

    for e in simple_depencies:
        if not shutil.which(e):
            print("[!] " + e + " is NOT installed!")
            return -1

    if isWindow():
        if not shutil.which("mingw32-make"):
            print("[!] mingw32-make on Windows is NOT installed!")
            return -1
    elif not shutil.which("make"):
        print("[!] make for linux is NOT installed!")
        return -1

    if _extractPythonVersion(cmdstr(python3 + " --version")) < 3.6:
        print("[!] requires python over v3.6")
        return -1
    print("done")

def version():
    global ver_name, ver_major, ver_minor, ver_fix, cwd, python3
    print("Builder. Support-utility for building World " + ver_name + " v" + str(ver_major) + "." + str(ver_minor) + str(ver_fix))
    print("Copyrights (c) kniz, 2009-2018")
    print(frame)
    print("")
    print("* use python version at " + python3 + " and its version is " + cmdstr(python3 + " --version"))
    print("* building directory is " + cwd)

def help():
    print("Usage: builder.py <command> <arg1> <arg2> ...")
    print("")
    print("command list:")
    print("\t * help")
    print("\t * history")
    print("\t * clean\tclear all cache files of cmake outputs.")
    print("\t * build")
    print("\t * run\t\tbuild + run one of predefined programs.")

def clean():
    print("Clearing next following files...")
    global cwd
    _clean(cwd)
    _cleanIntermediates()
    os.system("rm -rf " + cwd + "/html")
    print("was removed successfully.")

def _clean(directory):
    for path, dirs, files in os.walk(directory):

        for file in files:
            file_path = os.path.join(path, file)
            ext = os.path.splitext(file_path)[1]
            if  ext == ".sln" or ext == ".vcproj" or ext == ".vcxproj" or ext == ".cmake" or \
                file == "CMakeCache.txt" or ext == ".stamp" or ext == ".depend" or file == "Makefile" or \
                ext == ".exe" or ext == ".dll" or ext == ".log" or ext == ".a" or ext == ".so":
                print("\t * " + file_path)
                os.remove(file_path)

        for dir in dirs:
            if dir == "CMakeFiles" or dir == "tmp" or dir == "generated":
                abs_dir = os.path.join(path, dir)
                print("\t * " + abs_dir)
                shutil.rmtree(abs_dir)
cwd = ""

def _extractEnv():
    global python3
    if "PYTHON" in os.environ:
        python3 = os.environ["PYTHON"]
    else:
        cmd = ""
        if platform.system() == "Linux":
            cmd = "which"
        elif platform.system() == "Windows":
            cmd = "where"
        else:
            print("[!] " + platform.system() + " unsupported.")
            return -1

        no_python3 = os.system(cmd + " python3")
        print("no_python3=" + str(no_python3))
        python3 = cmdstr("which python")
        #python3 = cmdstr(cmd + " python") if no_python3 else cmdstr(cmd + " python3")
        print("python3=" + python3)
        return 0

def _init():
    global cwd
    cwd=os.path.dirname(os.path.realpath(sys.argv[0]))
    _extractBuildInfo()
    return _extractEnv()

def main():
    if _init():
        return -1
    version()

    os.chdir(cwd)
    if checkDependencies():
        print("\n[!] This program needs following softwares to be fully functional.")
        return -1

    if len(sys.argv) == 1:
        help()
    else:
        return branch(sys.argv[1])

    print("")
    return 0

ret = main()
if ret != 0:
    print("[!] ends with " + str(ret) + " exit code.")
sys.exit(ret)
